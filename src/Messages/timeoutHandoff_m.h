//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/timeoutHandoff.msg.
//

#ifndef __TIMEOUTHANDOFF_M_H
#define __TIMEOUTHANDOFF_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



class TimeoutHandoff;
#include "AlgoTimeout_m.h" // import AlgoTimeout

/**
 * Class generated from <tt>Messages/timeoutHandoff.msg:18</tt> by nedtool.
 * <pre>
 * packet TimeoutHandoff extends AlgoTimeout
 * {
 *     unsigned int idMH;
 *     unsigned int Ses;
 *     unsigned int seq_S;
 *     unsigned int SesLC;
 * }
 * </pre>
 */
class TimeoutHandoff : public ::AlgoTimeout
{
  protected:
    unsigned int idMH = 0;
    unsigned int Ses = 0;
    unsigned int seq_S = 0;
    unsigned int SesLC = 0;

  private:
    void copy(const TimeoutHandoff& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const TimeoutHandoff&);

  public:
    TimeoutHandoff(const char *name=nullptr, short kind=0);
    TimeoutHandoff(const TimeoutHandoff& other);
    virtual ~TimeoutHandoff();
    TimeoutHandoff& operator=(const TimeoutHandoff& other);
    virtual TimeoutHandoff *dup() const override {return new TimeoutHandoff(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getIdMH() const;
    virtual void setIdMH(unsigned int idMH);
    virtual unsigned int getSes() const;
    virtual void setSes(unsigned int Ses);
    virtual unsigned int getSeq_S() const;
    virtual void setSeq_S(unsigned int seq_S);
    virtual unsigned int getSesLC() const;
    virtual void setSesLC(unsigned int SesLC);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const TimeoutHandoff& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, TimeoutHandoff& obj) {obj.parsimUnpack(b);}

#endif // ifndef __TIMEOUTHANDOFF_M_H

