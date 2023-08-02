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
import inet.common.INETDefs;
import inet.common.packet.chunk.Chunk;
import StationWirelessMessage;

cplusplus{{
    
    #include "../commun_f.h"
}}

namespace inet;


cplusplus{{
    
    #include <vector> 
    typedef std::vector<idMsg> idMsgVectorAck_S;
}}

class idMsgVectorAck_S {
    @existingClass;
}
class Ack_S extends StationWirelessMessage
{
    idMsgVectorAck_S vSeq;
}
