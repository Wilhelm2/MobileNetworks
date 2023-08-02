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

#include "Communications.h"

unsigned int Communications::idMSSIdentifier = 0;

Communications::Communications()
{
}

Communications::~Communications()
{
}

void Communications::initialize(int stage)
{
	ApplicationBase::initialize(stage);

	if (stage == INITSTAGE_APPLICATION_LAYER)
	{
		setSockets();

		params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGE_NAME) + ".simParams").c_str()));
	}
	else if (stage == INITSTAGE_LAST)
	{
		InterfaceTable* t = dynamic_cast<InterfaceTable*>(getModuleByPath(par("interfaceTableModule")));
		wlanInterfaceId = t->getInterfaceByName("wlan0")->getInterfaceId();
	}
}

void Communications::setSockets()
{
	localPort = par("localPort");
	socketCoMSS.setOutputGate(gate("socketOut"));
	socketCoMSS.bind(L3Address(), localPort + 1);
	socketCoMSS.listen();
	socketCoMSS.setCallback(this);

	socketMH.setOutputGate(gate("socketOut"));
	socketMH.bind(localPort);
	socketMH.setCallback(this);

	createOverlayFromFile();
}

void Communications::createTreeOverlay()
{
	vector<unsigned int> currlevel;
	currlevel.push_back(0);
	vector<unsigned int> nextlevel;
	unsigned int pos = 0;
	while (pos < params->nbMobileSupportStations)
	{
		for (unsigned int k = 0; k < currlevel.size(); k++)
		{
			for (unsigned int i = 1; i < 4; i++) // Tree of depth 3
			{
				if (currlevel[k] == id && pos + i < params->nbMobileSupportStations) // current MSS + is not a leaf
					createTCPMSSCo(pos + i);
				nextlevel.push_back(pos + i);
			}
			pos += 3;
			if (currlevel[k] == id && k < currlevel.size() - 1) // not last element of that level
				createTCPMSSCo(currlevel[k + 1]);
		}
		currlevel = nextlevel;
		nextlevel.clear();
	}
	for (unsigned int i = 0; i < currlevel.size(); i++)
	{
		if (currlevel[i] == id)
			createTCPMSSCo(currlevel[i + 1]);
	}
}

void Communications::createOverlayFromFile()
{
	std::ifstream infile("../../../simulationInitilization/connectionToEstablish.txt");
	std::string line;
	unsigned int n;
	for (unsigned int i = 0; i <= id; i++)
		std::getline(infile, line);
	stringstream stream(line);
	while (stream >> n)
		createTCPMSSCo(n);
}

void Communications::createTCPMSSCo(unsigned int idMSS)
{
	TcpSocket *newSocket = new TcpSocket();
	newSocket->setOutputGate(gate("socketOut"));
	newSocket->setCallback(this);
	MSSsocketMap.addSocket(newSocket);
	newSocket->connect(trysolveaddr(("Stations[" + to_string(idMSS) + "]%eth0").c_str()), localPort + 1);
}

void Communications::handleStartOperation(LifecycleOperation *operation)
{
	resendPacket = new Packet();
	if (simTime() > 2) // after crash
	{
		socketCoMSS.renewSocket();
		setSockets();
	}
}

void Communications::handleCrashOperation(LifecycleOperation *operation)
{
	for (auto e : MSSsocketMap.socketMap)
		e.second->close();
	MSSsocketMap.deleteSockets();
	delete resendPacket;
}

void Communications::socketAvailable(TcpSocket *socket, TcpAvailableInfo *availableInfo)
{
	TcpSocket *newSocket = new TcpSocket(availableInfo);
	newSocket->setOutputGate(gate("socketOut"));
	newSocket->setCallback(this);
	if (availableInfo->getLocalPort() == (localPort + 1))
		MSSsocketMap.addSocket(newSocket);
	socket->accept(availableInfo->getNewSocketId());
}

