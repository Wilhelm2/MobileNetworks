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

#include "MessageFactory.h"
const unsigned int MessageFactory::messageLength = 100;

MessageFactory::MessageFactory() {}

MessageFactory::~MessageFactory() {}

IntrusivePtr<Apph_S> MessageFactory::createApph_S(idMsg id, unsigned int id_S, unsigned int Ses) {
    const IntrusivePtr<Apph_S>& payload = makeShared<Apph_S>();
    payload->setChunkLength(B(messageLength + 3));
    payload->setId(id);
    payload->setCellId(id_S);
    payload->setSes(Ses);
    return payload;
}

IntrusivePtr<AppS_S> MessageFactory::createAppS_S(unsigned int id_S,
                                                  idMsg id,
                                                  unsigned int SourceId,
                                                  unsigned int SourceSeq,
                                                  unsigned int Ses) {
    const IntrusivePtr<AppS_S>& payload = makeShared<AppS_S>();
    payload->setChunkLength(B(messageLength + 6));
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setId(id);
    payload->setId_S(id_S);
    payload->setSes(Ses);
    return payload;
}

IntrusivePtr<App_resp> MessageFactory::createApp_resp(idMsg id,
                                                      unsigned int s_resp,
                                                      unsigned int seqS,
                                                      unsigned int SourceId,
                                                      unsigned int SourceSeq) {
    const IntrusivePtr<App_resp>& payload = makeShared<App_resp>();
    payload->setChunkLength(B(5));
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setId(id);
    payload->setS_resp(s_resp);
    payload->setSeqS(seqS);
    return payload;
}

IntrusivePtr<AppS_h> MessageFactory::createAppS_h(unsigned int seq_S,
                                                  unsigned int id_S,
                                                  const vector<unsigned int>& M_d,
                                                  idMsg id) {
    const IntrusivePtr<AppS_h>& payload = makeShared<AppS_h>();
    payload->setSeq_S(seq_S);
    payload->setCellId(id_S);
    payload->setId(id);
    payload->setM_d(M_d);
    payload->setChunkLength(B(4 + M_d.size() + messageLength));
    return payload;
}

IntrusivePtr<AppCo> MessageFactory::createAppCo(unsigned int id_S,
                                                unsigned int seq_S,
                                                unsigned int targetHostId,
                                                unsigned int Ses,
                                                idMsg id) {
    const IntrusivePtr<AppCo>& payload = makeShared<AppCo>();
    payload->setChunkLength(B(messageLength + 6));
    payload->setSeq_S(seq_S);
    payload->setCellId(id_S);
    payload->setTargetHostId(targetHostId);
    payload->setId(id);
    payload->setSes(Ses);
    return payload;
}

IntrusivePtr<AppCoAck> MessageFactory::createAppCoAck(unsigned int id_h,
                                                      unsigned int seq_S,
                                                      unsigned int Ses,
                                                      unsigned int cellId) {
    const IntrusivePtr<AppCoAck>& payload = makeShared<AppCoAck>();
    payload->setChunkLength(B(4));
    payload->setId_h(id_h);
    payload->setCellId(cellId);
    payload->setSeq_S(seq_S);
    payload->setSes(Ses);
    return payload;
}

IntrusivePtr<Ack_h> MessageFactory::createAck_h(unsigned int id_h,
                                                const vector<AckInterval>& intervals,
                                                unsigned int seq_S,
                                                unsigned int Ses,
                                                unsigned int id_S) {
    const IntrusivePtr<Ack_h>& payload = makeShared<Ack_h>();
    payload->setId_h(id_h);
    payload->setCellId(id_S);
    payload->setSeq_S(seq_S);
    payload->setSes(Ses);
    payload->setIntervals(intervals);
    payload->setChunkLength(B(4 + intervals.size() * 2));
    return payload;
}

IntrusivePtr<Ack_S> MessageFactory::createAck_S(unsigned int id_S, const vector<idMsg>& vSeq) {
    const IntrusivePtr<Ack_S>& payload = makeShared<Ack_S>();
    payload->setChunkLength(B(1 + 2 * vSeq.size()));
    payload->setVSeq(vSeq);
    payload->setCellId(id_S);
    return payload;
}

