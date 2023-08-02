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

package mnwithstationfailures.simulations;
import inet.applications.udpapp.UdpBasicApp;
import inet.node.inet.AdhocHost;
import inet.mobility.base.MovingMobilityBase;

 
// Application part of Mobile hosts
simple MH extends UdpBasicApp
{
    parameters:
        @signal[packetSent](type=inet::Packet);
        @signal[packetReceived](type=inet::Packet);
        int InitSend = default(0);
	@class(MH);
}

// Module mobile hosts
module M extends AdhocHost 
{
    parameters:
        @display("i=block/routing"); // adds a default icon 
		@display("i=,cyan");   
	gates:
        inout gate[];
}

simple LinearMobilityInsideCells extends MovingMobilityBase
{
    parameters:
        double initialX @unit(m) = default(uniform(this.constraintAreaMinX, this.constraintAreaMaxX));
        double initialY @unit(m) = default(uniform(this.constraintAreaMinY, this.constraintAreaMaxY));
        double initialZ @unit(m) = default(nanToZero(uniform(this.constraintAreaMinZ, this.constraintAreaMaxZ)));
        bool initFromDisplayString = default(true); // enables one time initialization from the subject module's display string
        double speed @unit(mps) = default(0mps); // speed of the host
        double initialMovementHeading @unit(deg) = default(uniform(0deg, 360deg));
        double initialMovementElevation @unit(deg) = default(0deg);
        @class(LinearMobilityInsideCells);
}