void Communications::socketClosed(TcpSocket *socket)
{
	MSSsocketMap.removeSocket(socket);
}

void Communications::socketDeleted(TcpSocket *socket)
{
	// called by the station which calls socket.delete()
	cerr << simTime() << " MSS " << id << " SOCKET DELETED " << socket << endl;
	MSSsocketMap.removeSocket(socket);
}

void Communications::socketErrorArrived(UdpSocket *socket, Indication *indication)
{
	EV_WARN << "Ignoring UDP error report " << indication->getName() << endl;
	delete indication;
}

void Communications::socketDataArrived(TcpSocket* socket, Packet *packet, bool urgent)
{
	while (true)
	{
		if (packet->getTotalLength() == packet->getFrontOffset())
			break;
		if (dynamic_pointer_cast<const SliceChunk>(packet->peekAtFront()) != nullptr)
		{
			auto msg = dynamic_pointer_cast<const SliceChunk>(packet->popAtFront());
			if (ReassembleBuffers.find(socket->getSocketId()) == ReassembleBuffers.end()) // nouveau packet en plusieurs chunks
				ReassembleBuffers[socket->getSocketId()] = new ReassemblyBuffer(msg->getChunk()->getChunkLength());
			ReassembleBuffers[socket->getSocketId()]->replace(msg->getOffset(), msg); // ajoute le chunk au buffer
			if (ReassembleBuffers[socket->getSocketId()]->isComplete())
			{
				handleReceivedMessage(
						dynamic_pointer_cast<const FieldsChunk>(
								ReassembleBuffers[socket->getSocketId()]->getReassembledData()), packet);
				ReassembleBuffers.erase(socket->getSocketId());
			}
		}
		else
		{
			handleReceivedMessage(dynamic_pointer_cast<const FieldsChunk>(packet->popAtFront()), packet);
		}
	}
	delete packet;
}

void Communications::handleReceivedMessage(IntrusivePtr<const FieldsChunk> m, Packet* packet)
{
	const IntrusivePtr<const FIFOCom> msg = dynamic_pointer_cast<const FIFOCom>(m);
	if (msg == nullptr)
	{
		cerr << "Wrong message type: not FIFOCom message" << endl;
		exit(0);
	}
	if (msg->getSourceSeq() > FIFOorder[msg->getSourceId()])
	{
		sendToStations(m);
		routing(m, packet);
	}
	update(m, msg->getSourceSeq(), msg->getSourceId());
}

void Communications::update(const IntrusivePtr<const FieldsChunk>& m, unsigned int sourceSeq, unsigned int sourceId)
{
	if (sourceSeq > FIFOorder[sourceId])
	{
		assert(sourceSeq == (FIFOorder[sourceId] + 1));
		FIFOorder[sourceId]++;
		stats.receivedMessages++;
	}
	MSSb.updateLastMsgMSSb(sourceId);
}

void Communications::sendToWifiInterface(Packet *packet)
{
	packet->addTag<InterfaceReq>()->setInterfaceId(wlanInterfaceId);
	stats.effectiveNbWifiMessages++;
	stats.totalSizeWifiMessages += packet->getByteLength() + 48; // +48 : UDP + MAC + IPv4
	socketMH.sendTo(packet, Ipv4Address::ALLONES_ADDRESS, localPort);
}

void Communications::sendToStations(const IntrusivePtr<const FieldsChunk>& m)
{
	Packet *p;
	for (auto connection : MSSsocketMap)
	{
		p = new Packet("", m);
		connection.second->send(p);
		stats.theoreticalNbWiredMessages++;
	}
	FIFOorder[id] = seqMsg;
}

L3Address Communications::trysolveaddr(const char* address)
{
	L3Address result;
	L3AddressResolver().tryResolve(address, result);
	if (result.isUnspecified())
		EV_ERROR << "cannot resolve destination address : " << address << endl;
	return result;
}

