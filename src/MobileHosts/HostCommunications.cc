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

#include "HostCommunications.h"

HostCommunications::HostCommunications()
{
}

HostCommunications::~HostCommunications()
{
}

void HostCommunications::initialize(int stage)
{
	UdpBasicApp::initialize(stage);
	if (stage == INITSTAGE_LAST)
	{
		InterfaceTable* t = dynamic_cast<InterfaceTable*>(getModuleByPath(par("interfaceTableModule")));
		wlanInterfaceId = t->getInterfaceByName("wlan0")->getInterfaceId();
	}
}

void HostCommunications::sendPacket(Packet* packet, L3Address addr)
{
	emit(packetSentSignal, packet);
	packet->addTag<InterfaceReq>()->setInterfaceId(wlanInterfaceId);
	socket.sendTo(packet, addr, destPort);
	numSent++;
	stats.effectiveNbWifiMessages++;
}

L3Address HostCommunications::getMSSAdress(unsigned int stationId)
{
	L3Address result;
	L3AddressResolver().tryResolve(("Stations[" + to_string(stationId) + "]").c_str(), result);
	if (result.isUnspecified())
		EV_ERROR << "cannot resolve destination address : " << "Stations[" + to_string(stationId) + "]" << endl;
	return result;
}

unsigned int HostCommunications::computeClosestMSS(unsigned int idMobileHost, unsigned int nbMobileSupportStations)
{
	Coord MH = getCoordinates<LinearMobilityInsideCells>("Mobiles[" + to_string(idMobileHost) + "]");
	double minDistance = computeDistance(MH, getCoordinates<LinearMobility>("Stations[0]"));
	unsigned int minId = 0;
	double distance = 0;
	for (unsigned int i = 1; i < nbMobileSupportStations; i++)
	{
		distance = computeDistance(MH, getCoordinates<LinearMobility>("Stations[" + to_string(i) + "]"));
		if (distance < minDistance)
		{
			minDistance = distance;
			minId = i;
		}
	}
	if (minDistance > 120)
		std::cerr << "MH " << idMobileHost << " outside any cell" << endl;
	return minId;
}

template<typename MobilityModel>
Coord HostCommunications::getCoordinates(const string& name)
{
	cModule *mod = getModuleByPath(name.c_str());
	MobilityModel *mob = dynamic_cast<MobilityModel*>(mod->getSubmodule("mobility"));
	return mob->getCurrentPosition();
}

double HostCommunications::computeDistance(const Coord& c1, const Coord& c2)
{
	double hdistance = c1.x - c2.x;
	double vdistance = c1.y - c2.y;
	return sqrt(hdistance * hdistance + vdistance * vdistance);
}