IntrusivePtr<AckSS> MessageFactory::createAckSS(
    unsigned int id_S,
    const vector<tuple<unsigned int, unsigned int, unsigned int>>& vseqAckSS,
    unsigned int SourceId,
    unsigned int SourceSeq) {
    const IntrusivePtr<AckSS>& payload = makeShared<AckSS>();
    payload->setChunkLength(B(3 + 3 * vseqAckSS.size()));
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setVSeq(vseqAckSS);
    payload->setIdS(id_S);
    return payload;
}

IntrusivePtr<Init> MessageFactory::createInit(unsigned int targetHostId,
                                              unsigned int seq_S,
                                              unsigned int Ses,
                                              unsigned int Ses_LC,
                                              unsigned int id_S,
                                              MHState::STATE state) {
    const IntrusivePtr<Init>& payload = makeShared<Init>();
    payload->setChunkLength(B(8));
    payload->setCellId(id_S);
    payload->setSeq_S(seq_S);
    payload->setTargetHostId(targetHostId);
    payload->setSes(Ses);
    payload->setId_S(id_S);
    payload->setSes_LC(Ses_LC);
    payload->setState(state);
    return payload;
}

IntrusivePtr<Req1> MessageFactory::createReq1(unsigned int id_h,
                                              unsigned int seq_S,
                                              unsigned int Ses_LC,
                                              unsigned int Ses,
                                              const StationGroup& MSSb,
                                              unsigned int SourceId,
                                              unsigned int SourceSeq) {
    const IntrusivePtr<Req1>& payload = makeShared<Req1>();
    payload->setChunkLength(B(6 + MSSb.size()));
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setSeq_S(seq_S);
    payload->setId_h(id_h);
    payload->setSes_LC(Ses_LC);
    payload->setSes(Ses);
    payload->setMSSb(MSSb);
    return payload;
}

IntrusivePtr<Rsp1> MessageFactory::createRsp1(unsigned int id_h,
                                              unsigned int seq_h,
                                              const vector<idMsg>& m_nd,
                                              unsigned int Ses,
                                              unsigned int SourceId,
                                              unsigned int SourceSeq) {
    const IntrusivePtr<Rsp1>& payload = makeShared<Rsp1>();
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setM_nd(m_nd);
    payload->setSeq_h(seq_h);
    payload->setId_h(id_h);
    payload->setSes(Ses);
    payload->setChunkLength(B(5 + 2 * m_nd.size()));
    return payload;
}

IntrusivePtr<Req2> MessageFactory::createReq2(unsigned int id_h,
                                              const vector<idMsg>& msg_req,
                                              unsigned int Ses,
                                              const StationGroup& MSSb,
                                              unsigned int SourceId,
                                              unsigned int SourceSeq) {
    const IntrusivePtr<Req2>& payload = makeShared<Req2>();
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setMsg_req(msg_req);
    payload->setId_h(id_h);
    payload->setSes(Ses);
    payload->setChunkLength(B(2 + 2 * msg_req.size() + MSSb.size()));
    payload->setMSSb(MSSb);
    return payload;
}

IntrusivePtr<Rsp2> MessageFactory::createRsp2(unsigned int id_h,
                                              const vector<idMsg>& msg,
                                              const vector<idMsg>& msg_rcv,
                                              unsigned int Ses,
                                              unsigned int SourceId,
                                              unsigned int SourceSeq) {
    const IntrusivePtr<Rsp2>& payload = makeShared<Rsp2>();
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setId_h(id_h);
    payload->setSes(Ses);
    payload->setChunkLength(B(msg.size() * (messageLength + 2) + 2 + 2 * msg_rcv.size()));

    payload->setMsg(msg);
    payload->setMsg_rcv(msg_rcv);

    return payload;
}

