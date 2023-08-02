//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef COMMUNICATIONS_H_
#define COMMUNICATIONS_H_

#include <string.h>
#include <sstream>
#include <omnetpp.h>
#include <fstream>

#include "inet/common/packet/Packet.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"
#include "inet/applications/base/ApplicationBase.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/common/packet/chunk/SliceChunk.h"
#include "inet/common/packet/ReassemblyBuffer.h"

#include "inet/common/INETDefs.h"

#include "OpenSocketMap.h"
#include "../commun_f.h"
#include "../simulationParameters.h"
#include "../MessageFactory.h"

using namespace omnetpp;
using namespace inet;
using namespace std;

struct statsMSS
{
	unsigned int effectiveNbWifiMessages = 0;
	unsigned int totalSizeWifiMessages = 0;
	unsigned int theoreticalNbWifiMessages = 0; // par ex lorsque group 2 messages dans un send, ce sera bien considéré comme 2 sends ici
	unsigned int theoreticalNbWiredMessages = 0;
	unsigned int totalExpectedReceive = 0;
	unsigned int totalBufferCoMessages = 0;
	unsigned int receivedMessages = 0; // tous les messages reçus, même ceux jetés
};

class Communications: public ApplicationBase, public UdpSocket::ICallback, public TcpSocket::ICallback
{
public:
	Communications();
	virtual ~Communications();

	virtual void initialize(int stage) override;
	// Initializes the sockets as well as the wired network overlay
	void setSockets();
	// Creates a wired network overlay in form of a tree with a depth of 3
	void createTreeOverlay();
	// Creates a wired network overlay from a file
	void createOverlayFromFile();
	// Creates a TCP connection with the station idMSS
	void createTCPMSSCo(unsigned int idMSS);

	virtual void handleStartOperation(LifecycleOperation *operation) override;
	virtual void handleCrashOperation(LifecycleOperation *operation) override;

	/** @name TcpSocket::ICallback callback methods */
	virtual void socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo) override;
	virtual void socketClosed(TcpSocket *socket) override;
	virtual void socketDeleted(TcpSocket *socket) override;
	virtual void socketEstablished(TcpSocket *socket) override
	{
	}
	virtual void socketPeerClosed(TcpSocket *socket) override
	{
	}
	virtual void socketFailure(TcpSocket *socket, int code) override
	{
	}
	virtual void socketStatusArrived(TcpSocket *socket, TcpStatusInfo *status) override
	{
	}
	// Called upon reception of a packet. Messages can be split in several packets and packets may contain several messages.
	// Hence, messages must first be extracted from packets.
	void socketDataArrived(TcpSocket* socket, Packet *packet, bool urgent);
	// Processes that routes a message received on the wired network
	void handleReceivedMessage(IntrusivePtr<const FieldsChunk> m, Packet* packet);
	// Updates the FIFOorder structure to ensure that each message is processed exactly once
	void update(const IntrusivePtr<const FieldsChunk>& m, unsigned int sourceSeq, unsigned int sourceId);
	// Function implemented in MSS.cc and which processes the messages received on the wired network
	virtual void routing(const IntrusivePtr<const FieldsChunk> m, Packet * sourcePacket) = 0;

	/** @name UdpSocket::ICallback methods */
	virtual void socketErrorArrived(UdpSocket *socket, Indication *indication) override;
	virtual void socketClosed(UdpSocket *socket) override
	{
	}

	// Sends a message on the wifi interface
	void sendToWifiInterface(Packet *packet);
	// Sends a message on all outgoing wired channels
	void sendToStations(const IntrusivePtr<const FieldsChunk>& m);
	// Try to get the L3Address with the module's address
	L3Address trysolveaddr(const char* address);

	// Contains the interface id to avoid looking it up at each message sent on the wireless network
	unsigned int wlanInterfaceId;
	// Gets incoming TCP connections
	TcpSocket socketCoMSS;
	// Map containing TCP connections with other stations
	OpenSocketMap MSSsocketMap;
	// Socket to get messages from the wireless network from mobile hosts
	UdpSocket socketMH;
	// Local port at which listen for incoming TCP connections
	int localPort;

	// Counter used to assign unique ids to stations
	static unsigned int idMSSIdentifier;
	// The station's unique id
	unsigned int id = idMSSIdentifier++;
	// Sequence number used to process  messages sent over the wired network are processed exactly once and in FIFO order
	unsigned int seqMsg = 0;
	// Structure to ensure that messages sent over the wired network are processed exactly once and in FIFO order
	vector<unsigned int> FIFOorder;
	// Used to reassemble messages received on the wired network (since messages may be split in several packets)
	map<unsigned int, ReassemblyBuffer*> ReassembleBuffers;

	// Records the communication stats of the station
	struct statsMSS stats;

	// Messages are sent in packets of MSGPERPACKET messages
	const unsigned int MSGPERPACKET = 5;
	// Messages are added to resendPacket till it contains MSGPERPACKET messages, then resendPacket is broadcasted
	Packet* resendPacket;
	// Counts the number of messages in resendPacket
	unsigned int resendcpt = 0;

	// To access the simulation parameters
	SimulationParameters* params;

	// Structure to track the failure of stations of station groups
	StationGroup MSSb;
};

#endif /* COMMUNICATIONS_H_ */
