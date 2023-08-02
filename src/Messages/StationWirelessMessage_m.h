//
// Generated file, do not edit! Created by nedtool 5.5 from Messages/StationWirelessMessage.msg.
//

#ifndef __INET_STATIONWIRELESSMESSAGE_M_H
#define __INET_STATIONWIRELESSMESSAGE_M_H

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

class StationWirelessMessage;
} // namespace inet

#include "inet/common/INETDefs_m.h" // import inet.common.INETDefs

#include "inet/common/packet/chunk/Chunk_m.h" // import inet.common.packet.chunk.Chunk


namespace inet {

/**
 * Class generated from <tt>Messages/StationWirelessMessage.msg:20</tt> by nedtool.
 * <pre>
 * class StationWirelessMessage extends FieldsChunk
 * {
 *     unsigned int cellId;
 * }
 * </pre>
 */
class StationWirelessMessage : public ::inet::FieldsChunk
{
  protected:
    unsigned int cellId = 0;

  private:
    void copy(const StationWirelessMessage& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const StationWirelessMessage&);

  public:
    StationWirelessMessage();
    StationWirelessMessage(const StationWirelessMessage& other);
    virtual ~StationWirelessMessage();
    StationWirelessMessage& operator=(const StationWirelessMessage& other);
    virtual StationWirelessMessage *dup() const override {return new StationWirelessMessage(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getCellId() const;
    virtual void setCellId(unsigned int cellId);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const StationWirelessMessage& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, StationWirelessMessage& obj) {obj.parsimUnpack(b);}

} // namespace inet

#endif // ifndef __INET_STATIONWIRELESSMESSAGE_M_H

