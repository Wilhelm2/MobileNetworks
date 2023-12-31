//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/Delete.msg.
//

#ifndef __INET_DELETE_M_H
#define __INET_DELETE_M_H

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

class Delete;
} // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk

#include "FIFOCom_m.h" // import FIFOCom


namespace inet {

/**
 * Class generated from <tt>Messages/Delete.msg:24</tt> by nedtool.
 * <pre>
 * class Delete extends FIFOCom
 * {
 *     unsigned int Ses; // numéro de session lors de laquelle a reçu ce message
 *     unsigned int id_h; 	  // identifiant du mobile 
 * }
 * </pre>
 */
class Delete : public ::inet::FIFOCom
{
  protected:
    unsigned int Ses = 0;
    unsigned int id_h = 0;

  private:
    void copy(const Delete& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Delete&);

  public:
    Delete();
    Delete(const Delete& other);
    virtual ~Delete();
    Delete& operator=(const Delete& other);
    virtual Delete *dup() const override {return new Delete(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getSes() const;
    virtual void setSes(unsigned int Ses);
    virtual unsigned int getId_h() const;
    virtual void setId_h(unsigned int id_h);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Delete& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Delete& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef __INET_DELETE_M_H

