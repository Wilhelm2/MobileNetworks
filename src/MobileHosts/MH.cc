/*
 * MH.cc
 *
 *  Created on: May 7, 1619
 *      Author: dwilhelm
 */

#include "MH.h"

unsigned int MH::MHid = 0;

void MH::initialize(int stage)
{
	HostCommunications::initialize(stage);
	if (stage == INITSTAGE_LOCAL)
	{
		params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGE_NAME) + ".simParams").c_str()));
		BC = dynamic_cast<BroadcastController*>(getModuleByPath("Arbre.BCApp.app[0]"));
	}
}

void MH::handleCrashOperation(LifecycleOperation *operation)
{
	std::cerr << "NODE " << id_h << " FAILURE" << endl;
	RBuffer.clear();
	cancelEvent(&initTimeout);
	cancelEvent(&sendSeqTimeout);
	cancelEvent(selfMsg);
	cancelEvent(&updateCellTimer);
	for (cMessage* timeout : SBuffer.getTimeouts())
		cancelEvent(timeout);
	id_S = -1;
	state.moveToNewCell();
}

void MH::handleMessageWhenUp(cMessage *msg)
{
	if (msg->isSelfMessage())
		handleLocalMessage(msg);
	else
		handleReceivedMessage(dynamic_cast<Packet*>(msg));
}

void MH::updateCell()
{
	unsigned int newMobileSpportStation = computeClosestMSS(id_h, params->nbMobileSupportStations);
	if (newMobileSpportStation != id_S) // new station is closest -> connects to it
	{
		cancelEvent(&initTimeout);
		cancelEvent(&sendSeqTimeout);

		Ses++;
		id_S = newMobileSpportStation;
		state.moveToNewCell();
		RBuffer.switchBuffer();
		SendInit(newMobileSpportStation);
	}
	scheduleAt(simTime() + SimTime(100, SIMTIME_MS), &updateCellTimer);
}

// Schedules the message in a delay of 20ms +- 10ms
void MH::scheduleUrgentMessage(cMessage* m)
{
	scheduleAt(simTime() + params->checkPositif(params->getRandTime(SimTime(20, SIMTIME_MS))), m);
}

void MH::scheduleNextBroadcast()
{
	scheduleAt(simTime() + params->checkPositif(params->AppMFrequency + params->getRandTime(0)), selfMsg);
}

// Schedules message following the system's message load
// For a message load of 100msg/s returns 1s
// For a message load of 200msg/s returns 500ms
void MH::scheduleMessageLoadDependant(cMessage* m)
{
	double messageLoad = params->nbMobileHosts / params->AppMFrequency;
	simtime_t delay = SimTime(100. / messageLoad * 1000, SIMTIME_MS);
	scheduleAt(simTime() + params->checkPositif(params->getRandTime(delay)), m);
}

void MH::handleLocalMessage(cMessage* msg)
{
	if (msg == selfMsg)
		handleSelfMessage();
	else if (msg == &initTimeout)
		SendInit(id_S);
	else if (msg == &sendSeqTimeout)
		SendAck(msg);
	else if (msg == &updateCellTimer)
		updateCell();
	else
		Resend(msg);
}

void MH::handleSelfMessage()
{
	switch (selfMsg->getKind())
	{
		case START:
			processStart();
			break;
		case SEND:
			processSend();
			break;
		case STOP:
			processStop();
			break;
		default:
			;
	}
}

void MH::processStart()
{
	HostCommunications::processStart();
	scheduleAt(simTime() + 2 + (((float) id_h) / params->nbMobileHosts), &sendSeqTimeout);
	updateCell();
	cancelEvent(&initTimeout);
	scheduleUrgentMessage(&initTimeout);

	if (simTime() == 2) // beginning
		scheduleAt(simTime() + params->determineFirstSendTimeInMs(id_h) + 1, selfMsg);
	else
		scheduleNextBroadcast();
	selfMsg->setKind(SEND);
	for (const struct s_sendMsgMH& it : SBuffer.buffer)
		Resend(it.timeout);
}

