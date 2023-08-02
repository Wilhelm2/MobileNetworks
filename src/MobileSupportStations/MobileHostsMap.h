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

#ifndef MOBILESUPPORTSTATIONS_MOBILEHOSTSMAP_H_
#define MOBILESUPPORTSTATIONS_MOBILEHOSTSMAP_H_

#include <vector>
#include "../commun_f.h"
#include "Session.h"

// Map that contains the connections of stations with hosts in Session instances
// A station *s_i* maintains a Session for each connection of a host with *s_i* or a station of the group of *s_i*
class MobileHostsMap
{
public:
	MobileHostsMap();
	virtual ~MobileHostsMap();

	// Adds a session to the map
	void addSession(Session Ses, unsigned int idMH);
	// Registers a host by registering the session mh or updating it if this session is already registered locally
	void registerHost(Session mh);
	// Removes a session if it is already registered. Otherwise does nothing.
	void removeSession(unsigned int idMH, unsigned int Ses);
	// Removes the sessions with idMH whose session sequence number are lower than Ses
	vector<cMessage*> deleteSessions(unsigned int idMH, unsigned int Ses);

	// Finds the session of idMH with the session sequence number Ses. Returns Mobiles[id_h].end() if the session is not registered
	vector<Session>::iterator findSession(unsigned int idMH, unsigned int Ses);
	// REturns if registers a session of idMH that is not in handoff. Returns Mobiles[id_h].end() if no registered session is not in handoff
	vector<Session>::iterator getSessionIfNotInHandoff(unsigned int idMH);
	// Returns if registers a session of idMH that received the RecvReq1 message. Returns Mobiles[id_h].end() if not
	vector<Session>::iterator getSessionIfRecvReq1(unsigned int idMH);
	// Returns if registers a session of idMH of which the local station is in charge. Returns Mobiles[id_h].end() if not
	vector<Session>::iterator getSessionInCharge(unsigned int idMH, unsigned int idMSS);
	// Returns true if the host idMH is registered and false otherwise.
	bool isRegistered(unsigned int idMH);
	// Returns true if session Ses of host idMH is registered and false otherwise
	bool isSessionRegistered(unsigned int idMH, unsigned int Ses);
	// Returns true if has a session with idMH that is not in handoff
	bool hasSessionNotInHadoff(unsigned int idMH);
	// Returns true if it has a session with idMH that is between the reception of Req1 and Req2
	bool hasSessionRecvReq1(unsigned int idMH);
	// Returns true if idMSS is in charge of a session with idMH
	bool hasSessionInCharge(unsigned int idMH, unsigned int idMSS);

	// Acknowledges the AppCo messages of the session (idMH, Ses) with seqC
	list<struct s_sendMsgMSS> acknowledgeAppCo(unsigned int idMH, unsigned int Ses, unsigned int seqC);
	// Increments the seqh of idMH to acknowledge the reception of a message broadcasted by idMH
	void increasesAckh(unsigned int idMH);
	// Returns the number of connections registered in the cell of idMSS
	unsigned int nbHostsInCell(unsigned int idMSS);
	// Returns the minimal sequence number of the registered sessions
	unsigned int searchMinSeq();
	// Returns the list of minimal seqC numbers among the connection of which idMSS is in charge
	vector<unsigned int> getMinSeqCRegisteredMH(unsigned int idMSS);
	// Clears all sessions and returns the list of timeouts to cancel
	list<struct s_sendMsgMSS> clear();
	// Returns whether the map is empty or not
	bool empty();
	// Returns whether all hosts connected to idMSS have delivered the message seq or not
	bool allMHDelivered(unsigned int seq, unsigned int idMSS);

	// Returns the vector used to acknowledge messages to other stations
	vector<tuple<unsigned int, unsigned int, unsigned int>> createAckSSVectorConnectedHosts(unsigned int idMSS);
	// Returns the vector used to acknowledge messages to host connected to station idMSS
	vector<idMsg> createAckHVectorConnectedHosts(unsigned int idMSS);

	// For each host idMH contains a vector with the connections of idMH with stations of the group of the current station
	map<unsigned int, vector<Session> > Mobiles;
};

#endif /* MOBILESUPPORTSTATIONS_MOBILEHOSTSMAP_H_ */
