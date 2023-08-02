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

#include "MHSBuffer.h"

MHSBuffer::MHSBuffer() {}

MHSBuffer::~MHSBuffer() {}

struct s_sendMsgMH MHSBuffer::searchMsg(unsigned int seq) {
    for (struct s_sendMsgMH& it : buffer) {
        if (it.seqh == seq)
            return it;
    }
    cerr << "Message " << seq << " not found" << endl;
    exit(1);
}

void MHSBuffer::push_back(struct s_sendMsgMH msg) {
    buffer.push_back(msg);
}

vector<cMessage*> MHSBuffer::ackMessages(unsigned int seqACK) {
    vector<cMessage*> timeoutsToCancel;
    std::list<struct s_sendMsgMH>::iterator it = buffer.begin();
    while (it != buffer.end() && it->seqh <= seqACK) {
        timeoutsToCancel.push_back(it->timeout);
        it++;
    }
    buffer.erase(buffer.begin(), it);
    return timeoutsToCancel;
}

vector<cMessage*> MHSBuffer::getTimeouts() {
    vector<cMessage*> timeouts;
    for (struct s_sendMsgMH& m : buffer)
        timeouts.push_back(m.timeout);
    return timeouts;
}
