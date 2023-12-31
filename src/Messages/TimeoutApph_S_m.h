//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/TimeoutApph_S.msg.
//

#ifndef __TIMEOUTAPPH_S_M_H
#define __TIMEOUTAPPH_S_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



class TimeoutApph_S;
#include "AlgoTimeout_m.h" // import AlgoTimeout

/**
 * Class generated from <tt>Messages/TimeoutApph_S.msg:17</tt> by nedtool.
 * <pre>
 * packet TimeoutApph_S extends AlgoTimeout
 * {
 *     unsigned int seq;
 * }
 * </pre>
 */
class TimeoutApph_S : public ::AlgoTimeout
{
  protected:
    unsigned int seq = 0;

  private:
    void copy(const TimeoutApph_S& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const TimeoutApph_S&);

  public:
    TimeoutApph_S(const char *name=nullptr, short kind=0);
    TimeoutApph_S(const TimeoutApph_S& other);
    virtual ~TimeoutApph_S();
    TimeoutApph_S& operator=(const TimeoutApph_S& other);
    virtual TimeoutApph_S *dup() const override {return new TimeoutApph_S(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getSeq() const;
    virtual void setSeq(unsigned int seq);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const TimeoutApph_S& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, TimeoutApph_S& obj) {obj.parsimUnpack(b);}

#endif // ifndef __TIMEOUTAPPH_S_M_H

