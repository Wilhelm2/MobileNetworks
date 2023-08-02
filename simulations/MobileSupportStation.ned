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
import inet.applications.contract.IApp;
import inet.node.inet.INetworkNode;
import inet.node.inet.WirelessHost;

// Appllication part of the station
simple MSS like IApp
{
    parameters:
        string interfaceTableModule;   // The path to the InterfaceTable module
		int localPort= default(3000);
		int messageLength @unit(B);
        @signal[packetSent](type=inet::Packet);
        @signal[packetReceived](type=inet::Packet);
    @class(MSS);        
    gates:
        input socketIn @labels(TcpCommand/up,UdpControlInfo/up);
        output socketOut @labels(TcpCommand/down,UdpControlInfo/down);
}

// Module
module S extends WirelessHost like INetworkNode
{
    parameters:
        @display("i=block/routing"); // adds a default icon 
 	    @display("i=,gold");   
  	gates:
        inout gate[];
        
}