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

#include "StationReceiveBuffer.h"

StationReceiveBuffer::StationReceiveBuffer()
{
}

StationReceiveBuffer::~StationReceiveBuffer()
{
}

void StationReceiveBuffer::insert(const IntrusivePtr<const Apph_S>& m)
{
	list<idMsg>::iterator it = buffer.begin();
	while (it != buffer.end() && it->seqMH < m->getId().seqMH)
		it++;
	buffer.insert(it, m->getId());
}

bool StationReceiveBuffer::tryExtract(unsigned int seq, idMsg& container)
{
	for (list<idMsg>::iterator it = buffer.begin(); it != buffer.end(); it++)
	{
		if (it->seqMH == seq)
		{
			container = *it;
			buffer.erase(it);
			return true;
		}
	}
	return false;
}
