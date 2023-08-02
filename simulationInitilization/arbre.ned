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

		Stations[16]:S
		{
			@display("i=device/antennatower");
		}
		Mobiles[320]:M
     	{
     	    @display("i=device/cellphone");
     	}
     	
     	BCApp: BController
     	{
     	}
     	
        scenarioManager: ScenarioManager; 		

   	connections:
	    
Stations[0].ethg++ <--> chan <--> Stations[1].ethg++;
Stations[1].ethg++ <--> chan <--> Stations[2].ethg++;
Stations[2].ethg++ <--> chan <--> Stations[3].ethg++;
Stations[3].ethg++ <--> chan <--> Stations[0].ethg++;
Stations[4].ethg++ <--> chan <--> Stations[5].ethg++;
Stations[5].ethg++ <--> chan <--> Stations[6].ethg++;
Stations[6].ethg++ <--> chan <--> Stations[7].ethg++;
Stations[7].ethg++ <--> chan <--> Stations[4].ethg++;
Stations[0].ethg++ <--> chan <--> Stations[4].ethg++;
Stations[1].ethg++ <--> chan <--> Stations[5].ethg++;
Stations[2].ethg++ <--> chan <--> Stations[6].ethg++;
Stations[3].ethg++ <--> chan <--> Stations[7].ethg++;
Stations[8].ethg++ <--> chan <--> Stations[9].ethg++;
Stations[9].ethg++ <--> chan <--> Stations[10].ethg++;
Stations[10].ethg++ <--> chan <--> Stations[11].ethg++;
Stations[11].ethg++ <--> chan <--> Stations[8].ethg++;
Stations[12].ethg++ <--> chan <--> Stations[13].ethg++;
Stations[13].ethg++ <--> chan <--> Stations[14].ethg++;
Stations[14].ethg++ <--> chan <--> Stations[15].ethg++;
Stations[15].ethg++ <--> chan <--> Stations[12].ethg++;
Stations[8].ethg++ <--> chan <--> Stations[12].ethg++;
Stations[9].ethg++ <--> chan <--> Stations[13].ethg++;
Stations[10].ethg++ <--> chan <--> Stations[14].ethg++;
Stations[11].ethg++ <--> chan <--> Stations[15].ethg++;
Stations[0].ethg++ <--> chan <--> Stations[8].ethg++;
Stations[1].ethg++ <--> chan <--> Stations[9].ethg++;
Stations[2].ethg++ <--> chan <--> Stations[10].ethg++;
Stations[3].ethg++ <--> chan <--> Stations[11].ethg++;
Stations[4].ethg++ <--> chan <--> Stations[12].ethg++;
Stations[5].ethg++ <--> chan <--> Stations[13].ethg++;
Stations[6].ethg++ <--> chan <--> Stations[14].ethg++;
Stations[7].ethg++ <--> chan <--> Stations[15].ethg++;
}