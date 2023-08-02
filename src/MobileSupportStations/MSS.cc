/*
 * MSS.cc
 *
 *  Created on: May 7, 2019
 *      Author: dwilhelm
 */

#include "MSS.h"

#include <omnetpp/checkandcast.h>
#include <omnetpp/clog.h>
#include <omnetpp/cmessage.h>
#include <omnetpp/cnamedobject.h>
#include <omnetpp/cpacket.h>
#include <omnetpp/cpar.h>
#include <omnetpp/csimulation.h>
#include <omnetpp/simtime.h>
#include <omnetpp/simtime_t.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>
#include <map>
#include <string>
#include <tuple>
#include <utility>

#include "../../omnetpp-5.5.1/samples/inet/src/inet/common/InitStages.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/common/IntrusivePtr.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/common/packet/chunk/FieldsChunk.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/common/packet/Message.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/common/Ptr.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/common/Units.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/networklayer/common/InterfaceEntry.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/networklayer/common/L3Address.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/networklayer/contract/ipv4/Ipv4Address.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/transportlayer/contract/tcp/TcpCommand_m.h"
#include "../../omnetpp-5.5.1/samples/inet/src/inet/transportlayer/contract/udp/UdpSocket.h"

#include "../../omnetpp-5.5.1/samples/inet/src/inet/networklayer/common/L3AddressTag_m.h"
#include "../MobileHosts/MH.h"

/*                              Module functions                                    */

void MSS::initialize(int stage)
{
	Communications::initialize(stage);

	if (stage == INITSTAGE_APPLICATION_LAYER)
	{
		BC = dynamic_cast<BroadcastController*>(getModuleByPath("Arbre.BCApp.app[0]"));
		params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGE_NAME) + ".simParams").c_str()));
		MSSb.initialize(id, params->MSSbGroupSize);
		FIFOorder.resize(params->nbMobileSupportStations);
		SBuffer.setBC(BC);
	}
	else if (stage == INITSTAGE_LAST)
	{
		InterfaceTable* t = dynamic_cast<InterfaceTable*>(getModuleByPath(par("interfaceTableModule")));
		wlanInterfaceId = t->getInterfaceByName("wlan0")->getInterfaceId();
	}
}

void MSS::handleMessageWhenUp(cMessage *msg)
{
	if (msg->isSelfMessage())
		handleSelfMessage(msg);
	else
		handleReceivedMessage(msg);
}

void MSS::handleSelfMessage(cMessage* msg)
{
	if (msg == &sendSeqTimeout)
		SendACK(msg);
	else if (msg == &sendMinMsg)
		sendMinMessages();
	else if (dynamic_cast<AlgoTimeout*>(msg) != nullptr)
		handleTimeout(msg);
}

void MSS::handleReceivedMessage(cMessage* msg)
{
	if (TcpSocket *socketMsg = check_and_cast_nullable<TcpSocket*>(MSSsocketMap.findSocketFor(msg))) // New TCP connection
		socketMsg->processMessage(msg);
	else if (socketCoMSS.belongsToSocket(msg)) // Message from a station
		socketCoMSS.processMessage(msg);
	else
		// Message from a mobile
		socketMH.processMessage(msg);
}

void MSS::handleTimeout(cMessage* msg)
{
	if (TimeoutHandoff* tmp = dynamic_cast<TimeoutHandoff*>(msg))
	{
		SendReq_1(tmp->getSeq_S(), tmp->getIdMH(), tmp->getSesLC(), tmp->getSes());
		delete msg; // is scheduled in sendReq1
	}
	else if (TimeoutAppS_h * m = dynamic_cast<TimeoutAppS_h*>(msg))
	{
		scheduleMessageLoadDependantWithPartialProportionnality(m, m->getSeq(), SBuffer.getMinSeqNC(),
				SBuffer.getMaxSeqNC());
		struct s_sendMsgMSS mResend = SBuffer.getBroadcastedMessage(m->getSeq());
		sendMessage<AppS_h>(MessageFactory::createAppS_h(mResend.seqNC, id, mResend.MHdeliv, mResend.id));
	}
	else if (TimeoutAppCo * m = dynamic_cast<TimeoutAppCo*>(msg))
	{
		vector<Session>::iterator session = Mobiles.findSession(m->getId_h(), m->getSes());
		scheduleMessageLoadDependantWithPartialProportionnality(m, m->getSeq(),
				session->getCoMsgDeliver().front().seqNC, session->getCoMsgDeliver().back().seqNC);
		list<s_sendMsgMSS>::iterator it = session->getCoMsgDeliver().searchMsgIt(m->getSeq());
		sendMessage<AppCo>(MessageFactory::createAppCo(id, it->seqNC, m->getId_h(), m->getSes(), it->id));
	}
}

