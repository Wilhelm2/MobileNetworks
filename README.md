# MobileNetworks

# Introduction

This project implements a causal broadcast algorithm for mobile networks in C++ on the OMNeT++ simulator by using the INET framework. 
Causal order is defined by the *happened-before* relationship introduced by Lamport in 1978 [[1]](https://amturing.acm.org/p558-lamport.pdf). 
The *happened-before* relationship orders events in distributed systems following three rules [[1]](https://amturing.acm.org/p558-lamport.pdf):

>**Happened-before relation:**
>	Considering two events *e1* and *e2*, *e1* causally precedes *e2* (*e1 -> e2*) iff:
>	1. *e1* and *e2* occur on the same process and *e1* precedes *e2* or
>	2. for a message m *e1*=send(m) and *e2*=deliver(m) or
>	3. there exists an event *e3* such that *e1->e3* and *e3->e2*.

Causal order ensures that any two causally related messages are delivered to applications respecting that order. 

>**Causal order:** 
>	Processes deliver messages while respecting the causal relation between them.
>	For any message *m* and *m'*, if *m* causally precedes *m'*, denoted *m\rightarrow m'*, then all processes deliver *m* before *m'*:\
>	<p align = "center">send(m) -> send(m') => deliver(m) -> deliver(m'). </p>

# Mobile Networks

We consider mobile networks composed of Mobile Hosts, denoted *host(s)*, and Static Support Stations, denoted *station(s)*.
Hosts and stations exclusively communicate through message passing.

Hosts are the sources and destinations of application messages, and stations ensure that hosts receive and deliver messages causally.

![Example of mobile network](readmeFigures/MobileNetworks.png?raw=true)

The features of stations are the following:
* Each station is at the center of a cell, corresponding to the area covered by its antenna's transmission range.
* Each station holds the causal information hosts connected to it require to deliver application messages in causal order.
* Stations are subject to transient and permanent failures. A faulty station stops sending, receiving, and processing messages, and looses all data. 
* To handle failures, stations are split into groups. Each station of a group *G* stores a replica of the causal information stored by the other stations of *G*. If *|G|=f+1* then *f* stations of *G* can be down simultaneously. Hence, *f* is determined by the number of simultaneous station failures of a same group that should be tolerated. 
* Stations are static, ie they do not move.
* Stations have no energy limitations and have a much higher storage and computational capacity than hosts.

The features of hosts are the following:
* A host is connected to at most one station (generally the closest one) at any given moment and communicates with the system through that station, by sending messages on the wireless network. 
* A host can join and leave the network at any moment. A host that joins the system will not deliver those application messages that the station to which it connects the first time has discarded prior to its connection. 		
* Hosts move freely inside and outside cells.
* Hosts have computational, storage, and energy limitations.
* Hosts are subject to transient and permanent failures. For example, a host is temporarily faulty until its battery is recharged, or it is permanently faulty if it has a hardware failure.  A faulty host stops sending, receiving, processing messages, and it loses all variables it stored in volatile memory. 
* Hosts have two states: *up* and *down*. Stations control the state of hosts connected to them: A station considers a host connected to it as *down* after not receiving any messages from the host for a given interval of time. Otherwise, the station considers the host as *up*.	
    
The features of the network are the following:
* The wireless network is considered unreliable due to interferences. The bandwidth of the wireless network is much lower than the bandwidth of a wired network. Hosts communicate with stations exclusively through the wireless network.  
* The wired network is composed of FIFO communication channels without message losses. Nevertheless, wired channels can fail. Moreover, stations communicate over the wired network by using the algorithm proposed by Mostéfaoui[[2]](https://dblp.org/rec/journals/corr/abs-1805-05201.html). Therefore, wired channels can be added and removed, as long as there exists a path of communication channels initialized by the algorithm[[2]](https://dblp.org/rec/journals/corr/abs-1805-05201.html).  

# Principle of the algorithm

Friedman and Manor [[3]](https://www.researchgate.net/publication/228826549_Causal_Ordering_in_Deterministic_Overlay_Networks) first proposed an algorithm that ensures causal order through flooding in a static overlay network composed of FIFO communication channels. Flooding through FIFO channels ensures that no path exists over which messages travel out of causal order. For example, in Figure 3.3 A broadcasts m, which causally precedes m’ broadcasted by B. All processes receive m before m’ since upon reception all processes forward m on all their communication channels. Therefore, processes deliver messages upon reception, since they receive messages already causally ordered.

<p align="center">
    <img src= "readmeFigures/FloodingCB.png?raw=true">
</p>

However, flooding over FIFO channels without additional control mechanism cannot ensure causal broadcast in networks in which communication channels can be added. 
In fact, adding a new communication channel creates a shortcut over which processes
can temporarily receive messages out of causal order.

For example, in Figure 3.4, process A first broadcast a message m. After broadcasting m, it adds a new communication channel with C, then broadcasts m 0 . The path A − C is then a shortcut that m 0 can take, but shouldn’t till C receives and delivers m. The algorithms of does therefore not work in Mobile Networks. 

<p align="center">
    <img src= "readmeFigures/FloodingDynamic.png?raw=true">
</p>

Wilhelm and al.[[4]](https://dl.acm.org/doi/10.1145/3491003.3491010) proposed a causal broadcast algorithm for mobile networks based on flooding through FIFO channels. The proposed algorithm also takes into account the limited capacities of mobile networks. This project implements that algorithm. The main features of the implementation are:

* Broadcast of application messages through flooding.
* Handle host mobility between cells through a handoff procedure.
* Most of the causal information is handled at stations to unload mobile hosts.
* Implementation of host dynamics, ie hosts can join and leave the network during execution.
* Decentralized deletion mechanism of obsolete messages by stations.

The algorithm is not explained here in further details because it is quite difficult to understand. A detailed description can be found here [[4]](https://dl.acm.org/doi/10.1145/3491003.3491010).

# OMNeT++

The project was implemented on the version 5.6.1 of the OMNeT++ simulator [[5]](https://omnetpp.org/). 
This section gives a brief description of OMNeT++, which is an event based simulator primarly used for building network simulators. 
It is organized into modules that are combined in order to form a network. 

OMNeT++ uses the following specific files:
1. **Initialization file:** Contains the simulation parameters. Has the extension *.ini*.
2. **Network files:** Define modules as well as the simulation network which is composed of modules. Have the extension *.ned*.
3. **Message files:** Define the messages used by nodes. Have the extension *.msg*.


# User guide 

This section first explains how to install the required resources to execute the project. Then it shows how to import the project in the OMNeT++ built-in IDE based on Eclipse. Finally, it describes how to execute the simulation on the terminal.

## OMNeT++ installation 

The installation of the version 5.6.1 of OMNeT++ is explained [here](https://doc.omnetpp.org/omnetpp5/InstallGuide.pdf).
Note that OMNeT++ is not (even though mostly) retrocompatible. 
Hence, some modifications might be required when using another version of OMNeT++. 
Thus you should carefully install the version 5.6.1 of OMNeT++.

## Import project in OMNeT++ IDE

OMNeT++ comes with a built-in IDE based on Eclipse. This section explains how to import the project in that IDE and compile it. 

First, launch the OMNeT++ IDE with the command:
> omnetpp

Second, see [here](http://wnss.sv.cmu.edu/teaching/14814/s15/files/hw2addendum.pdf) a guide written by Brian Ricks on how to import existing code within the Ecplise IDE.

Third, the compilation of .msg files requires to add: in Project Explorer (left bar):
> left clock on the project -> Properties -> OMNeT++ -> Makemake -> Options -> Custom -> add MSGC:=*(MSGC) --msg6 -> OK -> Apply and close.

## How to run the simulation 

The simulation can be launched from the IDE as well as from the terminal.

To launch it from the IDE, click on the *Debug* or *Run* buttons in the bar under the *Menu* bar. 

To launch the simulation from the terminal:
1. Open a terminal in the project directory
2. Configure the files *omnetpp.ini* and *network.ned* following the desired simulation settings (number of nodes, control mechanism,...) 
3. Execute the command: 
>../out/gcc-release/src/ErrorDetectors -f omnetpp.ini -u Cmdenv 

The python script 'execution.py' is also provided. It builds the *.ini* and *.ned* files, launches the simulation, copies the simulation statistics in folders, and builds several graphs with those statistics.
Configure the simulation parameters by modifying 'execution.py'. 

The simulation parameter 'deliveryOption' determines the control mechanism used to deliver messages. The available control mechanisms are: 
1. **No control** Nodes deliver messages upon reception without any control. 
2. **Probabilistic clocks:** Nodes use Probabilistic clocks to causally order messages.
3. **Probabilistic clock and the error detector of Mostéfaoui and Weiss:** Nodes use Probabilistic clocks to deliver messages, and execute the error detector proposed by Mostéfaoui and Weiss  [[3]](https://hal.science/hal-02056349/document) on messages before delivering them.
4. **Probabilistic clock and an hash-based error detector using an average propagation delay hypothesis:** 
Nodes use Probabilistic clocks to causally order messages, and execute an hash-based error detector proposed by Wilhelm and al.[[4]](https://hal.science/hal-03984499) on on messages before delivering them. The hash-based error detector uses the average propagation delay of messages to determine their causal relationship. 
5. **Probabilistic clock and an hash-based error detector using the clock difference between messages:** Nodes use Probabilistic clocks to causally order messages, and execute an hash-based error detector proposed by Wilhelm and al.[[4]](https://hal.science/hal-03984499) on on messages before delivering them. The hash-based error detector uses the clock difference between messages to determine their causal relationship. 
6. **Probabilistic clock, an hash-based error detector using the clock difference between messages, and the recovery of messages tagged as not causally ordered:**
Nodes use Probabilistic clocks to causally order messages, and execute an hash-based error detector proposed by Wilhelm and al.[[4]](https://hal.science/hal-03984499) on on messages before delivering them. The hash-based error detector uses the average propagation delay of messages to determine their causal relationship. Moreover, nodes 
recover the causal dependencies of the messages *m* for that the error detector tags as not causally ordered, by requesting the causal dependencies of *m* to the node that broadcasted *m*. Such a message is then delivered once the node delivered all of the message's causal dependencies.
7. **Probabilistic clock, an hash-based error detector using the clock difference between messages, the recovery of messages tagged as not causally ordered, and a recovery test to avoid requesting the causal dependencies of messages:** Additionally to **6**, before requesting the causal dependencies of a message *m*, a node first tries to find the dependency set of *m* with messages that it did not deliver yet. 

## Data analysis

Statistics about the simulation are collected and written in the *simulations/data* folder. 

The folder *Graphs* contains python scripts to build graphs from those statistics. They are mainly written to be used from the 'execution.py' script, but can also be used outside of it. 

## Documentation

Detailled documentation can be found in the documentation folder, which is providing documentation under the PDF and html format. 
Documentation has been written with [Doxygen](https://www.doxygen.nl/).

# License

This project is distributed under the license GNU 3.0. 

# References

<a id="HappenedBefore">[1]</a> Time, Clocks, and the Ordering of Events in a Distributed System. Lamport, Leslie. Communications of the ACM 1978.

<a id="PC1">[2]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. PaCT 2017.

<a id="PC2">[3]</a> Probabilistic Causal Message Ordering. Mostéfaoui, Achour and Weiss, Stéphane. Technical report 2017.

<a id="PC">[4]</a> A probabilistic Dynamic Clock Set to capture message causality. Wilhelm, Daniel and Arantes, Luciana and Sens, Pierre. Technical report 2017.

<a id="OMNeT++">[5]</a> The OMNET++ Discrete Event Simulation System. Varga, Andras. ESM 2001.
