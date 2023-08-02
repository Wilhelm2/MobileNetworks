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

#ifndef MOBILESUPPORTSTATIONS_STATIONAPPCOSENDINGBUFFER_H_
#define MOBILESUPPORTSTATIONS_STATIONAPPCOSENDINGBUFFER_H_

#include "StationSendingBuffer.h"

// Contains the messages a host that connects to a station must deliver before delivering messages currently broadcasted
// by the station
class StationAppCoSendingBuffer {
   public:
    StationAppCoSendingBuffer();
    virtual ~StationAppCoSendingBuffer();
    // Returns the size of the buffer (ie the number of messages in the buffer)
    size_t size() const;
    // Returns the first element of the buffer without removing it from the buffer
    struct s_sendMsgMSS front() const;
    // Returns the last element of the buffer without removing it
    struct s_sendMsgMSS back() const;
    // Returns the last element of the buffer without removing it
    struct s_sendMsgMSS back();
    // Returns whether the buffer is empty or not
    bool empty() const;
    // Returns whether idMessage is contained in the buffer or not
    bool contains(idMsg idMessage) const;

    // Removes the messages from the buffer whose sequence numbre is lower than minSeq, and returns the list of removed
    // messages
    list<struct s_sendMsgMSS> clear(unsigned int minSeq);
    // Removes all messages from the buffer and returns them
    list<struct s_sendMsgMSS> clear();
    // Sets all timeouts to nullptr
    void nullifyTimeouts();

    // Searches the message of sequence number seq in the buffer
    list<struct s_sendMsgMSS>::iterator searchMsgIt(unsigned int seq);
    // Searches the message of id idMessage in the buffer
    list<struct s_sendMsgMSS>::iterator searchMsgIt(idMsg idMessage);
    // Updates the seqC of the buffer's messages in increasing order with the first message beginning with seqC=1
    void updateSeq();
    // Returns the ids of the buffer's messages
    vector<idMsg> getMessageIds() const;

    // Contains the message stored in the buffer
    list<struct s_sendMsgMSS> buffer;
};

#endif /* MOBILESUPPORTSTATIONS_STATIONAPPCOSENDINGBUFFER_H_ */
