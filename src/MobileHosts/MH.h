#ifndef MH_H_
#define MH_H_

#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <cstdlib>
#include <random>
#include <algorithm>

#include "inet/applications/udpapp/UdpBasicApp.h"
#include "inet/common/packet/Packet.h"
#include "inet/applications/base/ApplicationPacket_m.h"
#include "inet/common/TimeTag_m.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/linklayer/common/MacAddressTag_m.h"

#include "inet/networklayer/ipv4/Ipv4InterfaceData.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/mobility/static/StaticGridMobility.h"
#include "inet/mobility/single/LinearMobility.h"
#include "inet/networklayer/common/L3AddressTag_m.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/linklayer/common/InterfaceTag_m.h"

#include "../MessageFactory.h"
#include "../commun_f.h"
#include "../BroadcastController.h"
#include "LinearMobilityInsideCells.h"
#include "../simulationParameters.h"
#include "MHRBuffer.h"
#include "MHSBuffer.h"
#include "MHState.h"

#include "HostCommunications.h"

using namespace omnetpp;
using namespace inet;
using namespace std;

// Class that contains the main part of hosts.
// It implements the connection of hosts to stations as well as the sending, reception, delivering and acknowledgment of application messages.
class MH: public HostCommunications
{
public:
	virtual void initialize(int stage);
	virtual void handleCrashOperation(LifecycleOperation *operation) override;
	virtual void handleMessageWhenUp(cMessage *msg) override;

	// Checks every 100ms to connect to the closest station. Initiates a new connection whenever changes the station to which is the closest
	void updateCell();

	// Schedules an urgent message. Urgent messages are resent in 20ms
	void scheduleUrgentMessage(cMessage* m);
	// Planifies the next application message broadcast
	void scheduleNextBroadcast();
	// Schedules a message depending on the system's message load
	void scheduleMessageLoadDependant(cMessage* m);

	void handleLocalMessage(cMessage* msg);
	void handleSelfMessage();
	// Initializes the host. The host is initialized her and not in constructor because it requires that INET is initialized
	void processStart();
	// Sends an application message
	virtual void processSend() override;
	// Handles messages received from the station to which the host is connected
	void handleReceivedMessage(Packet *p);
	// Gets the next message from the received packe
	const inet::IntrusivePtr<const FieldsChunk> getNextMessage(Packet* p);
	// Checks if the message is from the cell to which the host is connected
	bool isMessageFromCell(const inet::IntrusivePtr<const FieldsChunk>& m);

	// Sends an acknowledge message
	void SendAck(cMessage *msg);
	// Handles a received acknowledge message
	void ReceiveAck(const inet::IntrusivePtr<const Ack_S>& msg);

	// Sends an Init message
	void SendInit(unsigned int target_S);
	// Handles received init messages
	void ReceiveInitAck(const inet::IntrusivePtr<const InitAck>& m, L3Address addr);
	// Initiates a connection
	void initiateConnection(unsigned int Ses, unsigned int seqNC, vector<tuple<idMsg, unsigned int>> updatedSeqS);

	// Broadcast an application message
	void broadcastPacket();
	// Handles the reception of application messages
	void RecvAppS_h(const inet::IntrusivePtr<const AppS_h>& msg);
	void handleAppMsg(struct s_recvMsgMH msg);
	// Delivers application messages in FIFO order
	void FIFODeliver();
	// Delivers an application message at the condition that it is not already delivered by the host
	void handleDeliverableMessage(idMsg idMessage, bool hasAlreadyDelivered);
	// Handles the reception of AppCo messages
	void RecvAppCo(const inet::IntrusivePtr<const AppCo>& msg);

	// Handles timeouts of broadcast messages
	void Resend(cMessage * msg);
public:
	// Used to assign unique ids to hosts
	static unsigned int MHid;

	// Station to which the host is connected
	unsigned int id_S = -1;
	// The hosts id
	unsigned int id_h = MHid++;
	// Sequence number of the next message the host will broadcast
	unsigned int seqh = 1;
	// Messages that the host has delivered are those $m$, m.seq <= seqNC
	unsigned int seqNC = 0;
	// Contains the host's state
	MHState state;
	// Session counter. The host increments it whenever it connects to a new station
	unsigned int Ses = 0;
	// The session counter of the most recent session in which the host received a confirmation that the station received its Init message
	// ie, the most recent connection in which the host received an InitAck or AppCo message from the station to which it connected itself
	unsigned int Ses_LC = -1;

	// Timeout for Init messages
	cMessage initTimeout;
	// Timeout for sending acknowledge messages
	cMessage sendSeqTimeout;
	// Timeout to check if is closer to a new station
	cMessage updateCellTimer;
	// Controller used to ensure that hosts deliver application messages in causal order
	BroadcastController *BC;

	// The host's reception buffer
	MHRBuffer RBuffer;
	// The host's sending buffer
	MHSBuffer SBuffer;
};

Define_Module(MH);
#endif /* MH_H_ */