void MSS::handleStartOperation(LifecycleOperation *operation)
{
	Communications::handleStartOperation(operation);
	scheduleAt(simTime() + 2, &sendSeqTimeout);
	std::cerr << "WARNING: sendMinMessage() deactivated" << endl;
	//        scheduleAt(simTime() + 2 , sendMinMsg);
	if (simTime() > 2) // After crash
	{
		cerr << "MSS " << id << " STARTING AT " << simTime();
		MSSb.setLastMsgMSSbCurrentTime();
		sendToStations(MessageFactory::createRecoverMSSReq(id, ++seqMsg, id));
		recovering = true;
	}
}

void MSS::handleStopOperation(LifecycleOperation *operation)
{
	//socketMH.close();
	delayActiveOperationFinish(par("stopOperationTimeout"));
}

void MSS::handleCrashOperation(LifecycleOperation *operation)
{
	Communications::handleCrashOperation(operation);
	cerr << "MSS " << id << " CRASH AT " << simTime() << endl;

	cancelAndDeleteTimeouts(SBuffer.clear());
	cancelAndDeleteTimeouts(Mobiles.clear());
	ReassembleBuffers.clear();
	cancelEvent(&sendSeqTimeout);
	cancelEvent(&sendMinMsg);
}

void MSS::socketDataArrived(UdpSocket *socket, Packet *packet)
{
	if (!recovering)
	{
		const IntrusivePtr<const HostMessage> msg = dynamic_pointer_cast<const HostMessage>(packet->popAtFront());
		if (msg->getCellId() == id)
		{
			stats.receivedMessages++;
			if (const IntrusivePtr<const Init> m = dynamic_pointer_cast<const Init>(msg))
				ReceiveInit(m);
			else if (const IntrusivePtr<const Apph_S> m = dynamic_pointer_cast<const Apph_S>(msg))
				ReceiveApph_S(m);
			else if (const IntrusivePtr<const Ack_h> m = dynamic_pointer_cast<const Ack_h>(msg))
				ReceiveACK(m);
			else if (const IntrusivePtr<const AppCoAck> m = dynamic_pointer_cast<const AppCoAck>(msg))
				ReceiveAppCoACK(m);
			else
			{
				cerr << "UDP message false message type" << endl;
				exit(1);
			}
		}
	}
	delete packet;
}

void MSS::routing(const IntrusivePtr<const FieldsChunk> m, Packet * sourcePacket)
{
	if (const IntrusivePtr<const RecoverMSS> msg = dynamic_pointer_cast<const RecoverMSS>(m))
		ReceiveRecoverMSS(msg);
	if (recovering) // Only handles RecoverMSS messages during recovery procedure
		return;
	else if (const IntrusivePtr<const AppS_S> msg = dynamic_pointer_cast<const AppS_S>(m))
		ReceiveAppS_S(msg, sourcePacket);
	else if (const IntrusivePtr<const AckSS> msg = dynamic_pointer_cast<const AckSS>(m))
		ReceiveAckSS(msg);
	else if (const IntrusivePtr<const App_resp> msg = dynamic_pointer_cast<const App_resp>(m))
		ReceiveApp_sresp(msg, sourcePacket);
	else if (const IntrusivePtr<const Req1> msg = dynamic_pointer_cast<const Req1>(m))
		ReceiveReq_1(msg);
	else if (const IntrusivePtr<const Rsp1> msg = dynamic_pointer_cast<const Rsp1>(m))
		ReceiveRsp_1(msg);
	else if (const IntrusivePtr<const Req2> msg = dynamic_pointer_cast<const Req2>(m))
		ReceiveReq_2(msg);
	else if (const IntrusivePtr<const Rsp2> msg = dynamic_pointer_cast<const Rsp2>(m))
		ReceiveRsp_2(msg);
	else if (const IntrusivePtr<const Delete> msg = dynamic_pointer_cast<const Delete>(m))
		ReceiveDelete(msg);
	else if (const IntrusivePtr<const RecoverMSSReq> msg = dynamic_pointer_cast<const RecoverMSSReq>(m))
		ReceiveRecoverMSSReq(msg);
	else if (const IntrusivePtr<const Register> msg = dynamic_pointer_cast<const Register>(m))
		ReceiveRegister(msg);
	else
	{
		cerr << "Unkown message type, message : " << m << endl;
		exit(1);
	}
}

