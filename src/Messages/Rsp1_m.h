//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/Rsp1.msg.
//

#ifndef __INET_RSP1_M_H
#define __INET_RSP1_M_H

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

class Rsp1;
} // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk

#include "FIFOCom_m.h" // import FIFOCom

// cplusplus {{
  
    #include "../commun_f.h"
// }}


namespace inet {

// cplusplus {{
    
    #include <vector> 
    typedef std::vector<idMsg> IdMsgVectorRsp1;
// }}

/**
 * Class generated from <tt>Messages/Rsp1.msg:39</tt> by nedtool.
 * <pre>
 * class Rsp1 extends FIFOCom
 * {
 *     unsigned int id_h;
 *     unsigned int seq_h;
 *     IdMsgVectorRsp1 m_nd;
 *     unsigned int Ses;
 * }
 * </pre>
 */
class Rsp1 : public ::inet::FIFOCom
{
  protected:
    unsigned int id_h = 0;
    unsigned int seq_h = 0;
    IdMsgVectorRsp1 m_nd;
    unsigned int Ses = 0;

  private:
    void copy(const Rsp1& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Rsp1&);

  public:
    Rsp1();
    Rsp1(const Rsp1& other);
    virtual ~Rsp1();
    Rsp1& operator=(const Rsp1& other);
    virtual Rsp1 *dup() const override {return new Rsp1(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getId_h() const;
    virtual void setId_h(unsigned int id_h);
    virtual unsigned int getSeq_h() const;
    virtual void setSeq_h(unsigned int seq_h);
    virtual const IdMsgVectorRsp1& getM_nd() const;
    virtual IdMsgVectorRsp1& getM_ndForUpdate() { handleChange();return const_cast<IdMsgVectorRsp1&>(const_cast<Rsp1*>(this)->getM_nd());}
    virtual void setM_nd(const IdMsgVectorRsp1& m_nd);
    virtual unsigned int getSes() const;
    virtual void setSes(unsigned int Ses);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Rsp1& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Rsp1& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef __INET_RSP1_M_H

