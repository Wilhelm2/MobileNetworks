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
import FIFOCom; 

cplusplus{{  
    #include "../commun_f.h"
}} 
namespace inet;


cplusplus{{
    
    #include <vector> 
    typedef idMsg idMsgAppS_S;
}}

class idMsgAppS_S {
    @existingClass;
}

class AppS_S extends FIFOCom
{
    idMsgAppS_S id;
    unsigned int id_S; // pour la simulation pour reconnaître la station source 
    unsigned int Ses;
}