void MSS::clearSBuffer(unsigned int seq)
{
	cancelAndDeleteTimeouts(SBuffer.clearSBuffer(seq, id, BC));
}

void MSS::print()
{
	cout << "TIME: " << simTime() << " MSS " << id;
}

// Kept in class MSS instead of Communications because of template compilation in C++
template<typename T> void MSS::sendMessage(IntrusivePtr<T> msg)
{
	resendPacket->insertAtBack(msg);
	resendcpt++;
	if (resendcpt == MSGPERPACKET)
	{
		sendToWifiInterface(resendPacket);
		stats.totalExpectedReceive += Mobiles.nbHostsInCell(id);
		resendPacket = new Packet("Resend");
		resendcpt = 0;
	}
	stats.theoreticalNbWifiMessages++;
}

unsigned int MSS::searchMinSeq()
{
	unsigned int minSeq = (Mobiles.empty() ? SBuffer.getSeqC() : Mobiles.searchMinSeq());
	return minSeq;
}

void MSS::sendMinMessages()
{
	scheduleMessageLoadDependant(&sendMinMsg);
	if (SBuffer.empty())
		return;
	vector<unsigned int> v = Mobiles.getMinSeqCRegisteredMH(id);
	sort(v.begin(), v.end());
	v.erase(unique(v.begin(), v.end()), v.end()); // delete duplicates
	for (unsigned int i : v)
	{
		s_sendMsgMSS it = SBuffer.getBroadcastedMessage(i);
		sendMessage<AppS_h>(MessageFactory::createAppS_h(it.seqNC, id, it.MHdeliv, it.id));
	}
}

void MSS::scheduleUrgentMessage(cMessage* m)
{
	scheduleAt(simTime() + params->checkPositif(params->getRandTime(SimTime(20, SIMTIME_MS))), m);
}

// Schedules message following the system's message load
// For a message load of 100msg/s returns 500ms
// For a message load of 200msg/s returns 250ms
void MSS::scheduleMessageLoadDependant(cMessage* m)
{
	double messageLoad = params->nbMobileHosts / params->AppMFrequency;
	simtime_t delay = SimTime(50. / messageLoad * 1000, SIMTIME_MS);
	scheduleAt(simTime() + params->checkPositif(params->getRandTime(delay)), m);
}

void MSS::scheduleMessageLoadDependantWithPartialProportionnality(cMessage* m, unsigned int seq, unsigned int min,
		unsigned max)
{
	double messageLoad = params->nbMobileHosts / params->AppMFrequency;
	simtime_t base = SimTime(50. / messageLoad * 1000, SIMTIME_MS);
	double proportion = (seq + 1 - min) * 1. / (max + 1 - min); // +1 to handle the case when min = max
	simtime_t delay = 1. / 2 * base + 1. / 2 * base * proportion;
	scheduleAt(simTime() + params->checkPositif(params->getRandTime(delay)), m);
}

void MSS::cancelAndDeleteTimeouts(vector<cMessage*> timeouts)
{
	for (cMessage*t : timeouts)
		cancelAndDelete(t);
}

void MSS::cancelAndDeleteTimeouts(list<struct s_sendMsgMSS> messages)
{
	for (struct s_sendMsgMSS& m : messages)
		cancelAndDelete(m.timeout);
}

// Functions to handle acknowledgments
void MSS::SendACK(cMessage *msg)
{
	scheduleMessageLoadDependant(msg);

	vector<idMsg> vseq = Mobiles.createAckHVectorConnectedHosts(id);
	sendMessage(MessageFactory::createAck_S(id, vseq));

	vector<tuple<unsigned int, unsigned int, unsigned int>> vseqAckSS = Mobiles.createAckSSVectorConnectedHosts(id);
	sendToStations(MessageFactory::createAckSS(id, vseqAckSS, id, ++seqMsg));
}

