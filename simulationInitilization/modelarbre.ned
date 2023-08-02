package mnwithstationfailures.simulations;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.node.inet.INetworkNode;
import inet.physicallayer.contract.packetlevel.IRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.node.inet.WirelessHost;
import inet.node.inet.StandardHost;
import inet.node.ethernet.Eth100M;
import inet.applications.udpapp.UdpBasicApp;
import inet.applications.contract.IApp;
import inet.physicallayer.contract.packetlevel.IReceiver;
import inet.common.scenario.ScenarioManager;
import inet.physicallayer.ieee80211.packetlevel.Ieee80211ScalarRadioMedium;
import inet.physicallayer.common.packetlevel.RadioMedium;
import inet.node.inet.AdhocHost;



simple BControllerApp extends UdpBasicApp
{
    parameters: 
    @class(BroadcastController);
}

module BController extends StandardHost
{
    parameters:
        @display("i=,red");
}

simple StatsApp extends UdpBasicApp
{
    parameters: 
    @class(Stats);
}

module StatsModule extends StandardHost
{
    
}

network Arbre
{
    parameters:
        @display("bgg=100,1,grey95");
    
    types:
        channel chan extends ned.DatarateChannel 
        {
            datarate=100Mbps;
            delay = 10ms;//100ms;}
        } 
    
    submodules:
		visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
            @display("p=650,650");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=650,650");
        }
        
//        radioreceiver: <default("UnitDiskReceiver")> like IReceiver {
//        }
        
//	radioMedium: <default("Ieee80211ScalarRadioMedium")> like IRadioMedium {
        radioMedium: <default("UnitDiskRadioMedium")> like IRadioMedium {
            parameters: 
            @display("p=650,650");
        }
		
		simParams: SimulationParameters
		{
			
		}
		StatsM: StatsModule
		{
			    
		}

