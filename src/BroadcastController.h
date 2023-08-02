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

#ifndef BROADCASTCONTROLLER_H_
#define BROADCASTCONTROLLER_H_

#include <string.h>
#include <omnetpp.h>
#include <vector>
#include <stdexcept>
#include "inet/applications/udpapp/UdpBasicApp.h"

#include "simulationParameters.h"
#include "commun_f.h"
using namespace inet;
using namespace std;

typedef struct s_msg
{
	unsigned int seq; // sequence number of message given by mh
	vector<unsigned int> clock; // message clock
	unsigned int nbDeliv; // done deliveries
	vector<bool> hasDelivered; // array used to notice multiple deliveries
	simtime_t sendTime;
} msgControl;

// Module which is used to control that mobile hosts deliver messages in causal order
// A station calls HostSystemJoin whenever a new host joins the system
// A host calls hostNotifiesSendMessage whenever it broadcasts a message
// A host calls deliverMessage whenever it delivers a message
class BroadcastController: public cSimpleModule
{
public:
	BroadcastController();
	virtual ~BroadcastController();
	virtual void initialize(int stage);
	void handleMessage(cMessage *msg) override;

	void HostSystemJoin(unsigned int id, unsigned int idMSS);

	void hostNotifiesBroadcastMessage(idMsg id);
	void deliverMessage(idMsg id, unsigned int idDest);

	msgControl& searchMessage(vector<msgControl>& vecMessages, unsigned int seq);
	bool satisfiesDeliveryConditions(const vector<unsigned int>& messageClock, const vector<unsigned int>& processClock,
			unsigned int idSource);
	bool entrySatisfiesDeliveryCondition(unsigned int index, unsigned int idSource, unsigned int toDeliverEntry,
			unsigned int localVectorEntry);
	void deleteMessage(idMsg id);
	void DeliveryError(string errorReason, idMsg idM, unsigned int destProcess,
			const vector<unsigned int>& messageClock, const vector<unsigned int>& processClock);
	void printClock(const vector<unsigned int>& v);
	void stationRemovesMessage(idMsg id, unsigned int idMSS);

	vector<vector<msgControl>> messages; // Each host has an assigned vector of messages it broadcasted
	unsigned int totalDelivered = 0; // total message deliveries performed by hosts
	simtime_t totalTimeDelivered = 0; // total message delivery delays, from a message broadcast to its delivery by hosts

private:
	vector<vector<unsigned int>> HostsClock;
	vector<vector<unsigned int>> MSSClock; // Clock of the oldest message station i broadcasts. Kept to assign it to processes which join the system

	SimulationParameters* params;
};

Define_Module(BroadcastController);

#endif /* BROADCASTCONTROLLER_H_ */
