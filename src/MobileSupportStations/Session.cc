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

#include "Session.h"

Session::Session() {
    handoffTimeout = nullptr;
}

Session::Session(unsigned int id,
                 unsigned int seqh,
                 unsigned int seqc,
                 const vector<AckInterval>& ackIntervals,
                 unsigned int Ses,
                 unsigned int MSSinCharge) {
    this->id = id;
    this->seqh = seqh;
    this->seqc = seqc;
    this->ackIntervals = ackIntervals;
    this->Ses = Ses;
    this->state = state;
    this->MSSinCharge = MSSinCharge;
    this->handoffTimeout = nullptr;
}

Session::~Session() {}

bool Session::isInHandoff() const {
    return (state != CONNECTION_STATE::Established && state != CONNECTION_STATE::Send_MD);
}

list<struct s_sendMsgMSS> Session::updateProcessReq1(vector<unsigned int>& recvReq1Vector,
                                                     unsigned int SesReq1,
                                                     unsigned int seqc,
                                                     unsigned int Ses_LC) {
    list<struct s_sendMsgMSS> timeoutsToCancel = {};
    this->recvReq1Vector = recvReq1Vector;
    state = Session::CONNECTION_STATE::Rcv_Req1;
    this->SesReq1 = SesReq1;
    if (Ses_LC == Ses) {
        this->seqc = seqc;
        timeoutsToCancel = coMsgToDeliver.clear(seqc);
    }
    return timeoutsToCancel;
}

void Session::updateProcessRsp1(unsigned int seqh,
                                const vector<unsigned int>& recvRsp1Vector,
                                const vector<idMsg>& notDeliveredMessages) {
    this->seqh = seqh;
    this->recvRsp1Vector = recvRsp1Vector;
    state = Session::CONNECTION_STATE::Rcv_Rsp1;
    this->notDeliveredMessages = notDeliveredMessages;
}

cMessage* Session::updateProcessRsp2(const list<struct s_sendMsgMSS>& appCoMsg) {
    cMessage* timeout = handoffTimeout;
    notDeliveredMessages.clear();
    handoffTimeout = nullptr;
    this->coMsgToDeliver.buffer = appCoMsg;
    state = Session::CONNECTION_STATE::Send_MD;
    return timeout;
}

bool Session::connectionIsEstablished() const {
    return state == CONNECTION_STATE::Established;
}

bool Session::connectionInHandoff() const {
    return state != CONNECTION_STATE::Established;
}

void Session::updateAcknowledgment(unsigned int seqc, const vector<AckInterval>& ackIntervals) {
    this->seqc = seqc;
    this->ackIntervals = ackIntervals;
}

bool Session::notInInitState() const {
    return state != CONNECTION_STATE::Init;
}

bool Session::isInCharge(unsigned int idMSS) const {
    return MSSinCharge == idMSS;
}

void Session::confirmConnection(unsigned int seqc) {
    state = Session::CONNECTION_STATE::Established;
    this->seqc = seqc;
    ackIntervals = {{1, seqc}};
}

StationAppCoSendingBuffer& Session::getCoMsgDeliver() {
    return coMsgToDeliver;
}

const StationAppCoSendingBuffer& Session::getCoMsgDeliver() const {
    return coMsgToDeliver;
}
