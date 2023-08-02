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

#include "StationGroup.h"

StationGroup::StationGroup()
{
}

StationGroup::~StationGroup()
{
}

unsigned int StationGroup::size() const
{
	return group.size();
}

unsigned int& StationGroup::operator[](unsigned int index)
{
	return group[index];
}

void StationGroup::initialize(unsigned int idMSS, unsigned int groupSize)
{
	for (unsigned int i = idMSS - (idMSS % groupSize); i < idMSS - (idMSS % groupSize) + groupSize; i++)
		group.push_back(i);
	groupResponsible = group[0];
	lastMsgMSSb.resize(groupSize, 0);
	mssUp.resize(groupSize, true);
}

void StationGroup::print()
{
	for (unsigned int i : group)
		cout << i << " ";
	cout << endl;
}

bool StationGroup::contains(unsigned int idMSS)
{
	return std::find(group.begin(), group.end(), idMSS) != group.end();
}

bool StationGroup::isGroupResponsible(unsigned int idMSS)
{
	return idMSS == groupResponsible;
}

unsigned int StationGroup::getGroupResponsible()
{
	return groupResponsible;
}

void StationGroup::setGroupResponsible(unsigned int newResponsible)
{
	groupResponsible = newResponsible;
}

void StationGroup::updateLastMsgMSSb(unsigned int id)
{
	for (unsigned int i = 0; i < group.size(); i++)
	{
		if (group[i] == id)
		{
			lastMsgMSSb[i] = simTime();
			mssUp[i] = true;
		}
	}
}

void StationGroup::setLastMsgMSSbCurrentTime()
{
	std::fill(lastMsgMSSb.begin(), lastMsgMSSb.end(), simTime());
}

simtime_t StationGroup::getLastMessageTime(unsigned int source)
{
	return lastMsgMSSb[source];
}
