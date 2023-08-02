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

#include "BroadcastController.h"

BroadcastController::BroadcastController()
{
}

BroadcastController::~BroadcastController()
{
}

void BroadcastController::initialize(int stage)
{
	cSimpleModule::initialize(stage);
	scheduleAt(0, new cMessage());
}

void BroadcastController::handleMessage(cMessage *msg)
{
	delete msg;
	params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGE_NAME) + ".simParams").c_str()));
	HostsClock.resize(params->nbMobileHosts, vector<unsigned int>(params->nbMobileHosts, 0));
	MSSClock.resize(params->nbMobileSupportStations, vector<unsigned int>(params->nbMobileHosts, 0));
	messages.resize(params->nbMobileHosts);
}

// Called by station when it receives an Init message from a host which joins the system
void BroadcastController::HostSystemJoin(unsigned int idMH, unsigned int idMSS)
{
	std::cout << "Host " << idMH << "joins the system  by connecting to station " << idMSS << endl;
	unsigned int localSeq = HostsClock[idMH][idMH]; // to save the case where idMH broadcasted a message prior to connecting to the first station
	HostsClock[idMH] = MSSClock[idMSS];
	HostsClock[idMH][idMH] = localSeq;
}

// Called by host messageID.idMH to notify the control of it's broadcast of message messageID
void BroadcastController::hostNotifiesBroadcastMessage(idMsg messageID)
{
	HostsClock[messageID.idMH][messageID.idMH]++; // increment the clock of the sending process
	msgControl t = { messageID.seqMH, HostsClock[messageID.idMH], 0, vector<bool>(params->nbMobileHosts, false),
			simTime() };
	messages[messageID.idMH].push_back(t);
}

// Called by host idDest when it delivers message id.
// The function controls that the host can deliver id in causal order.
// Moreover, it updates the control structures and deletes the message if it becomes obsolete
void BroadcastController::deliverMessage(idMsg id, unsigned int idDest)
{
	msgControl& msg = searchMessage(messages[id.idMH], id.seqMH);

	// Checks if the host can effectively deliver the message
	if (!satisfiesDeliveryConditions(msg.clock, HostsClock[idDest], id.idMH))
		DeliveryError("Out of causal delivery ", id, idDest, msg.clock, HostsClock[idDest]);

	// Checks if the host has already delivered the message
	if (msg.hasDelivered[idDest])
		DeliveryError("Host has already delivered ", id, idDest, msg.clock, HostsClock[idDest]);

	// Updates the msgControl structure
	if (idDest != id.idMH) // does not increment its own entry, since did it when broadcasting the message
		HostsClock[idDest][id.idMH]++;
	msg.nbDeliv++;
	msg.hasDelivered[idDest] = true;
	totalDelivered++;
	totalTimeDelivered += simTime() - msg.sendTime;

	// delete the message once it was received by all hosts and stations
	if (msg.nbDeliv == (params->nbMobileHosts + params->nbMobileSupportStations))
		deleteMessage(id);
}

// Searches a message following its sequence number seq among the messages broadcasted by a given host
msgControl& BroadcastController::searchMessage(vector<msgControl>& messages, unsigned int seq)
{
	for (msgControl& m : messages)
	{
		if (m.seq == seq)
			return m;
	}
	assert("Message not registered");
	throw 0;
}

// Verifies that a message's clock satisfies its delivery conditions
bool BroadcastController::satisfiesDeliveryConditions(const vector<unsigned int>& messageClock,
		const vector<unsigned int>& processClock, unsigned int idSource)
{
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		if (!entrySatisfiesDeliveryCondition(i, idSource, messageClock[i], processClock[i]))
			return false;
	}
	return true;
}

// Verifies that a given clock entry satisfies a message's delivery conditions
bool BroadcastController::entrySatisfiesDeliveryCondition(unsigned int index, unsigned int idSource,
		unsigned int dependencyValue, unsigned int value)
{
	return dependencyValue == 0 || (value >= (dependencyValue - (index == idSource))); // optimization
//    return (localVectorEntry >= toDeliverEntry && index != idSource) || (localVectorEntry >= (toDeliverEntry-1) && index == idSource);
}

// Deletes an obsolete message (ie once it was acknowledged by all hosts and stations)
void BroadcastController::deleteMessage(idMsg id)
{
//    std::cout << "BROADCASTCONTROLLER: DELETE MESSAGE (" << idSrc <<"," <<seq<<")"<<endl;
	for (vector<msgControl>::iterator it = messages[id.idMH].begin(); it != messages[id.idMH].end(); ++it)
		if (it->seq == id.seqMH)
		{
			messages[id.idMH].erase(it);
			return;
		}
}

// Prints a delivery error and stops the program
void BroadcastController::DeliveryError(string errorReason, idMsg idM, unsigned int destProcess,
		const vector<unsigned int>& messageClock, const vector<unsigned int>& processClock)
{
	cerr << "BroadcastController " << errorReason << " idMsg " << idM.idMH << "," << idM.seqMH << " destination "
			<< destProcess << endl;
	cerr << "Process clock: ";
	printClock(processClock);
	cerr << "Message clock: ";
	printClock(messageClock);
	exit(1);
}

void BroadcastController::printClock(const vector<unsigned int>& v)
{
	for (unsigned int i : v)
		std::cout << i << " ";
	std::cout << endl;
}

// Called by a station whenever it removes a message from its SBuffer, ie when all hosts connected to it delivered the message
void BroadcastController::stationRemovesMessage(idMsg id, unsigned int idMSS)
{
	msgControl& msg = searchMessage(messages[id.idMH], id.seqMH);
	for (unsigned int i = 0; i < msg.clock.size(); i++)
		MSSClock[idMSS][i] = max(MSSClock[idMSS][i], msg.clock[i]);
	msg.nbDeliv++;
	if (msg.nbDeliv == (params->nbMobileHosts + params->nbMobileSupportStations))
		deleteMessage(id);
}