void MSS::ReceiveACK(const IntrusivePtr<const Ack_h>& m)
{
	if (Mobiles.isSessionRegistered(m->getId_h(), m->getSes()))
	{
		vector<Session>::iterator session = Mobiles.findSession(m->getId_h(), m->getSes());
		if (session->connectionIsEstablished())
		{
			session->updateAcknowledgment(m->getSeq_S(), m->getIntervals());
			checkStopSendSBufferMessages(m->getIntervals());
		}
	}
}

void MSS::ReceiveAckSS(const IntrusivePtr<const AckSS>& m)
{
	if (MSSb.contains(m->getIdS()))
	{
		for (tuple<unsigned int, unsigned int, unsigned int> t : m->getVSeq())
		{
			list<struct s_sendMsgMSS> acknowledgedAppCo = Mobiles.acknowledgeAppCo(get<0>(t), get<2>(t), get<1>(t));
			SBuffer.tagAlreadyDeliveredMessagesByMH(acknowledgedAppCo, get<0>(t)); // adds id of mh to those messages if they are stored in SBuffer
		}
	}
}

void MSS::ReceiveAppCoACK(const IntrusivePtr<const AppCoAck>& m)
{
	if (Mobiles.isSessionRegistered(m->getId_h(), m->getSes()))
	{
		vector<Session>::iterator session = Mobiles.findSession(m->getId_h(), m->getSes());
		if (session->connectionInHandoff())
		{
			list<struct s_sendMsgMSS> acknowledgedAppCo = session->getCoMsgDeliver().clear(m->getSeq_S());
			SBuffer.tagAlreadyDeliveredMessagesByMH(acknowledgedAppCo, m->getId_h());
			cancelAndDeleteTimeouts(acknowledgedAppCo);

			if (session->getCoMsgDeliver().empty())
				FinalizeInit(m->getId_h(), m->getSes());
		}
		else
		{
			vector<tuple<idMsg, unsigned int>> msgOrder = SBuffer.getMessagesOrderBroadcastedMessages();
			sendMessage(
					MessageFactory::createInitAck(id, m->getId_h(), session->seqh, session->seqc, session->Ses,
							msgOrder));
		}
	}
}

void MSS::checkStopSendSBufferMessages(const vector<AckInterval>& ackIntervals)
{
	vector<cMessage*> timeouts = SBuffer.checkStopBroadcastMessages(Mobiles, ackIntervals, id);
	cancelAndDeleteTimeouts(timeouts);
	clearSBuffer(searchMinSeq());
}

void MSS::ReceiveInit(const IntrusivePtr<const Init> &m)
{
	int id_h = m->getTargetHostId();

	if (Mobiles.isSessionRegistered(id_h, m->getSes()))
		handleExistingConnection(id_h, m->getSes());
	else
		handleNewConnection(id_h, m);
}

void MSS::handleExistingConnection(unsigned int id_h, unsigned int Ses)
{
	vector<Session>::iterator session = Mobiles.findSession(id_h, Ses);
	if (session->connectionIsEstablished())
	{
		vector<tuple<idMsg, unsigned int>> msgOrder = SBuffer.getMessagesOrderBroadcastedMessages();
		sendMessage(MessageFactory::createInitAck(id, id_h, session->seqh, session->seqc, session->Ses, msgOrder));
		resendMessageSBuffer(id_h);
	}
}

void MSS::handleNewConnection(unsigned int id_h, const IntrusivePtr<const Init> &m)
{
	if (Mobiles.hasSessionInCharge(id_h, id)) // another session of the host is registered
	{
		vector<Session>::iterator session = Mobiles.getSessionInCharge(id_h, id);
		if (!session->isInHandoff())
		{
			if (m->getSes_LC() == session->Ses) // was connected to this station, moved but did not succeed to connect to another station, then came back
			{
				list<struct s_sendMsgMSS> acknowledgedAppCo = session->getCoMsgDeliver().clear(m->getSeq_S());
				SBuffer.tagAlreadyDeliveredMessagesByMH(acknowledgedAppCo, id_h);
				cancelAndDeleteTimeouts(acknowledgedAppCo);
				SBuffer.tagAlreadyDeliveredMessagesByMH(id_h, m->getSeq_S());
			}
			session->Ses = m->getSes();
			session->getCoMsgDeliver().updateSeq();
			if (session->getCoMsgDeliver().empty())
			{
				FinalizeInit(id_h, m->getSes());
				resendMessageSBuffer(id_h); // to handle host failures
			}
			sendToStations(MessageFactory::createRegister(*session, id, ++seqMsg));
			SendDelete(id_h, session->Ses);
		}
	}
	else
		RegisterHost(id_h, m);
}