IntrusivePtr<InitAck> MessageFactory::createInitAck(unsigned int id_S,
                                                    unsigned int id_h,
                                                    unsigned int seq_h,
                                                    unsigned int seq_S,
                                                    unsigned int Ses,
                                                    const vector<tuple<idMsg, unsigned int>>& MsgOrder) {
    const IntrusivePtr<InitAck>& payload = makeShared<InitAck>();
    payload->setCellId(id_S);
    payload->setId_h(id_h);
    payload->setSeq_h(seq_h);
    payload->setSeq_S(seq_S);
    payload->setSes(Ses);
    payload->setMsgOrder(MsgOrder);
    payload->setChunkLength(B(5 + MsgOrder.size() * 3));
    return payload;
}

IntrusivePtr<Delete> MessageFactory::createDelete(unsigned int Ses,
                                                  unsigned int id_h,
                                                  unsigned int SourceId,
                                                  unsigned int SourceSeq) {
    const IntrusivePtr<Delete>& payload = makeShared<Delete>();
    payload->setChunkLength(B(4));
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setId_h(id_h);
    payload->setSes(Ses);
    return payload;
}

IntrusivePtr<Register> MessageFactory::createRegister(const Session& hi,
                                                      unsigned int SourceId,
                                                      unsigned int SourceSeq) {
    const IntrusivePtr<Register>& payload = makeShared<Register>();
    payload->setChunkLength(B(2 + sizeof(hi) + messageLength * hi.getCoMsgDeliver().size()));
    payload->setHi(hi);
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    return payload;
}

IntrusivePtr<RecoverMSSReq> MessageFactory::createRecoverMSSReq(unsigned int SourceId,
                                                                unsigned int SourceSeq,
                                                                unsigned int recoveringMSSId) {
    const IntrusivePtr<RecoverMSSReq>& payload = makeShared<RecoverMSSReq>();
    payload->setChunkLength(B(3));
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setRecoveringMSSId(recoveringMSSId);
    return payload;
}

IntrusivePtr<RecoverMSS> MessageFactory::createRecoverMSS(unsigned int MSScible,
                                                          unsigned int SourceId,
                                                          unsigned int SourceSeq,
                                                          const MSSSBuffer& SBuffer,
                                                          unsigned int seq_S,
                                                          unsigned int s_resp,
                                                          const vector<unsigned int>& received,
                                                          const map<unsigned int, vector<Session>>& Mobiles) {
    const IntrusivePtr<RecoverMSS>& payload = makeShared<RecoverMSS>();

    payload->setSBuffer(SBuffer);
    payload->setChunkLength(
        B(6 + SBuffer.size() * messageLength + received.size() * messageLength + Mobiles.size() * 15));
    payload->setSourceId(SourceId);
    payload->setSourceSeq(SourceSeq);
    payload->setMSScible(MSScible);

    payload->setReceived(received);
    payload->setSeq_S(seq_S);
    payload->setS_resp(s_resp);

    payload->setMobiles(Mobiles);
    return payload;
}

TimeoutAppS_h* MessageFactory::createTimeoutAppS_h(unsigned int seq) {
    TimeoutAppS_h* payload = new TimeoutAppS_h();
    payload->setSeq(seq);
    return payload;
}

TimeoutApph_S* MessageFactory::createTimeoutApph_S(unsigned int seq) {
    TimeoutApph_S* payload = new TimeoutApph_S();
    payload->setSeq(seq);
    return payload;
}

TimeoutAppCo* MessageFactory::createTimeoutAppCo(unsigned int seq, unsigned int id_h, unsigned int Ses) {
    TimeoutAppCo* payload = new TimeoutAppCo();
    payload->setSeq(seq);
    payload->setId_h(id_h);
    payload->setSes(Ses);
    return payload;
}

TimeoutHandoff* MessageFactory::createTimeoutHandoff(unsigned int idMH,
                                                     unsigned int Ses,
                                                     unsigned int SesLC,
                                                     unsigned int seq_S) {
    TimeoutHandoff* payload = new TimeoutHandoff();
    payload->setIdMH(idMH);
    payload->setSes(Ses);
    payload->setSesLC(SesLC);
    payload->setSeq_S(seq_S);
    return payload;
}
