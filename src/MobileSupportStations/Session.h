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

#ifndef MOBILESUPPORTSTATIONS_SESSION_H_
#define MOBILESUPPORTSTATIONS_SESSION_H_

#include "../Messages/timeoutHandoff_m.h"
#include "../commun_f.h"
#include "StationAppCoSendingBuffer.h"
#include "StationReceiveBuffer.h"
#include "StationSendingBuffer.h"

class StationSendingBuffer;

// Represents the connections of a host to a station
class Session {
   public:
    enum class CONNECTION_STATE { Init, Rcv_Rsp1, Send_MD, Established, Rcv_Req1 };
    Session();
    virtual ~Session();
    Session(unsigned int id,
            unsigned int seqh,
            unsigned int seqc,
            const vector<AckInterval>& ackIntervals,
            unsigned int Ses,
            unsigned int MSSinCharge);
    bool isInHandoff() const;
    // Updates the session upon processing a Req1 message
    list<struct s_sendMsgMSS> updateProcessReq1(vector<unsigned int>& recvReq1Vector,
                                                unsigned int SesReq1,
                                                unsigned int seqc,
                                                unsigned int Ses_LC);
    // Updates the session upon processing a Rsp1 message
    void updateProcessRsp1(unsigned int seqh,
                           const vector<unsigned int>& recvRsp1Vector,
                           const vector<idMsg>& notDeliveredMessages);
    // Updates the session upon processing a Rsp2 message
    cMessage* updateProcessRsp2(const list<struct s_sendMsgMSS>& appCoMsg);
    bool connectionIsEstablished() const;
    bool connectionInHandoff() const;
    // Updates the stored acknowledge sequence number and acknowledge interval
    void updateAcknowledgment(unsigned int seqc, const vector<AckInterval>& ackIntervals);
    bool notInInitState() const;
    bool isInCharge(unsigned int idMSS) const;
    void confirmConnection(unsigned int seqc);
    StationAppCoSendingBuffer& getCoMsgDeliver();
    const StationAppCoSendingBuffer& getCoMsgDeliver() const;

    unsigned int id;                   // the host's id
    unsigned int seqh;                 // means that the host delivered the messages $m$ with m.seq <= seqh
    unsigned int seqc;                 // means that the host delivered the messages $m$ with m.seq <= seqc
    vector<AckInterval> ackIntervals;  // Contains intervals of sequences in order to acknowledge messages more
                                       // efficiently and broadcast them less on the wireless network
    StationReceiveBuffer RBuffer;      // Reception buffer of application messages broadcasted by host id
    unsigned int
        Ses;  // Session sequence number. Each session has an associated session sequence number given by the host id.
              // Host id increments the session sequence number at each new connection it tries to establish
    Session::CONNECTION_STATE state = Session::CONNECTION_STATE::Init;

    unsigned int MSSinCharge = -1;  // Station in charge of host id. Necessary since stations of a group keep a copy of
                                    // the connection of hosts connected to other stations of the group

    // fields specific to handoffs
    vector<unsigned int> recvReq1Vector;  // Vector used to determine the messages received after Req1
    vector<unsigned int> recvRsp1Vector;  // Vector used to determine the messages received after Rsp1
    unsigned int SesReq1;                 // Contains the session number of the currently performed handoff
    TimeoutHandoff* handoffTimeout;  // Must be a pointer because MSS must own it since the timer cannot be cancelled in
                                     // the Session function
    vector<idMsg>
        notDeliveredMessages;  // Contains the id of not delivered messages, stored to be used upon reception of Rsp2

   private:
    StationAppCoSendingBuffer coMsgToDeliver;  // Contains the application messages the host must deliver before
                                               // delivering application messages currently broadcasted by the station
};

#endif /* MOBILESUPPORTSTATIONS_SESSION_H_ */