void MSS::RegisterHost(unsigned int id_h, const IntrusivePtr<const Init>& m)
{
	Session h(id_h, 0, SBuffer.getMinSeqNC() - 1, { { 0, 0 } }, m->getSes(), id); // getMinSeqNC()-1 to avoid the mss to delete any message during the handoff
	Mobiles.addSession(h, id_h);
	if (m->getState() == MHState::STATE::Join)
	{
		BC->HostSystemJoin(id_h, id);
		FinalizeInit(id_h, m->getSes());
		sendToStations(MessageFactory::createRegister(*Mobiles.findSession(id_h, m->getSes()), id, ++seqMsg));
		SendDelete(id_h, h.Ses);
	}
	else
		SendReq_1(m->getSeq_S(), m->getTargetHostId(), m->getSes_LC(), m->getSes());
}

void MSS::FinalizeInit(unsigned int id_h, unsigned int Ses)
{
	vector<Session>::iterator session = Mobiles.findSession(id_h, Ses);
	session->confirmConnection(SBuffer.determineSeqC(id_h));
	clearSBuffer(searchMinSeq());
	vector<tuple<idMsg, unsigned int>> msgOrder = SBuffer.getMessagesOrderBroadcastedMessages();
	if (session->isInCharge(id))
		sendMessage(MessageFactory::createInitAck(id, id_h, session->seqh, session->seqc, session->Ses, msgOrder));
}

void MSS::SendDelete(unsigned int id_h, unsigned int Ses)
{
	sendToStations(MessageFactory::createDelete(Ses, id_h, id, ++seqMsg));
	ReceiveDelete(MessageFactory::createDelete(Ses, id_h, id, seqMsg));
}

void MSS::ReceiveDelete(const IntrusivePtr<const Delete>& m)
{
	cancelAndDeleteTimeouts(Mobiles.deleteSessions(m->getId_h(), m->getSes()));
}

void MSS::ReceiveApph_S(const IntrusivePtr<const Apph_S> &m)
{
	if (Mobiles.isSessionRegistered(m->getId().idMH, m->getSes()))
	{
		vector<Session>::iterator session = Mobiles.findSession(m->getId().idMH, m->getSes());

		if (session->notInInitState()) // ensures that mss won't broadcast a message from h_i before it received all messages that h_i might have delivered when connected to another station
		{
			if (m->getId().seqMH == session->seqh + 1)
			{
				Disseminate(m->getId(), m->getSes());
				FIFODisseminate(m->getId().idMH, m->getSes());
			}
			else if (m->getId().seqMH > session->seqh + 1)
				session->RBuffer.insert(m);
			// otherwise the message was already received
		}
	}
}

void MSS::FIFODisseminate(unsigned int id_h, unsigned int Ses)
{
	vector<Session>::iterator session = Mobiles.findSession(id_h, Ses);
	idMsg idM;
	while (session->RBuffer.tryExtract(session->seqh + 1, idM)) // next message to disseminate
		Disseminate(idM, Ses);
}

void MSS::Disseminate(idMsg m, unsigned int Ses)
{
	vector<Session>::iterator session = Mobiles.findSession(m.idMH, Ses);
	sendToStations(MessageFactory::createAppS_S(id, m, id, ++seqMsg, Ses));
	session->seqh++;
	handleFIFOOrderedMessage(m);
}

void MSS::handleFIFOOrderedMessage(idMsg idM)
{
	struct s_sendMsgMSS m(-1, idM, nullptr, { }, FIFOorder);
	SBuffer.pendingMsgPushBack(m);
	if (MSSb.isGroupResponsible(id))
	{
		broadcastInCell(idM, SBuffer.getSeqC());
		sendToStations(MessageFactory::createApp_resp(idM, id, SBuffer.getSeqC(), id, ++seqMsg));
		SBuffer.incrementSeqC();
	}
}

