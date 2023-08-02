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

#ifndef MOBILESUPPORTSTATIONS_STATIONRECEIVEBUFFER_H_
#define MOBILESUPPORTSTATIONS_STATIONRECEIVEBUFFER_H_

#include <omnetpp.h>

#include "../Messages/Apph_S_m.h"

using namespace inet;

// Represents a reception buffer in which a station stores the messages it receives from a host
// till the messages are FIFO-ordered. A station keeps a ReceptionBuffer for each host connected to it.
class StationReceiveBuffer
{
public:
	StationReceiveBuffer();
	virtual ~StationReceiveBuffer();
	// Inserts a message in the buffer following its sequence number.
	void insert(const IntrusivePtr<const Apph_S>& m);
	/* Extracts into container the application message of sequence number seq if contained in the buffer
	 and returns true. Returns false if the message is not contained in the buffer*/
	bool tryExtract(unsigned int seq, idMsg& container);
	/* Contains the application messages received from a given host.
	 The messages are ordered following their sequence number*/
	list<idMsg> buffer;
};

#endif /* MOBILESUPPORTSTATIONS_STATIONRECEIVEBUFFER_H_ */
