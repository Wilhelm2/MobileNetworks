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

#include "StationSendingBuffer.h"
#include <memory>

StationSendingBuffer::StationSendingBuffer() {}

StationSendingBuffer::~StationSendingBuffer() {}

list<struct s_sendMsgMSS>::iterator StationSendingBuffer::begin() {
    return buffer.begin();
}

list<struct s_sendMsgMSS>::const_iterator StationSendingBuffer::begin() const {
    return buffer.begin();
}

list<struct s_sendMsgMSS>::const_iterator StationSendingBuffer::end() const {
    return buffer.end();
}

list<struct s_sendMsgMSS>::iterator StationSendingBuffer::end() {
    return buffer.end();
}

struct s_sendMsgMSS StationSendingBuffer::front() const {
    return buffer.front();
}

struct s_sendMsgMSS StationSendingBuffer::back() const {
    return buffer.back();
}

struct s_sendMsgMSS StationSendingBuffer::back() {
    return buffer.back();
}

struct s_sendMsgMSS StationSendingBuffer::extractMsg(idMsg idMessage) {
    list<struct s_sendMsgMSS>::iterator m;
    for (m = buffer.begin(); m != buffer.end(); m++) {
        if (ID_EQ(m->id, idMessage))
            break;
    }
    assert(m != buffer.end());  // message not found
    struct s_sendMsgMSS tmp = *m;
    buffer.erase(m);
    return tmp;
}

void StationSendingBuffer::push_back(struct s_sendMsgMSS m) {
    buffer.push_back(m);
}

s_sendMsgMSS StationSendingBuffer::searchMsg(unsigned int seq) const {
    for (const s_sendMsgMSS& it : buffer) {
        if (it.seqNC == seq)
            return it;
    }
    cerr << "Message MSS SBuffer " << seq << " not found" << endl;
    throw "Message not found";
}

list<struct s_sendMsgMSS>::iterator StationSendingBuffer::searchMsgIt(unsigned int seq) {
    for (list<struct s_sendMsgMSS>::iterator it = buffer.begin(); it != buffer.end(); it++) {
        if (it->seqNC == seq)
            return it;
    }
    cerr << "Message MSS SBuffer " << seq << " not found" << endl;
    throw "Message not found";
}

list<struct s_sendMsgMSS>::iterator StationSendingBuffer::searchMsgIt(idMsg idMessage) {
    for (list<struct s_sendMsgMSS>::iterator it = buffer.begin(); it != buffer.end(); it++) {
        if (ID_EQ(it->id, idMessage))
            return it;
    }
    cerr << "Message MSS SBuffer " << idMessage.idMH << "," << idMessage.seqMH << " not found" << endl;
    throw "Message not found";
}

bool StationSendingBuffer::contains(unsigned int seq) const {
    for (const s_sendMsgMSS& it : buffer) {
        if (it.seqNC == seq)
            return true;
    }
    return false;
}

bool StationSendingBuffer::contains(idMsg idMessage) const {
    for (const s_sendMsgMSS& it : buffer) {
        if (ID_EQ(it.id, idMessage))
            return true;
    }
    return false;
}

vector<cMessage*> StationSendingBuffer::clear() {
    vector<cMessage*> timeoutsToCancel;
    for (const struct s_sendMsgMSS& m : buffer) {
        if (m.timeout)
            timeoutsToCancel.push_back(m.timeout);
    }
    buffer.clear();
    return timeoutsToCancel;
}

vector<cMessage*> StationSendingBuffer::clear(unsigned int minSeq, unsigned int idMSS, BroadcastController* BC) {
    vector<cMessage*> timeoutsToCancel;
    list<struct s_sendMsgMSS>::iterator it;
    for (it = buffer.begin(); it != buffer.end() && it->seqNC <= minSeq; it++) {
        if (it->timeout)
            timeoutsToCancel.push_back(it->timeout);
        it->timeout = nullptr;
        BC->stationRemovesMessage(it->id, idMSS);
    }
    buffer.erase(buffer.begin(), it);
    return timeoutsToCancel;
}

unsigned int StationSendingBuffer::size() const {
    return buffer.size();
}