void MSS::broadcastInCell(idMsg idM, unsigned int seqS)
{
	list<struct s_sendMsgMSS>::iterator m = SBuffer.push_backBroadcastedMessages(idM, seqS);
	scheduleMessageLoadDependantWithPartialProportionnality(m->timeout, m->seqNC, SBuffer.getMinSeqNC(),
			SBuffer.getMaxSeqNC());
	if (Mobiles.nbHostsInCell(id) == 0)
		BC->stationRemovesMessage(idM, id);
	else
		sendMessage<AppS_h>(MessageFactory::createAppS_h(seqS, id, m->MHdeliv, idM));
}

void MSS::ReceiveAppS_S(const IntrusivePtr<const AppS_S>& m, Packet* sourcePacket)
{
	handleFIFOOrderedMessage(m->getId());
	Mobiles.increasesAckh(m->getId().idMH);
}

void MSS::ReceiveApp_sresp(const IntrusivePtr<const App_resp>& m, Packet* sourcePacket)
{
	if (MSSb.contains(m->getSourceId()))
	{
		// if previous mss failed and this mss is now responsible, then it has already broadcasted this message and does only need to remove it (but should never happen !)
		//        if(!SBuffer.containsPendingApp(m->getId()))
		//           return;
		broadcastInCell(m->getId(), m->getSeqS());
	}
}

void MSS::ReceiveRecoverMSSReq(const IntrusivePtr<const RecoverMSSReq> &m)
{
	if (!MSSb.contains(m->getRecoveringMSSId()))
		sendToStations(m);
	else
		sendToStations(
				MessageFactory::createRecoverMSS(m->getRecoveringMSSId(), id, ++seqMsg, SBuffer, SBuffer.getSeqC(),
						MSSb.getGroupResponsible(), FIFOorder, Mobiles.Mobiles));
}

void MSS::ReceiveRecoverMSS(const IntrusivePtr<const RecoverMSS> &m)
{
	if (id != m->getMSScible())
		return;
	if (!recovering)
		return;
	SBuffer = m->getSBuffer();
	for (unsigned int i = SBuffer.getMinSeqNC(); i < SBuffer.getMaxSeqNC(); i++)
	{
		cMessage* m = MessageFactory::createTimeoutAppS_h(i);
		SBuffer.setTimerBroadcastedMessage(m, i);
		scheduleMessageLoadDependantWithPartialProportionnality(m, i, SBuffer.getMinSeqNC(), SBuffer.getMaxSeqNC());
	}
	FIFOorder = m->getReceived();
	MSSb.setGroupResponsible(m->getS_resp());
	if (MSSb.isGroupResponsible(id))
	{
		while (!SBuffer.hasPendingMessages())
		{
			struct s_sendMsgMSS m = SBuffer.extractFirstPendingMsg();
			sendToStations(MessageFactory::createApp_resp(m.id, id, SBuffer.getSeqC(), id, ++seqMsg));
			broadcastInCell(m.id, SBuffer.getSeqC()); // does pop_front in broadcastInCell
			SBuffer.incrementSeqC();
		}
	}

	Mobiles.Mobiles = m->getMobiles();
	manualHostReconnectAfterFailure();

	recovering = false;
}

void MSS::manualHostReconnectAfterFailure()
{
	// manually provokes the reconnection of the mobiles that were connected to this station
	MH* mh;
	char tmp[40];
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		sprintf(tmp, "Arbre.Mobiles[%d].app[0]", i);
		mh = dynamic_cast<MH*>(getModuleByPath(tmp));
		if (mh->id_S == id) // was connected to station
			mh->id_S = -1; // upon next computation of closest station will reconnect to id
	}
}

void MSS::SendReq_1(unsigned int seq_S, unsigned int id_h, unsigned int Ses_LC, unsigned int Ses)
{
	vector<Session>::iterator session = Mobiles.findSession(id_h, Ses);
	session->handoffTimeout = MessageFactory::createTimeoutHandoff(id_h, Ses, Ses_LC, seq_S);
	scheduleAt(simTime() + 1, session->handoffTimeout);
	sendToStations(MessageFactory::createReq1(id_h, seq_S, Ses_LC, Ses, MSSb, id, ++seqMsg));
}

