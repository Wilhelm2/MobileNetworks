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

#include "MHRBuffer.h"

MHRBuffer::MHRBuffer() {}

MHRBuffer::~MHRBuffer() {}

void MHRBuffer::clear() {
    buffer.clear();
}

unsigned int MHRBuffer::size() const {
    return buffer.size();
}

void MHRBuffer::print() {
    std::cout << "Number of messages in buffer " << buffer.size() << endl;
    for (struct s_recvMsgMH& it : buffer)
        std::cout << "Message (" << it.id.idMH << "," << it.id.seqMH << ") with seqS " << it.seqC << endl;
}

void MHRBuffer::insertMsg(struct s_recvMsgMH m) {
    std::list<struct s_recvMsgMH>::iterator it = buffer.begin();
    while (it != buffer.end() && it->seqC < m.seqC)
        it++;
    if (it == buffer.end() || it->seqC != m.seqC)
        buffer.insert(it, m);
}

void MHRBuffer::erase(unsigned int seqAck) {
    std::list<struct s_recvMsgMH>::iterator it2 = buffer.begin();
    while (it2 != buffer.end() && it2->seqC <= seqAck)
        it2++;
    buffer.erase(buffer.begin(), it2);
}

void MHRBuffer::switchBuffer() {
    oldBuffer = buffer;
    buffer.clear();
}

void MHRBuffer::updateAllSeqS(const vector<tuple<idMsg, unsigned int>>& updatedSeqS) {
    for (struct s_recvMsgMH& m : oldBuffer) {
        for (const tuple<idMsg, unsigned int>& update : updatedSeqS) {
            if (ID_EQ(m.id, get<0>(update))) {
                m.seqC = get<1>(update);
                insertMsg(m);
                break;
            }
        }
    }
    oldBuffer.clear();
}

bool MHRBuffer::isDeliverableMessage(unsigned int seqNC) {
    return !buffer.empty() && buffer.front().seqC == seqNC + 1;
}

struct s_recvMsgMH MHRBuffer::popFront() {
    struct s_recvMsgMH m = buffer.front();
    buffer.pop_front();
    return m;
}

vector<AckInterval> MHRBuffer::getAckIntervals(unsigned int seqS) {
    unsigned int begin = seqS;
    unsigned int seqSofPreviousMsg = seqS;
    vector<AckInterval> ackIntervals = {{0, seqS}};
    for (const struct s_recvMsgMH& m : buffer) {
        if (seqSofPreviousMsg + 1 != m.seqC) {
            ackIntervals.push_back({begin, seqSofPreviousMsg});
            begin = m.seqC;
            seqSofPreviousMsg = m.seqC;
        } else
            seqSofPreviousMsg++;
    }
    ackIntervals.push_back({begin, seqSofPreviousMsg});
    return ackIntervals;
}
