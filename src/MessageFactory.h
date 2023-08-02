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

#ifndef MESSAGEFACTORY_H_
#define MESSAGEFACTORY_H_

#include "Messages/AckSS_m.h"
#include "Messages/Ack_S_m.h"
#include "Messages/Ack_h_m.h"
#include "Messages/AlgoTimeout_m.h"
#include "Messages/AppCoAck_m.h"
#include "Messages/AppCo_m.h"
#include "Messages/AppS_S_m.h"
#include "Messages/AppS_h_m.h"
#include "Messages/App_resp_m.h"
#include "Messages/Apph_S_m.h"
#include "Messages/Delete_m.h"
#include "Messages/FIFOCom_m.h"
#include "Messages/HostMessage_m.h"
#include "Messages/InitAck_m.h"
#include "Messages/Init_m.h"
#include "Messages/RecoverMSSReq_m.h"
#include "Messages/RecoverMSS_m.h"
#include "Messages/Register_m.h"
#include "Messages/Req1_m.h"
#include "Messages/Req2_m.h"
#include "Messages/Rsp1_m.h"
#include "Messages/Rsp2_m.h"
#include "Messages/StationWirelessMessage_m.h"
#include "Messages/TimeoutAppCo_m.h"
#include "Messages/TimeoutAppS_h_m.h"
#include "Messages/TimeoutApph_S_m.h"
#include "Messages/timeoutHandoff_m.h"

#include "inet/common/INETDefs.h"
#include "inet/common/IntrusivePtr.h"

#include "MobileHosts/MHState.h"
#include "MobileSupportStations/MSSSBuffer.h"
#include "MobileSupportStations/Session.h"
#include "MobileSupportStations/StationGroup.h"
#include "commun_f.h"

using namespace omnetpp;
using namespace inet;

// Class used to create messages
class MessageFactory {
   public:
    MessageFactory();

    // Messages used to disseminate application messages
    static IntrusivePtr<Apph_S> createApph_S(idMsg id, unsigned int id_S, unsigned int Ses);
    static IntrusivePtr<AppS_S> createAppS_S(unsigned int id_S,
                                             idMsg id,
                                             unsigned int SourceId,
                                             unsigned int SourceSeq,
                                             unsigned int Ses);
    static IntrusivePtr<App_resp> createApp_resp(idMsg id,
                                                 unsigned int s_resp,
                                                 unsigned int seqS,
                                                 unsigned int SourceId,
                                                 unsigned int SourceSeq);
    static IntrusivePtr<AppS_h> createAppS_h(unsigned int seq_S,
                                             unsigned int id_S,
                                             const vector<unsigned int>& M_d,
                                             idMsg id);

    // Messages used to send and acknowledge application messages during host connections
    // (messages a connecting host did not deliver yet but which the current station already discarded)
    static IntrusivePtr<AppCo> createAppCo(unsigned int id_S,
                                           unsigned int seq_S,
                                           unsigned int targetHostId,
                                           unsigned int Ses,
                                           idMsg id);
    static IntrusivePtr<AppCoAck> createAppCoAck(unsigned int id_h,
                                                 unsigned int seq_S,
                                                 unsigned int Ses,
                                                 unsigned int cellId);

    // Messages to acknowledge application messages sent outside host connection initialization
    static IntrusivePtr<Ack_h> createAck_h(unsigned int id_h,
                                           const vector<AckInterval>& intervals,
                                           unsigned int seq_S,
                                           unsigned int Ses,
                                           unsigned int id_S);
    static IntrusivePtr<Ack_S> createAck_S(unsigned int id_S, const vector<idMsg>& vSeq);
    static IntrusivePtr<AckSS> createAckSS(unsigned int id_S,
                                           const vector<tuple<unsigned int, unsigned int, unsigned int>>& vseqAckSS,
                                           unsigned int SourceId,
                                           unsigned int SourceSeq);

    // Messages used during the connection of hosts to stations.
    static IntrusivePtr<Init> createInit(unsigned int id_h,
                                         unsigned int seq_S,
                                         unsigned int Ses,
                                         unsigned int Ses_LC,
                                         unsigned int id_S,
                                         MHState::STATE state);
    static IntrusivePtr<Req1> createReq1(unsigned int id_h,
                                         unsigned int seq_S,
                                         unsigned int Ses_LC,
                                         unsigned int Ses,
                                         const StationGroup& MSSb,
                                         unsigned int SourceId,
                                         unsigned int SourceSeq);
    static IntrusivePtr<Rsp1> createRsp1(unsigned int id_h,
                                         unsigned int seq_h,
                                         const vector<idMsg>& m_nd,
                                         unsigned int Ses,
                                         unsigned int SourceId,
                                         unsigned int SourceSeq);
    static IntrusivePtr<Req2> createReq2(unsigned int id_h,
                                         const vector<idMsg>& msg_req,
                                         unsigned int Ses,
                                         const StationGroup& MSSb,
                                         unsigned int SourceId,
                                         unsigned int SourceSeq);
    static IntrusivePtr<Rsp2> createRsp2(unsigned int id_h,
                                         const vector<idMsg>& msg,
                                         const vector<idMsg>& msg_rcv,
                                         unsigned int Ses,
                                         unsigned int SourceId,
                                         unsigned int SourceSeq);
    static IntrusivePtr<InitAck> createInitAck(unsigned int id_S,
                                               unsigned int id_h,
                                               unsigned int seq_h,
                                               unsigned int seq_S,
                                               unsigned int Ses,
                                               const vector<tuple<idMsg, unsigned int>>& MsgOrder);
    static IntrusivePtr<Delete> createDelete(unsigned int Ses,
                                             unsigned int id_h,
                                             unsigned int SourceId,
                                             unsigned int SourceSeq);
    static IntrusivePtr<Register> createRegister(const Session& hi, unsigned int SourceId, unsigned int SourceSeq);

    // Messages used by stations to recover from failures
    static IntrusivePtr<RecoverMSS> createRecoverMSS(unsigned int MSScible,
                                                     unsigned int SourceId,
                                                     unsigned int SourceSeq,
                                                     const MSSSBuffer& SBuffer,
                                                     unsigned int seq_S,
                                                     unsigned int s_resp,
                                                     const vector<unsigned int>& received,
                                                     const map<unsigned int, vector<Session>>& Mobiles);
    static IntrusivePtr<RecoverMSSReq> createRecoverMSSReq(unsigned int SourceId,
                                                           unsigned int SourceSeq,
                                                           unsigned int recoveringMSSId);

    // Timeout messages
    static TimeoutAppS_h* createTimeoutAppS_h(unsigned int seq);
    static TimeoutApph_S* createTimeoutApph_S(unsigned int seq);
    static TimeoutAppCo* createTimeoutAppCo(unsigned int seq, unsigned int id_h, unsigned int Ses);
    static TimeoutHandoff* createTimeoutHandoff(unsigned int idMH,
                                                unsigned int Ses,
                                                unsigned int SesLC,
                                                unsigned int seq_S);
    virtual ~MessageFactory();

    static const unsigned int messageLength;  // Application message length
};

#endif /* MESSAGEFACTORY_H_ */
