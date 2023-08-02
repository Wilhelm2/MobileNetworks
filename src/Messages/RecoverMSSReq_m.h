//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/RecoverMSSReq.msg.
//

#ifndef __INET_RECOVERMSSREQ_M_H
#define __INET_RECOVERMSSREQ_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif


namespace inet {

class RecoverMSSReq;
} // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk

#include "FIFOCom_m.h" // import FIFOCom


namespace inet {

/**
 * Class generated from <tt>Messages/RecoverMSSReq.msg:29</tt> by nedtool.
 * <pre>
 * class RecoverMSSReq extends FIFOCom
 * {
 *     unsigned int recoveringMSSId;
 * }
 * </pre>
 */
class RecoverMSSReq : public ::inet::FIFOCom
{
  protected:
    unsigned int recoveringMSSId = 0;

  private:
    void copy(const RecoverMSSReq& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const RecoverMSSReq&);

  public:
    RecoverMSSReq();
    RecoverMSSReq(const RecoverMSSReq& other);
    virtual ~RecoverMSSReq();
    RecoverMSSReq& operator=(const RecoverMSSReq& other);
    virtual RecoverMSSReq *dup() const override {return new RecoverMSSReq(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getRecoveringMSSId() const;
    virtual void setRecoveringMSSId(unsigned int recoveringMSSId);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const RecoverMSSReq& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, RecoverMSSReq& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef __INET_RECOVERMSSREQ_M_H

