#ifndef MSS_H_
#define MSS_H_

#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <bits/stdc++.h>

#include "../MessageFactory.h"
#include "../BroadcastController.h"
#include "../commun_f.h"
#include "Communications.h"
#include "MobileHostsMap.h"

using namespace omnetpp;

#include "inet/applications/udpapp/UdpBasicApp.h"
#include "inet/common/packet/Packet.h"
#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/applications/common/SocketTag_m.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include "OpenSocketMap.h"
#include "inet/common/lifecycle/ILifecycle.h"
#include "inet/common/lifecycle/LifecycleOperation.h"
#include "inet/common/packet/chunk/Chunk.h"
#include "inet/common/packet/chunk/SliceChunk.h"
#include "inet/common/packet/ReassemblyBuffer.h"
#include <random>
using namespace inet;
using namespace std;

#include "MSSSBuffer.h"
#include "Session.h"
#include "../MobileHosts/MHState.h"

// Contains the causal broadcast algorithm for mobile support stations
class MSS: public Communications
{
public:
	virtual int numInitStages() const override
	{
		return NUM_INIT_STAGES;
	}
	;
	virtual void initialize(int stage);
	// Handles received messages (internal and external) when up
	virtual void handleMessageWhenUp(cMessage *msg) override;
	// Handles messages that the station sent to itself (timeouts)
	void handleSelfMessage(cMessage* msg);
	// Handles messages that the station received from other nodes (stations and hosts)
	void handleReceivedMessage(cMessage* msg);
	// Handles timeouts of application or handoff messages
	void handleTimeout(cMessage* msg);

	// Called when the station starts
	virtual void handleStartOperation(LifecycleOperation *operation) override;
	// Called when the station stops operating
	virtual void handleStopOperation(LifecycleOperation *operation) override;
	// Called when the station crashes
	virtual void handleCrashOperation(LifecycleOperation *operation) override;
	// Handles received UDP packets
	virtual void socketDataArrived(UdpSocket *socket, Packet *packet) override;
	// Handles received TCP packets
	void routing(const IntrusivePtr<const FieldsChunk> m, Packet * sourcePacket);

	// Removes the messages of the SBuffer whose sequence number are lower or equal to seq
	void clearSBuffer(unsigned int seq);
	void print();
	// Sends a message on the wireless interface
	template<typename T> void sendMessage(IntrusivePtr<T> msg);
	// Searches the minimum sequence number of the session registered at the station
	unsigned int searchMinSeq();
	// Sends the message of minimal sequence number that the host connected to the station have not delivered yet
	void sendMinMessages();
	// Schedules an urgent message to be sent in 20ms
	void scheduleUrgentMessage(cMessage* m);
	// Schedules a message depending on the system's message load
	// For a message load of 100msg/s returns 500ms
	// For a message load of 200msg/s returns 250ms
	void scheduleMessageLoadDependant(cMessage* m);
	// Schedules a message depending on the system's message load, with
	// Messages with a low sequence number being resent faster
	void scheduleMessageLoadDependantWithPartialProportionnality(cMessage* m, unsigned int seq, unsigned int min,
			unsigned max);
	// Cancels and deletes the timeouts given as parameter
	void cancelAndDeleteTimeouts(vector<cMessage*> timeouts);
	// Cancels and deletes the timeouts of the message list given as parameter
	void cancelAndDeleteTimeouts(list<struct s_sendMsgMSS> messages);

	/* Message acknowledgments */

	// Sends an acknowledge message on the wireless network to acknowledge messages to connected hosts
	void SendACK(cMessage *msg);
	// Handles an acknowledge message from a host
	void ReceiveACK(const IntrusivePtr<const Ack_h> &m);
	// Handles an acknowledge message from another station
	void ReceiveAckSS(const IntrusivePtr<const AckSS>& m);
	// Receives an acknowledge message for AppCo messages from a connected host
	void ReceiveAppCoACK(const IntrusivePtr<const AppCoAck>& m);
	// Checks whether broadcasted messages have been delivered by all connected hosts and can be stopped being broadcasted
	void checkStopSendSBufferMessages(const vector<AckInterval>& ackIntervals);

	/* Host connection */

