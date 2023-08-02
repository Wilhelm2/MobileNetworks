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

#ifndef MOBILESUPPORTSTATIONS_STATIONGROUP_H_
#define MOBILESUPPORTSTATIONS_STATIONGROUP_H_

#include "../commun_f.h"

// Represents a group of stations
class StationGroup {
   private:
    // Contains the ids of station belonging to the group of stations
    vector<unsigned int> group;
    // Id of the station responsible of the group
    unsigned int groupResponsible;
    // Contains the timestamp of the last received message from each station of the group
    vector<simtime_t> lastMsgMSSb;
    // Tracks failures of the stations of the group. mssUp[i] = true means that group[i] is up.
    vector<bool> mssUp;

   public:
    StationGroup();
    virtual ~StationGroup();
    unsigned int size() const;
    unsigned int& operator[](unsigned int index);
    // Computes MSSb of idMSS following the number of stations by group of stations
    void initialize(unsigned int idMSS, unsigned int groupSize);
    // Prints the group
    void print();
    // Returns whether the group contains idMSS or not
    bool contains(unsigned int idMSS);
    // Returns whether idMSS is the group responsible or not
    bool isGroupResponsible(unsigned int idMSS);
    // Returns the id of the group responsible
    unsigned int getGroupResponsible();
    // Sets the group responsible
    void setGroupResponsible(unsigned int newResponsible);
    // Updates the most recent time in which the station received a message from station id
    void updateLastMsgMSSb(unsigned int id);
    // Sets all entries of last received message to current time
    void setLastMsgMSSbCurrentTime();
    // Gets last time where received a message from station source
    simtime_t getLastMessageTime(unsigned int source);
};

#endif /* MOBILESUPPORTSTATIONS_STATIONGROUP_H_ */