void MSS::ReceiveReq_1(const IntrusivePtr<const Req1>& m)
{
	if (!Mobiles.hasSessionInCharge(m->getId_h(), id))
		return;
	vector<Session>::iterator session = Mobiles.getSessionInCharge(m->getId_h(), id);
	if (session->isInHandoff() || session->Ses > m->getSes()) // session is currently in handoff or is more recent than the requester session
		return;

	list<struct s_sendMsgMSS> acknowledgedAppCo = session->updateProcessReq1(FIFOorder, m->getSes(), m->getSeq_S(),
			m->getSes_LC());

	SBuffer.tagAlreadyDeliveredMessagesByMH(acknowledgedAppCo, m->getId_h());
	cancelAndDeleteTimeouts(acknowledgedAppCo);
	sendToStations(
			MessageFactory::createRsp1(m->getId_h(), session->seqh, buildVectorIdsNotDeliveredMessages(*session),
					m->getSes(), id, ++seqMsg));
}

vector<idMsg> MSS::buildVectorIdsNotDeliveredMessages(const Session& session)
{
	vector<idMsg> m_nd = session.getCoMsgDeliver().getMessageIds(); // Not delivered AppCo messages
	vector<idMsg> notDeliveredSBufferMessages = SBuffer.getIdNotDeliveredMessages(session.id, session.seqc); // Not delivered messages in sending buffer
	m_nd.insert(m_nd.end(), notDeliveredSBufferMessages.begin(), notDeliveredSBufferMessages.end());
	return m_nd;
}

void MSS::ReceiveRsp_1(const IntrusivePtr<const Rsp1> &m)
{
	if (!Mobiles.isSessionRegistered(m->getId_h(), m->getSes()))
		return;

	vector<Session>::iterator session = Mobiles.findSession(m->getId_h(), m->getSes());
	session->updateProcessRsp1(m->getSeq_h(), FIFOorder, m->getM_nd());

	sendToStations(
			MessageFactory::createReq2(m->getId_h(), SBuffer.notContainedMessages(m->getM_nd()), m->getSes(), MSSb, id,
					++seqMsg));
}

void MSS::ReceiveReq_2(const IntrusivePtr<const Req2>& m)
{
	if (!Mobiles.hasSessionRecvReq1(m->getId_h()))
		return;
	vector<Session>::iterator session = Mobiles.getSessionIfRecvReq1(m->getId_h());

	assertContainsRequestedMessages(m->getMsg_req(), session->getCoMsgDeliver());

	cancelAndDeleteTimeouts(session->getCoMsgDeliver().clear());

	sendToStations(
			MessageFactory::createRsp2(m->getId_h(), m->getMsg_req(),
					SBuffer.getIdMessagesReceivedAfter(session->recvReq1Vector), m->getSes(), id, ++seqMsg));
	//Mobiles.removeSession(id_h, m->getSes()); Will do it upon reception of DELETE. Otherwise the causal information of host will be lost if the requesting station fails before sending the information to its backup stations
}

void MSS::assertContainsRequestedMessages(const vector<idMsg>& requestedMessages,
		const StationAppCoSendingBuffer& AppCoMsgToDeliver)
{
	for (idMsg msg : requestedMessages)
		assert(AppCoMsgToDeliver.contains(msg) || SBuffer.contains(msg));
}

void MSS::ReceiveRsp_2(const IntrusivePtr<const Rsp2> &m)
{
	if (!Mobiles.isSessionRegistered(m->getId_h(), m->getSes()))
		return;

	vector<Session>::iterator session = Mobiles.findSession(m->getId_h(), m->getSes());

	SBuffer.tagAlreadyDeliveredMessagesByMH(session->recvRsp1Vector, session->notDeliveredMessages, m->getMsg_rcv(),
			m->getId_h());
	resendMessageSBuffer(m->getId_h());
	UpdateResendPacket();

	cancelAndDelete(
			session->updateProcessRsp2(
					buildAppCoMsg(SBuffer.getIdMessagesNotDeliveredNorBroadcasted(m->getId_h()), m->getMsg(),
							m->getId_h(), m->getSes())));
	if (session->getCoMsgDeliver().empty())
		FinalizeInit(m->getId_h(), m->getSes());
	else
		sendAppCoMsg(session->getCoMsgDeliver().buffer, m->getId_h(), m->getSes());

	sendToStations(MessageFactory::createRegister(*session, id, ++seqMsg));
	SendDelete(session->id, session->Ses);
}

