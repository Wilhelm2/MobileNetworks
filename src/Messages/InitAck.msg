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
    typedef std::vector<std::tuple<idMsg,unsigned int>> TupleVectorInitAck;
}}

class TupleVectorInitAck {
    @existingClass;
}


class InitAck extends StationWirelessMessage
{
    unsigned int id_h; // id unique du MH    
    unsigned int seq_h; // seq max des msg qui ont été diffusés par l'ancien MSS
    unsigned int seq_S; // seq des msg de l'ancien MSS
    unsigned int Ses;
    TupleVectorInitAck msgOrder;
}
