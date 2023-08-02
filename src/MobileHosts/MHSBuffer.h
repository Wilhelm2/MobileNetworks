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

#ifndef MHSBuffer_H_
#define MHSBuffer_H_

#include "../commun_f.h"

struct s_sendMsgMH
{
	unsigned int seqh;
	cMessage* timeout;
};

// Sending buffer of hosts
class MHSBuffer
{
public:
	MHSBuffer();
	virtual ~MHSBuffer();
	// Searches for a message in the sending buffer. Terminates the program if the message is not contained (because it should and therefore means there is a bug)
	struct s_sendMsgMH searchMsg(unsigned int seq);
	void push_back(struct s_sendMsgMH msg);
	// Acknowledges messages based on seqAck and returns the sending timeouts of those messages such that the host can cancel and delete them.
	vector<cMessage*> ackMessages(unsigned int seqACK);
	// Returns all message timeouts such that the host can cancel and delete them
	vector<cMessage*> getTimeouts();

	// Sending buffer
	list<struct s_sendMsgMH> buffer;
};

#endif /* MHSBuffer_H_ */
