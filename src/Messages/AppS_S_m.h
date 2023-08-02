//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/AppS_S.msg.
//

#ifndef __INET_APPS_S_M_H
#define __INET_APPS_S_M_H

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

class AppS_S;
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
    typedef idMsg idMsgAppS_S;
// }}

/**
 * Class generated from <tt>Messages/AppS_S.msg:37</tt> by nedtool.
 * <pre>
 * class AppS_S extends FIFOCom
 * {
 *     idMsgAppS_S id;
 *     unsigned int id_S; // pour la simulation pour reconnaître la station source 
 *     unsigned int Ses;
 * }
 * </pre>
 */
class AppS_S : public ::inet::FIFOCom
{
  protected:
    idMsgAppS_S id;
    unsigned int id_S = 0;
    unsigned int Ses = 0;

  private:
    void copy(const AppS_S& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AppS_S&);

  public:
    AppS_S();
    AppS_S(const AppS_S& other);
    virtual ~AppS_S();
    AppS_S& operator=(const AppS_S& other);
    virtual AppS_S *dup() const override {return new AppS_S(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual const idMsgAppS_S& getId() const;
    virtual idMsgAppS_S& getIdForUpdate() { handleChange();return const_cast<idMsgAppS_S&>(const_cast<AppS_S*>(this)->getId());}
    virtual void setId(const idMsgAppS_S& id);
    virtual unsigned int getId_S() const;
    virtual void setId_S(unsigned int id_S);
    virtual unsigned int getSes() const;
    virtual void setSes(unsigned int Ses);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AppS_S& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AppS_S& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef __INET_APPS_S_M_H

