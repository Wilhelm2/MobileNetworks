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

#ifndef MOBILEHOSTS_HOSTCOMMUNICATIONS_H_
#define MOBILEHOSTS_HOSTCOMMUNICATIONS_H_

#include "LinearMobilityInsideCells.h"
#include "inet/applications/udpapp/UdpBasicApp.h"
#include "inet/common/IntrusivePtr.h"
#include "inet/common/geometry/common/Coord.h"
#include "inet/common/packet/Packet.h"
#include "inet/linklayer/common/InterfaceTag_m.h"
#include "inet/mobility/single/LinearMobility.h"
#include "inet/networklayer/common/InterfaceTable.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/networklayer/contract/IInterfaceTable.h"

#include "../commun_f.h"
using namespace omnetpp;
using namespace inet;

struct statsMH {
    unsigned int broadcastedMessagesSinceLastMeasure =
        0;  // Number of messages the host broadcasted since the last measure of the Stats module
    unsigned int totalDelivered = 0;             // Total number of messages the host delivered
    unsigned int theoreticalNbWifiMessages = 0;  // Total number of messages the host sent (through the sendMessage()
                                                 // function, knowing that sent messages might be grouped by TCP/UDP)
    unsigned int effectiveNbWifiMessages = 0;    // Total number of messages that the host effectively sent (ie when
                                                 // taking into account the grouping of messages)
    unsigned int receivedMessages =
        0;  // Total number of received messages. Also takes into account messages received multiple times
};

// Base class for hosts. Handles the communication of hosts
class HostCommunications : public inet::UdpBasicApp {
   public:
    HostCommunications();
    virtual ~HostCommunications();
    virtual void initialize(int stage);

    // Sends packet packet to the station of address addr
    void sendPacket(Packet* packet, L3Address addr);
    // Gets the address of the station stationId
    L3Address getMSSAdress(unsigned int stationId);
    // Checks the distance to each station and returns the id of the station that is the closest in distance to the host
    unsigned int computeClosestMSS(unsigned int idMobileHost, unsigned int nbMobileSupportStations);
    // Gets coordinates of the module whose name is given as parameter
    template <typename MobilityModel>
    Coord getCoordinates(const string& name);
    // Computes the distance between two coordinates
    double computeDistance(const Coord& c1, const Coord& c2);

    // id of the wlan interface kept to avoid looking it up at every send
    unsigned int wlanInterfaceId;
    // Contains the stats of the host communications
    struct statsMH stats;
    // Contains the simulation parameters
    SimulationParameters* params;
};

#endif /* MOBILEHOSTS_HOSTCOMMUNICATIONS_H_ */
