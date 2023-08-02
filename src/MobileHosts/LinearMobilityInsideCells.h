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

#ifndef LINEARMOBILITYINSIDECELLS_H_
#define LINEARMOBILITYINSIDECELLS_H_

#include "inet/common/INETDefs.h"
#include "inet/mobility/base/MovingMobilityBase.h"
#include "inet/mobility/single/LinearMobility.h"

#include "../commun_f.h"
#include "../simulationParameters.h"
using namespace inet;

/**
 * @brief Linear movement model + Restrain movements in order to stay in the area covered by stations
 *
 * @ingroup mobility
 * @author Emin Ilker Cetinbas
 */
class INET_API LinearMobilityInsideCells : public MovingMobilityBase {
   protected:
    double speed;

   protected:
    virtual int numInitStages() const override { return NUM_INIT_STAGES; }

    /** @brief Initializes mobility model parameters.*/
    virtual void initialize(int stage) override;

    /** @brief Move the host*/
    virtual void move() override;
    virtual void orient() override;

    // Checks if the host is out of the area covered by stations
    bool OutOfZone(Coord& velocity, double speed);
    // Get the coordinates of the station name
    Coord getStationCoordinates(string name);
    // Computes the distance between two coordinates
    double computeDistance(Coord c1, Coord c2);

   public:
    virtual double getMaxSpeed() const override { return speed; }
    LinearMobilityInsideCells();
    virtual ~LinearMobilityInsideCells();

    // Period at which the host changes direction
    simtime_t directionUpdatePeriod;
    // Last time the host changed direction
    simtime_t lastDirectionUpdate;
    // Access to simulations parameters
    SimulationParameters* params;
};

Define_Module(LinearMobilityInsideCells);

#endif /* LINEARMOBILITYINSIDECELLS_H_ */