void MH::processSend()
{
//    print(); std::cout << "MH " << id_h << " BROADCAST seq " << seqh <<endl;
	broadcastPacket();
	BC->hostNotifiesBroadcastMessage( { id_h, seqh });
	//deliver(id, seq); delivers when it receives the message from the station
	scheduleNextBroadcast();
	if (stopTime > 0 && selfMsg->getArrivalTime() > stopTime)
	{
		cancelEvent(selfMsg);
		selfMsg->setKind(STOP);
		scheduleAt(stopTime, selfMsg);
	}
	seqh++;
}

void MH::handleReceivedMessage(Packet* p)
{
	while (const inet::IntrusivePtr<const FieldsChunk> m = getNextMessage(p))
	{
		stats.receivedMessages++;
		if (isMessageFromCell(m))
		{
			if (state.isConnectionEstablished())
			{
				if (dynamic_pointer_cast<const AppS_h>(m) != nullptr)
					RecvAppS_h (dynamic_pointer_cast<const AppS_h>(m));else if(dynamic_pointer_cast<const Ack_S> (m)!= nullptr)
					ReceiveAck(dynamic_pointer_cast<const Ack_S>(m));
				}
				else
				{
					if(dynamic_pointer_cast<const InitAck> (m)!= nullptr)
					ReceiveInitAck(dynamic_pointer_cast<const InitAck>(m), p->getTag<L3AddressInd>()->getSrcAddress());
					else if(dynamic_pointer_cast<const AppCo> (m)!= nullptr)
					RecvAppCo(dynamic_pointer_cast<const AppCo>(m));
				}
			}
		}
		delete p;
	}

const inet::IntrusivePtr<const FieldsChunk> MH::getNextMessage(Packet* p)
{
	if (p->getDataLength() == B(0))
		return nullptr;
	else
		return dynamic_pointer_cast<const FieldsChunk>(p->popAtFront());
}

bool MH::isMessageFromCell(const inet::IntrusivePtr<const FieldsChunk>& m)
{
	const inet::IntrusivePtr<const StationWirelessMessage> msg = dynamic_pointer_cast<const StationWirelessMessage>(m);
	if (msg != nullptr)
	{
		if (msg->getCellId() == id_S)
			return true;
	}
	else
	{
		cerr << "RECEIVED UNKNOWN MESSAGE TYPE " << m << endl;
		exit(1);
	}
	return false;
}

void MH::SendAck(cMessage *msg)
{
	if (!state.isInitiatingConnection()) // required because host might change station in chooseStation called just before
	{
		stats.theoreticalNbWifiMessages++;
		if (!state.isConnectionEstablished()) // sends much more acknowledgments to initiate the connection faster
		{
			scheduleUrgentMessage(msg);
			sendPacket(new Packet("ack_h", MessageFactory::createAppCoAck(id_h, seqNC, Ses, id_S)), getMSSAdress(id_S));
		}
		else
		{
			scheduleMessageLoadDependant(msg);
			sendPacket(
					new Packet("ack_h",
							MessageFactory::createAck_h(id_h, RBuffer.getAckIntervals(seqNC), seqNC, Ses, id_S)),
					getMSSAdress(id_S));
		}
	}
}

void MH::ReceiveAck(const inet::IntrusivePtr<const Ack_S>& m)
{
	for (const idMsg& elt : m->getVSeq()) // vseq contains an ack for each session of which the station is in charge
	{
		if (elt.idMH == id_h)
		{
			vector<cMessage*> timeoutsToCancel = SBuffer.ackMessages(elt.seqMH);
			for (cMessage* m : timeoutsToCancel)
				cancelAndDelete(m);
			return;
		}
	}
}

void MH::SendInit(unsigned int target_S)
{
	state.moveToNewCell();
	cout << simTime() << "MH " << id_h << " connects with Ses " << Ses << " to station " << target_S << endl;
	sendPacket(new Packet("Init", MessageFactory::createInit(id_h, seqNC, Ses, Ses_LC, target_S, state.getState())),
			getMSSAdress(id_S));
	stats.theoreticalNbWifiMessages++;
	scheduleUrgentMessage(&initTimeout);
}

void MH::ReceiveInitAck(const inet::IntrusivePtr<const InitAck>& m, L3Address addr)
{
	if (m->getId_h() == id_h && m->getSes() == Ses)
	{
		initiateConnection(m->getSes(), m->getSeq_S(), m->getMsgOrder());
		state.setConnectionToEstablished();

		vector<cMessage*> timeoutsToCancel = SBuffer.ackMessages(m->getSeq_h());
		for (cMessage* m : timeoutsToCancel)
			cancelAndDelete(m);
		for (const struct s_sendMsgMH& m : SBuffer.buffer)
		{
			sendPacket(new Packet("Apph_S", MessageFactory::createApph_S( { id_h, m.seqh }, id_S, Ses)),
					getMSSAdress(id_S));
			stats.theoreticalNbWifiMessages++;
		}
	}
}

