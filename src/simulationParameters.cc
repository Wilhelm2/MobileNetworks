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

#include "simulationParameters.h"

Define_Module(SimulationParameters);

void SimulationParameters::initialize() {
    nbMobileHosts = par("nbMobileHosts");
    nbMobileSupportStations = par("nbMobileSupportStations");
    SaveInterval = par("SaveInterval");
    AppMFrequency = par("AppMFrequency");
}

simtime_t SimulationParameters::determineFirstSendTimeInMs(unsigned int id) {
    float nodeWillBroadcastAt =
        AppMFrequency * ((float)id) / nbMobileHosts * 1000000;  // necessary because SimTime only takes int as argument
    std::cerr << "Node " << id << " will send first message at " << SimTime(nodeWillBroadcastAt, SIMTIME_US)
              << std::endl;
    return SimTime(nodeWillBroadcastAt, SIMTIME_US);
}

simtime_t SimulationParameters::getRandTime(simtime_t interval) {
    return interval + SimTime((variable)(generatorSendDistribution), SIMTIME_US);
}

simtime_t SimulationParameters::checkPositif(simtime_t t) {
    simtime_t tmp = SimTime(100, SIMTIME_US);
    while (t < 0)
        t += tmp;
    return t;
}
