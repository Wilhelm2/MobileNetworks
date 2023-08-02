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

#include "LinearMobilityInsideCells.h"
#include "inet/common/INETMath.h"

LinearMobilityInsideCells::LinearMobilityInsideCells() {
    speed = 0;
}

LinearMobilityInsideCells::~LinearMobilityInsideCells() {}

void LinearMobilityInsideCells::initialize(int stage) {
    MovingMobilityBase::initialize(stage);

    EV_TRACE << "initializing LinearMobility stage " << stage << endl;
    if (stage == INITSTAGE_LOCAL) {
        speed = par("speed");
        stationary = (speed == 0);
        rad heading = deg(fmod(par("initialMovementHeading").doubleValue(), 360));
        rad elevation = deg(fmod(par("initialMovementElevation").doubleValue(), 360));
        Coord direction = Quaternion(EulerAngles(heading, -elevation, rad(0))).rotate(Coord::X_AXIS);

        lastVelocity = direction * speed;
        directionUpdatePeriod = SimTime(5, SIMTIME_S);
        params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGE_NAME) + ".simParams").c_str()));
    }
}

bool LinearMobilityInsideCells::OutOfZone(Coord& velocity, double speed) {
    Coord MH = lastPosition;
    Coord ClosestStation = getStationCoordinates("Stations[0]");
    double minDist = computeDistance(MH, ClosestStation);
    double distance = 0;

    for (unsigned int i = 1; i < params->nbMobileSupportStations;
         i++)  // calcule la distance avec tous les MSS et garde le plus petit
    {
        distance = computeDistance(MH, getStationCoordinates("Stations[" + to_string(i) + "]"));
        if (distance < minDist) {
            minDist = distance;
            ClosestStation = getStationCoordinates("Stations[" + to_string(i) + "]");
        }
    }
    if (minDist > 120) {
        const double g_Pi = 3.14159265358979323846;
        rad angle = rad(atan2(ClosestStation.y - MH.y, ClosestStation.x - MH.x) + ((rand() % 180 - 110) * g_Pi / 180));
        velocity.x = speed * cos(angle);  // | x_a - (x_a + speed*cos(angle)) |
        velocity.y = speed * sin(angle);  // | y_a - (y_a + speed*sin(angle)) |
        return true;
    }
    return false;
}

Coord LinearMobilityInsideCells::getStationCoordinates(string name) {
    cModule* mod = getModuleByPath(name.c_str());
    LinearMobility* mob = dynamic_cast<LinearMobility*>(mod->getSubmodule("mobility"));
    return mob->getCurrentPosition();
}

double LinearMobilityInsideCells::computeDistance(Coord c1, Coord c2) {
    double hdistance = c1.x - c2.x;
    double vdistance = c1.y - c2.y;
    return sqrt(hdistance * hdistance + vdistance * vdistance);
}

void LinearMobilityInsideCells::orient() {
    // udpates directly in move to take into account the new orientation in the new position
}

void LinearMobilityInsideCells::move() {
    double elapsedTime = (simTime() - lastUpdate).dbl();
    lastPosition += lastVelocity * elapsedTime;
    Coord added = lastVelocity * elapsedTime;

    if (directionUpdatePeriod < (simTime() - lastDirectionUpdate))  // change direction every 5 seconds
    {
        rad angle = rad((((double)rand() / (RAND_MAX)) + 1) * 2 * M_PI);
        lastVelocity.x = speed * cos(angle);  // | x_a - (x_a + speed*cos(angle)) |
        lastVelocity.y = speed * sin(angle);  // | y_a - (y_a + speed*sin(angle)) |
        lastDirectionUpdate = simTime();
    }

    if (OutOfZone(lastVelocity, speed)) {
        lastPosition -= added;
        lastPosition += lastVelocity * elapsedTime;
        std::cerr << simTime() << " OUT OF ZONE" << lastPosition.x << "," << lastPosition.y << ", CHANGE DiRECTION "
                  << getOwner()->getFullName() << " NEW DIRECTION " << lastVelocity << endl;
    }
}
