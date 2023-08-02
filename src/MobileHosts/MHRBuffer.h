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

#ifndef MHRBuffer_H_
#define MHRBuffer_H_

#include "../commun_f.h"

struct s_recvMsgMH
{
	unsigned int seqC;
	idMsg id;
	bool hasAlreadyDelivered;
};

// Reception buffer of hosts
class MHRBuffer
{
public:
	MHRBuffer();
	virtual ~MHRBuffer();
	void clear();
	unsigned int size() const;
	void print();
	// inserts the message in the reception buffer if it is not already contained (checked with seqC)
	void insertMsg(struct s_recvMsgMH m);
	// Removes the message from the buffer whose sequence number is lower than seqAck
	void erase(unsigned int seqAck);
	// Switches buffer to oldBuffer when changing cell. Keeps the buffer in oldBuffer
	// to try to keep messages when it receives the order of messages by the new station
	void switchBuffer();
	// Tries to put messages from oldBuffer into buffer with the sequence number contained in updatedSeqS
	// The function is executed when the host receives an AppCo or InitAck message from the station to which it tries to connect
	// updatedSeqS contains the order with which the new station orders application messages
	void updateAllSeqS(const vector<tuple<idMsg, unsigned int>>& updatedSeqS);
	// Checks if a message can be delivered
	bool isDeliverableMessage(unsigned int seqNC);
	// Pops the front of the buffer
	struct s_recvMsgMH popFront();
	// Computes ack intervals based on the messages contained in the buffer
	vector<AckInterval> getAckIntervals(unsigned int seqS);

	// Buffer containing pending messages application messages received from the station to which the host is connected
	// Messages are took from the buffer once they are FIFO-ordered
	list<struct s_recvMsgMH> buffer;
	// Contains the old buffer state before the host changed cell. The host keeps it till it receives the first AckCo or InitAck message
	// The purpose is to not loose the received but not delivered messages when changing cell
	list<struct s_recvMsgMH> oldBuffer;
};

#endif /* MHRBuffer_H_ */
