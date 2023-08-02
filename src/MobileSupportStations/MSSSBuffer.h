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

#ifndef MOBILESUPPORTSTATIONS_MSSSBUFFER_H_
#define MOBILESUPPORTSTATIONS_MSSSBUFFER_H_

#include <vector>
#include <algorithm>

#include "../Messages/Rsp2_m.h"
#include "../commun_f.h"
#include "Session.h"
#include "StationSendingBuffer.h"
#include "../BroadcastController.h"
#include "../Messages/TimeoutAppS_h_m.h"
#include "MobileHostsMap.h"

// Contains the sending buffer of stations.
// *buffer* contains the messages that are currently broadcasted by the station.
// *pendingMsg* contains the messages whose broadcast is delayed upon reception
// of their seqC from the station's group responsible station (received in a App_sresp message)
class MSSSBuffer
{
public:
	MSSSBuffer();
	virtual ~MSSSBuffer();

	// Extracts the message idMessage from pendingMsg
	struct s_sendMsgMSS extractPendingMsg(idMsg idMessage);
	// Extracts the first message from pendingMsg
	struct s_sendMsgMSS extractFirstPendingMsg();
	// Pushes back m on pendingMsg
	void pendingMsgPushBack(struct s_sendMsgMSS m);
	// Returns whether pendingMsg is empty or not
	bool hasPendingMessages();

	// Pushes back the message idM of sequence number seqS to the buffer of broadcasted messages
	list<struct s_sendMsgMSS>::iterator push_backBroadcastedMessages(idMsg idM, unsigned int seqS);
	// Returns the structure associated to the broadcasted message of sequence number seq
	struct s_sendMsgMSS getBroadcastedMessage(unsigned int seq);
	// Sets the timer of the broadcasted message of sequence number seq
	void setTimerBroadcastedMessage(cMessage*m, unsigned int seq);
	// Returns whether the buffer of broadcasted messages contains the message of sequence number seq
	bool broadcastedMessagesContain(unsigned int seq);
	// Returns the first entry of buffer which is still broadcasted
	list<struct s_sendMsgMSS>::iterator beginStillBroadcastedMessages();
	// Resends messages whose broadcast was stopped. Only resends the messages which have an associated
	// sequence number higher than the message with the lowest sequence number that is still broadcasted
	vector<TimeoutAppS_h*> resendBroadcastedMessages(unsigned int id_h);
	// Checks if can stop to broadcast messages
	vector<cMessage*> checkStopBroadcastMessages(MobileHostsMap& Mobiles, const vector<AckInterval>& ackIntervals,
			unsigned int idMSS);

	// Sets BC
	void setBC(BroadcastController* BC);
	// Returns seqC
	unsigned int getSeqC();
	// Increments SeqC by one
	void incrementSeqC();

	// Returns whether the sending buffer contains idMessage or not
	bool contains(idMsg idMessage) const;
	// Returns the messages among messages that are neither contained in buffer nor pendingMsg
	vector<idMsg> notContainedMessages(vector<idMsg> messages);
	// Returns whether pendingMsg and buffer are both empty
	bool empty() const;
	// Returns the size of pendingMsg + buffer
	size_t size() const;
	// Gets the maximal attributed seqNC
	unsigned int getMaxSeqNC() const;
	// Gets the minimal attributed seqNC that is still buffered
	unsigned int getMinSeqNC() const;
	// Gets the minimal attributed seqNC that is still broadcasted
	unsigned int getMinBroadcastedSeqNC() const;

	// Removes from buffer all messages whose sequence number is lower or equal to minSeq
	// and returns the timeouts of those messages
	vector<cMessage*> clearSBuffer(unsigned int minSeq, unsigned int idMSS, BroadcastController *BC);
	// Removes all messages from buffer and pendingMsg and returns
	// the timouts of the messages of buffer
	vector<cMessage*> clear();

	// Determines the sequence number to associate to host id_h
	unsigned int determineSeqC(unsigned int id_h) const;
	// Returns the id of all messages not delivered by idMH whose sequence number is higher than seqACK
	vector<idMsg> getIdNotDeliveredMessages(unsigned int idMH, unsigned int seqACK) const;
	// Returns the id of all messages received after timeVector
	vector<idMsg> getIdMessagesReceivedAfter(const vector<unsigned int>& timeVector) const;
	// Returns the id and seq of the broadcasted messages
	vector<tuple<idMsg, unsigned int>> getMessagesOrderBroadcastedMessages() const;
	// Returns the id of messages that idMH did not deliver and that the station does not broadcast anymore
	vector<idMsg> getIdMessagesNotDeliveredNorBroadcasted(unsigned int idMH);

	// Tags the messages that id_h already delivered. See paper for explanations
	void tagAlreadyDeliveredMessagesByMH(const vector<unsigned int>& recvRsp1Vector,
			const vector<idMsg>& notDeliveredMessages, const vector<idMsg>& msg_rcv, unsigned int id_h);
	// Tags the messages with id_h whose sequence number are lower or equal to seqC
	void tagAlreadyDeliveredMessagesByMH(unsigned int id_h, unsigned int seqC);
	// Tags the messages with id_h whose id is contained in acknowledgedAppCo
	void tagAlreadyDeliveredMessagesByMH(const list<struct s_sendMsgMSS>& acknowledgedAppCo, unsigned int idMH);

private:
	// Contains currently broadcasted application messages
	StationSendingBuffer buffer;
	// Contains messages delayed till receiving their seqS from group responsible
	StationSendingBuffer pendingMsg;

	// Sequence number counter used to give sequence number to broadcasted application messages
	unsigned int seqC = 1;
	// Reference to broadcast controller to avoid resolving it every time
	BroadcastController *BC;
};

#endif /* MOBILESUPPORTSTATIONS_MSSSBUFFER_H_ */
