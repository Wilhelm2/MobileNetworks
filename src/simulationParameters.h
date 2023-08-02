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

#ifndef __MNWITHSTATIONFAILURES_SIMULATIONPARAMETERS_H_
#define __MNWITHSTATIONFAILURES_SIMULATIONPARAMETERS_H_

#include <omnetpp.h>
#include <random>

using namespace omnetpp;

/**
 * Class containing the simulation parameters. Other classes access to the simulation parameter through this class.
 */
class SimulationParameters : public cSimpleModule {
   protected:
    virtual void initialize();
    virtual void handleMessage(cMessage* msg){};

   public:
    simtime_t determineFirstSendTimeInMs(unsigned int id);
    simtime_t getRandTime(simtime_t interval);
    simtime_t checkPositif(simtime_t t);

    // Number of mobile hosts inside the system
    unsigned int nbMobileHosts;
    // Number of mobile support stations inside the system
    unsigned int nbMobileSupportStations;
    // Interval at which the Stats module takes and writes statistics to files
    simtime_t SaveInterval;
    // Message broadcast frequency (in seconds).
    double AppMFrequency;
    // Size of groups of stations
    const int MSSbGroupSize = 2;
    // When scheduling a message, sends it with +- variable us (used in getRandTime)
    std::normal_distribution<double> variable = std::normal_distribution<double>(0., 10000.);
    // Generator used to generate variable delays
    std::default_random_engine generatorSendDistribution;
};

#endif