	// Handles init messages from hosts
	void ReceiveInit(const IntrusivePtr<const Init> &m);
	// Handles init message for an existing connection
	void handleExistingConnection(unsigned int id_h, unsigned int Ses);
	// Handles init message for a new connection
	void handleNewConnection(unsigned int id_h, const IntrusivePtr<const Init> &m);
	// Registers a new connection and begins the handoff of it
	void RegisterHost(unsigned int id_h, const IntrusivePtr<const Init>& m);
	// Finalizes the initialization of a new connection
	void FinalizeInit(unsigned int id_h, unsigned int Session);
	// Sends a delete message to other stations on the wired network
	void SendDelete(unsigned int id_h, unsigned int Ses);
	// Handles received delete messages
	void ReceiveDelete(const IntrusivePtr<const Delete> &m);

	/* Application messages */

	// Handles received application messages from hosts
	void ReceiveApph_S(const IntrusivePtr<const Apph_S>& m);
	// Disseminates FIFO ordered application messages in FIFO order
	void FIFODisseminate(unsigned int id_h, unsigned int Ses);
	// Disseminates a FIFO ordered application message and handles it
	void Disseminate(idMsg m, unsigned int Ses);
	// Handles a FIFO ordered application message
	void handleFIFOOrderedMessage(idMsg idM);
	// Broadcasts an application message on the wireless network
	void broadcastInCell(idMsg idM, unsigned int seqS);
	// Handles an application message received from another station
	void ReceiveAppS_S(const IntrusivePtr<const AppS_S>& m, Packet* sourcePacket);
	// Handles the reception of an App_sresp message
	void ReceiveApp_sresp(const IntrusivePtr<const App_resp>& m, Packet* sourcePacket);

	/* Station recovery */

	// Handles a received station recovery request
	void ReceiveRecoverMSSReq(const IntrusivePtr<const RecoverMSSReq> &m);
	// Handles a received recovery message - initializes the station, sets it to the up state and resumes normal execution
	void ReceiveRecoverMSS(const IntrusivePtr<const RecoverMSS> &m);
	// Manually triggers the reconnection of hosts
	void manualHostReconnectAfterFailure();

	/* Handoff */

	// Sends a Req1 message
	void SendReq_1(unsigned int seq_S, unsigned int id_h, unsigned int Ses_LC, unsigned int Ses);
	// Receives a Req1 message
	void ReceiveReq_1(const IntrusivePtr<const Req1>& m);
	// Builds the vector of message ids that the host has not delivered
	vector<idMsg> buildVectorIdsNotDeliveredMessages(const Session& session);
	// REceives a Rsp1 message
	void ReceiveRsp_1(const IntrusivePtr<const Rsp1>& m);
	// Receives a Req2 message
	void ReceiveReq_2(const IntrusivePtr<const Req2>& m);
	// Verifies that the station buffers all requested messages
	void assertContainsRequestedMessages(const vector<idMsg>& requestedMessages,
			const StationAppCoSendingBuffer& AppCoMsgToDeliver);
	// Receives a Rsp2 message
	void ReceiveRsp_2(const IntrusivePtr<const Rsp2>& m);
	// Builds the AppCo messages to be sent to the host
	list<struct s_sendMsgMSS> buildAppCoMsg(const vector<idMsg>& alreadyAcknowledgedMessages,
			const vector<idMsg>& alreadyDeletedMessages, unsigned int id_h, unsigned int Ses);
	// Sends AppCo messages to the host
	void sendAppCoMsg(const list<struct s_sendMsgMSS>& appCoMsg, unsigned int id_h, unsigned int Ses);
	// Builds a list of message ids and the sequence number the station associates to them in
	// order to send it to the connecting host
	vector<tuple<idMsg, unsigned int>> buildMessageOrderList(const list<struct s_sendMsgMSS>& appCoMsg);
	// Updates messages to be broadcasted. Called after the connection of a host
	void UpdateResendPacket();
	// Resends messages of the SBuffer that id_h did not deliver yet provided that their sequence number is higher
	// than the sequence number of the lowest message still broadcasted by the station
	void resendMessageSBuffer(unsigned int id_h);
	// Handles a register message
	void ReceiveRegister(const IntrusivePtr<const Register> &m);

	// Contains the sessions structures
	MobileHostsMap Mobiles;
	// The station's sending buffer containing pending messages and currently broadcasted messages
	MSSSBuffer SBuffer;

	BroadcastController *BC;
	// Timeout to send acknowledge messages
	cMessage sendSeqTimeout;
	// Timeout to call the sendMinMessages() function
	cMessage sendMinMsg;

	// Set to true when recovering and false otherwise
	// Till the end of recovery ignores messages other than the RecoverMSS message
	bool recovering = false;
	// Reference to simulation parameters to avoid looking it up all the time
	SimulationParameters* params;
};

Define_Module(MSS);

#endif /* MSS_H_ */