bool StationSendingBuffer::empty() const {
    return buffer.empty();
}

void StationSendingBuffer::print() const {
    cout << "buffer state:" << endl;
    for (const s_sendMsgMSS& it : buffer)
        cout << "(" << it.id.idMH << "," << it.id.seqMH << ") with seqNC " << it.seqNC << endl;
    cout << "-----" << endl;
}

void StationSendingBuffer::setTimer(cMessage* msg, unsigned int seqNC) {
    for (s_sendMsgMSS& it : buffer) {
        if (it.seqNC == seqNC) {
            it.timeout = msg;
            return;
        }
    }
    assert(true && "Message not found");
}

vector<idMsg> StationSendingBuffer::getIdMessagesReceivedAfter(const vector<unsigned int>& timeVector) const {
    vector<idMsg> msgReceivedAfter;
    for (const s_sendMsgMSS& it : buffer) {
        if (sumVector(timeVector) < sumVector(it.recvVector))
            msgReceivedAfter.push_back(it.id);
    }
    return msgReceivedAfter;
}

unsigned int StationSendingBuffer::sumVector(const vector<unsigned int>& v) const {
    return std::accumulate(v.begin(), v.end(), 0);
}

vector<tuple<idMsg, unsigned int>> StationSendingBuffer::getMessagesOrder() const {
    vector<tuple<idMsg, unsigned int>> ids;
    for (const s_sendMsgMSS& m : buffer)
        ids.push_back(make_tuple(m.id, m.seqNC));
    return ids;
}

bool StationSendingBuffer::containsMessage(idMsg IDMessage,
                                           const vector<idMsg>& msg_rcv,
                                           const vector<idMsg>& notDeliveredMessages) {
    auto lambda_exp = [IDMessage](const idMsg& id) { return ID_EQ(id, IDMessage); };

    bool msg_rcvContains = find_if(msg_rcv.begin(), msg_rcv.end(), lambda_exp) != msg_rcv.end();
    bool notDelieredMessagesContains =
        find_if(notDeliveredMessages.begin(), notDeliveredMessages.end(), lambda_exp) != notDeliveredMessages.end();
    if (msg_rcvContains || notDelieredMessagesContains)
        return false;
    else
        return true;
    //
    //	for (const idMsg& elt : msg_rcv)
    //	{
    //		if (ID_EQ(elt, IDMessage))
    //			return false;
    //	}
    //	for (const idMsg& id : notDeliveredMessages)
    //		if (ID_EQ(id, IDMessage))
    //			return false;
    //
    //	return true;
}

void StationSendingBuffer::tagAlreadyDeliveredMessagesByMH(const vector<unsigned int>& recvRsp1Vector,
                                                           const vector<idMsg>& notDeliveredMessages,
                                                           const vector<idMsg>& msg_rcv,
                                                           unsigned int id_h) {
    for (s_sendMsgMSS& it : buffer) {
        if (vectorSum(it.recvVector) < vectorSum(recvRsp1Vector) &&
            containsMessage(it.id, msg_rcv, notDeliveredMessages)) {
            it.MHdeliv.push_back(id_h);
            //			cout << "MH " << id_h << " has already delivered (" << it.id.idMH << "," << it.id.seqMH << ")" << "
            //seqNC "
            //					<< it.seqNC << " still broadcasted " << (it.timeout != nullptr) << endl;
        } else
            ;
        //			cout << "MH " << id_h << " has not delivered (" << it.id.idMH << "," << it.id.seqMH << ")
        //seqNC "
        //					<< it.seqNC << " still broadcasted " << (it.timeout != nullptr) << endl;
    }
}

unsigned int StationSendingBuffer::vectorSum(const vector<unsigned int>& v) {
    return std::accumulate(v.begin(), v.end(), 0);
}

void StationSendingBuffer::tagAlreadyDeliveredMessagesByMH(unsigned int idMH, unsigned int seqC) {
    list<s_sendMsgMSS>::iterator it;
    for (it = buffer.begin(); it != buffer.end() && it->seqNC <= seqC; it++)
        it->AddMHDeliv(idMH);
}
