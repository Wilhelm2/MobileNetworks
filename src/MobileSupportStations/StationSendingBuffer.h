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

#ifndef MOBILESUPPORTSTATIONS_STATIONSENDINGBUFFER_H_
#define MOBILESUPPORTSTATIONS_STATIONSENDINGBUFFER_H_

#include <algorithm>
#include <vector>
#include "../BroadcastController.h"
#include "../Messages/Rsp2_m.h"
#include "../commun_f.h"

// Structure containing the information of a message a station broadcasts inside its cell
struct s_sendMsgMSS {
    unsigned int seqNC;  // Sequence number associated to the message
    idMsg id;            // The message's id
    // The message's timeout. The message is resend upon expiration of the timeout.
    // Must be kept as pointer because timeouts are stored in StationSendingSBuffer but are deleted/scheduled in the MSS
    // class
    cMessage* timeout;
    // Contains the id of some hosts which have already delivered the message. Used during handoffs.
    // See paper for explanation
    vector<unsigned int> MHdeliv;
    // Used as reception time of messages. A simtime_t variable is not used because two messages can be received at the
    // same simetime_t.
    vector<unsigned int> recvVector;

    // Returns whether MHdeliv contains idh or not
    bool MHdelivContains(unsigned int idh) const {
        return std::find(MHdeliv.begin(), MHdeliv.end(), idh) != MHdeliv.end();
    }

    // Adds idh to MHdeliv if it does not already contain it
    void AddMHDeliv(unsigned int idh) {
        if (!MHdelivContains(idh))
            MHdeliv.push_back(idh);
    }

    // Removes idh of MHdeliv if MHdeliv already contains it
    void MHdelivRemove(unsigned int idh) {
        for (vector<unsigned int>::iterator it = MHdeliv.begin(); it != MHdeliv.end(); it++) {
            if (*it == idh) {
                MHdeliv.erase(it);
                return;
            }
        }
    }

    s_sendMsgMSS(unsigned int seqNC,
                 idMsg id,
                 cMessage* timeout,
                 vector<unsigned int> MHdeliv,
                 vector<unsigned int> recvVector) {
        this->seqNC = seqNC;
        this->id = id;
        this->timeout = timeout;
        this->MHdeliv = MHdeliv;
        this->recvVector = recvVector;
    }
    s_sendMsgMSS() {}
};

// Contains the sending buffer of stations.
// A station stores in its sending buffer the messages it broadcasts to mobile hosts connected to it
class StationSendingBuffer {
   public:
    StationSendingBuffer();
    virtual ~StationSendingBuffer();

    list<struct s_sendMsgMSS>::iterator begin();
    list<struct s_sendMsgMSS>::const_iterator begin() const;
    list<struct s_sendMsgMSS>::iterator end();
    list<struct s_sendMsgMSS>::const_iterator end() const;

    struct s_sendMsgMSS front() const;
    struct s_sendMsgMSS back() const;
    struct s_sendMsgMSS back();

    // Extract the message of id idMessage from the buffer
    struct s_sendMsgMSS extractMsg(idMsg idMessage);
    void push_back(struct s_sendMsgMSS m);

    // Returns the message of sequence number seq from the buffer
    s_sendMsgMSS searchMsg(unsigned int seq) const;
    // Returns an iterator to the entry of the buffer to the message of sequence number seq
    list<struct s_sendMsgMSS>::iterator searchMsgIt(unsigned int seq);
    // Returns an iterator to the entry of the buffer to the message of id idMessage
    list<struct s_sendMsgMSS>::iterator searchMsgIt(idMsg idMessage);
    // Returns whether the message of sequence number seq is contained in the buffer or not
    bool contains(unsigned int seq) const;
    // Returns whether the message of id idMessage is contained in the buffer or not
    bool contains(idMsg idMessage) const;

    // Clears all the message from the buffer and returns the timeouts to cancel
    vector<cMessage*> clear();
    // Clears all the message from the buffer whose sequence number is lower than minSeq, acknowledges the messages to
    // BC, and returns the timeouts to cancel
    vector<cMessage*> clear(unsigned int minSeq, unsigned int idMSS, BroadcastController* BC);

    unsigned int size() const;
    bool empty() const;
    void print() const;

    // Sets the timer of the message whose sequence number is seqNC to msg
    void setTimer(cMessage* msg, unsigned int seqNC);
    // Returns the ids of the messages received after timeVector
    vector<idMsg> getIdMessagesReceivedAfter(const vector<unsigned int>& timeVector) const;
    // Returns the sum of the entries of an unsigned int vector
    unsigned int sumVector(const vector<unsigned int>& v) const;
    // Returns a vector containing one tuple <idMsg, seq> for each message of id idMsg broadcasted by the station,
    // with seq the sequence number that the station associated to idMsg
    vector<tuple<idMsg, unsigned int>> getMessagesOrder() const;
    // Returns whether the two lists msg_rcv and notDeliveredMessages contain IDMessage or not
    bool containsMessage(idMsg IDMessage, const vector<idMsg>& msg_rcv, const vector<idMsg>& notDeliveredMessages);

    // Tags all messages received before recvRsp1 that where already delivered by host id_h
    void tagAlreadyDeliveredMessagesByMH(const vector<unsigned int>& recvRsp1Vector,
                                         const vector<idMsg>& notDeliveredMessages,
                                         const vector<idMsg>& msg_rcv,
                                         unsigned int id_h);
    // Returns the sum of an int vector
    unsigned int vectorSum(const vector<unsigned int>& v);
    // Tags all messages that where already delivered by host idMH based on seqC
    void tagAlreadyDeliveredMessagesByMH(unsigned int idMH, unsigned int seqC);

    // Contains the messages
    list<struct s_sendMsgMSS> buffer;
};

#endif /* MOBILESUPPORTSTATIONS_STATIONSENDINGBUFFER_H_ */