void MH::initiateConnection(unsigned int Ses, unsigned int seqNC, vector<tuple<idMsg, unsigned int>> updatedSeqS)
{
	this->seqNC = seqNC;
	this->Ses_LC = Ses;

	RBuffer.updateAllSeqS(updatedSeqS);
	RBuffer.erase(seqNC);
	FIFODeliver();

	if (!sendSeqTimeout.isScheduled())
		scheduleUrgentMessage(&sendSeqTimeout);
	cancelEvent(&initTimeout);
}

void MH::broadcastPacket()
{
	idMsg idM = { id_h, seqh };
	if (!state.isInitiatingConnection())
	{
		sendPacket(new Packet("Apph_S", MessageFactory::createApph_S(idM, id_S, Ses)), getMSSAdress(id_S));
		stats.theoreticalNbWifiMessages++;
	}
	stats.broadcastedMessagesSinceLastMeasure++;
	struct s_sendMsgMH m = { seqh, MessageFactory::createTimeoutApph_S(seqh) };
	SBuffer.push_back(m);
	scheduleMessageLoadDependant(m.timeout);
}

void MH::RecvAppS_h(const inet::IntrusivePtr<const AppS_h>& msg)
{
	struct s_recvMsgMH mt = { msg->getSeq_S(), msg->getId(), (find(msg->getM_d().begin(), msg->getM_d().end(), id_h)
			!= msg->getM_d().end()) // checks if the mobile has already delivered the message
			};
	handleAppMsg(mt);
}

void MH::handleAppMsg(struct s_recvMsgMH msg)
{
	if (msg.seqC > seqNC + 1)
		RBuffer.insertMsg(msg);
	else if (msg.seqC == seqNC + 1)
	{
		handleDeliverableMessage(msg.id, msg.hasAlreadyDelivered);
		FIFODeliver();
	}
}

void MH::FIFODeliver()
{
	struct s_recvMsgMH messageToDeliver;
	unsigned int cpt = 0; // count number of deliveries to directly send an ACK in case of many deliveries
	while (RBuffer.isDeliverableMessage(seqNC))
	{
		messageToDeliver = RBuffer.popFront();
		handleDeliverableMessage(messageToDeliver.id, messageToDeliver.hasAlreadyDelivered);
		cpt++;
	}
	if (cpt > 50 && sendSeqTimeout.isScheduled()) // delivered many messages and is currently sending ACK messages (i.e., not in handoff)
	{
		sendPacket(
				new Packet("ack_h",
						MessageFactory::createAck_h(id_h, RBuffer.getAckIntervals(seqNC), seqNC, Ses, id_S)),
				getMSSAdress(id_S));
		stats.theoreticalNbWifiMessages++;
	}
}

void MH::handleDeliverableMessage(idMsg idMessage, bool hasAlreadyDelivered)
{
	seqNC++;
	if (!hasAlreadyDelivered)
	{
		BC->deliverMessage(idMessage, id_h);
		stats.totalDelivered++;
	}
}

void MH::RecvAppCo(const inet::IntrusivePtr<const AppCo>& msg)
{
	if (msg->getTargetHostId() == id_h && msg->getSes() == Ses)
	{
		if (state.isInitiatingConnection())
		{
			initiateConnection(msg->getSes(), 0, msg->getMsgOrder());
			state.setConnectionToConnecting();
		}
		handleAppMsg( { msg->getSeq_S(), msg->getId(), false });
	}
}

void MH::Resend(cMessage *msg)
{
	TimeoutApph_S* m = dynamic_cast<TimeoutApph_S*>(msg);
	struct s_sendMsgMH elt = SBuffer.searchMsg(m->getSeq());
	if (!state.isInitiatingConnection())
		sendPacket(new Packet("Apph_S", MessageFactory::createApph_S( { id_h, elt.seqh }, id_S, Ses)),
				getMSSAdress(id_S));
	scheduleMessageLoadDependant(m);
}

