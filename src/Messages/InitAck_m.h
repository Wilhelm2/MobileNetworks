//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/InitAck.msg.
//

#ifndef __INET_INITACK_M_H
#define __INET_INITACK_M_H

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

class InitAck;
} // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk

#include "StationWirelessMessage_m.h" // import StationWirelessMessage

// cplusplus {{
  
    #include "../commun_f.h"
// }}


namespace inet {

// cplusplus {{
    
    #include <vector> 
    typedef std::vector<std::tuple<idMsg,unsigned int>> TupleVectorInitAck;
// }}

/**
 * Class generated from <tt>Messages/InitAck.msg:39</tt> by nedtool.
 * <pre>
 * class InitAck extends StationWirelessMessage
 * {
 *     unsigned int id_h; // id unique du MH    
 *     unsigned int seq_h; // seq max des msg qui ont été diffusés par l'ancien MSS
 *     unsigned int seq_S; // seq des msg de l'ancien MSS
 *     unsigned int Ses;
 *     TupleVectorInitAck msgOrder;
 * }
 * </pre>
 */
class InitAck : public ::inet::StationWirelessMessage
{
  protected:
    unsigned int id_h = 0;
    unsigned int seq_h = 0;
    unsigned int seq_S = 0;
    unsigned int Ses = 0;
    TupleVectorInitAck msgOrder;

  private:
    void copy(const InitAck& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const InitAck&);

  public:
    InitAck();
    InitAck(const InitAck& other);
    virtual ~InitAck();
    InitAck& operator=(const InitAck& other);
    virtual InitAck *dup() const override {return new InitAck(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getId_h() const;
    virtual void setId_h(unsigned int id_h);
    virtual unsigned int getSeq_h() const;
    virtual void setSeq_h(unsigned int seq_h);
    virtual unsigned int getSeq_S() const;
    virtual void setSeq_S(unsigned int seq_S);
    virtual unsigned int getSes() const;
    virtual void setSes(unsigned int Ses);
    virtual const TupleVectorInitAck& getMsgOrder() const;
    virtual TupleVectorInitAck& getMsgOrderForUpdate() { handleChange();return const_cast<TupleVectorInitAck&>(const_cast<InitAck*>(this)->getMsgOrder());}
    virtual void setMsgOrder(const TupleVectorInitAck& msgOrder);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const InitAck& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, InitAck& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef __INET_INITACK_M_H