list<struct s_sendMsgMSS> MSS::buildAppCoMsg(const vector<idMsg>& alreadyAcknowledgedMessages,
		const vector<idMsg>& alreadyDeletedMessages, unsigned int id_h, unsigned int Ses)
{
	list<struct s_sendMsgMSS> appCoMsg;
	unsigned int seqCo = 1;
	vector<idMsg> resultIds = alreadyDeletedMessages;
	resultIds.insert(resultIds.end(), alreadyAcknowledgedMessages.begin(), alreadyAcknowledgedMessages.end());

	for (const idMsg& elt : resultIds)
	{
		struct s_sendMsgMSS mt = s_sendMsgMSS(seqCo, elt, MessageFactory::createTimeoutAppCo(seqCo, id_h, Ses), { },
				{ });
		scheduleMessageLoadDependantWithPartialProportionnality(mt.timeout, seqCo, 1, resultIds.size());
		appCoMsg.push_back(mt);
		seqCo++;
	}
	return appCoMsg;
}

void MSS::sendAppCoMsg(const list<struct s_sendMsgMSS>& appCoMsg, unsigned int id_h, unsigned int Ses)
{
	list<struct s_sendMsgMSS>::const_iterator it = appCoMsg.begin();
	vector<tuple<idMsg, unsigned int>> messageOrder = buildMessageOrderList(appCoMsg); // builds the list that contains the sequence number order of messages broadcasted by that Station

	const auto& payloadMH = MessageFactory::createAppCo(id, it->seqNC, id_h, Ses, it->id);
	payloadMH->setMsgOrder(messageOrder); // appends to first appCoMessage the messageOrder list
	payloadMH->setChunkLength(payloadMH->getChunkLength() + B(messageOrder.size() * 2 * sizeof(idMsg)));
	sendMessage(payloadMH);
	it++;

	for (; it != appCoMsg.end(); it++)
	{
		const auto& payloadMH = MessageFactory::createAppCo(id, it->seqNC, id_h, Ses, it->id);
		sendMessage(payloadMH);
	}
}

vector<tuple<idMsg, unsigned int>> MSS::buildMessageOrderList(const list<struct s_sendMsgMSS>& appCoMsg)
{
	vector<tuple<idMsg, unsigned int>> messageOrder;
	for (const struct s_sendMsgMSS& t : appCoMsg)
		messageOrder.push_back(make_tuple(t.id, t.seqNC));
	vector<tuple<idMsg, unsigned int>> currentlyBroadcastedOrder = SBuffer.getMessagesOrderBroadcastedMessages();
	messageOrder.insert(messageOrder.end(), currentlyBroadcastedOrder.begin(), currentlyBroadcastedOrder.end());
	return messageOrder;
}

void MSS::UpdateResendPacket()
{
	Packet* newPacket = new Packet();
	while (resendPacket->getTotalLength() != resendPacket->getFrontOffset())
	{
		IntrusivePtr<const Chunk> msg = resendPacket->popAtFront();
		inet::IntrusivePtr<const AppS_h> m = dynamic_pointer_cast<const AppS_h>(msg);
		if (m && SBuffer.broadcastedMessagesContain(m->getSeq_S())) // message might have been acknowledged by all mhs since then and is thus not in SBuffer anymore
		{
			struct s_sendMsgMSS s_m = SBuffer.getBroadcastedMessage(m->getSeq_S());
			m = MessageFactory::createAppS_h(s_m.seqNC, id, s_m.MHdeliv, s_m.id);
			newPacket->insertAtBack(m);
		}
		else
			newPacket->insertAtBack(msg);
	}
	delete resendPacket;
	resendPacket = newPacket;
}

void MSS::resendMessageSBuffer(unsigned int id_h)
{
	vector<TimeoutAppS_h*> timeoutsToSchedule = SBuffer.resendBroadcastedMessages(id_h);
	for (TimeoutAppS_h* m : timeoutsToSchedule)
		scheduleMessageLoadDependantWithPartialProportionnality(m, m->getSeq(), SBuffer.getMinSeqNC(),
				SBuffer.getMaxSeqNC());
}

void MSS::ReceiveRegister(const IntrusivePtr<const Register> &m)
{
	if (MSSb.contains(m->getHi().MSSinCharge))
		Mobiles.registerHost(m->getHi());
}
