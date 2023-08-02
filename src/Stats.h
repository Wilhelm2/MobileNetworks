/*
 * Stats.h
 *
 *  Created on: Aug 9, 2019
 *      Author: wilhelm
 */

#ifndef STATS_H_
#define STATS_H_

#include <omnetpp.h>
#include <string.h>
#include <fstream>
#include <numeric>
#include <stdexcept>
#include <vector>
#include "inet/applications/udpapp/UdpBasicApp.h"
#include "inet/networklayer/ipv4/Ipv4.h"
#include "inet/power/storage/SimpleEpEnergyStorage.h"

#include "BroadcastController.h"
#include "MobileHosts/MH.h"
#include "MobileSupportStations/MSS.h"

using namespace inet;
using namespace std;

// Class that takes the simulation statistics, writes them to files and prints them on the terminal
class Stats : public cSimpleModule {
   public:
    Stats();
    virtual ~Stats();

    void initialize(int stage) override;
    void handleMessage(cMessage* msg) override;
    // Takes statistics
    void takeStats();

    // Returns a pointer to the station idMSS
    MSS* getMSSPointer(unsigned int idMSS);
    // Returns a pointer to the mobile idMH
    MH* getMHPointer(unsigned int idMH);
    // Returns a pointer to the Ipv4 module of the station idMSS
    Ipv4* getMSSIPv4Pointer(unsigned int idMSS);
    // Returns a pointer to the Ipv4 module of the mobile idMH
    Ipv4* getMHIPv4Pointer(unsigned int idMH);

    // Writes to file the amount of data sent by stations and mobiles
    void WriteSentAmountData();
    // Writes to file the theoretical number of messages sent over the wired an wireless network
    void WriteNbTheoricalSentMessages();
    // Writes to file the effective number of messages sent over the wired ans wireless network
    void WriteNbSentMsg();
    // Writes to file the number of broadcasted messages as well as the number of delivered messages by mobile hosts
    void WriteTotalMessages();
    // Writes to file the average time between the broadcast of messages and their delivery by mobile hosts
    void WriteAvgDeliverTime();
    // Writes to file the average collision rate of messages on the wireless network
    void WriteCollisionRate();
    // Writes to file the energy consumption of mobile hosts
    void EnergyConsumption();

    // Writes to file the number of messages in the SBuffer of mobile hosts
    void WriteSBufferMH();
    // Writes to file the number of messages in the RBuffer of mobile hosts
    void WriteRBufferMH();
    // Writes to file the seqC of all mobile hosts
    void WriteMinSeqMHFile();
    // Prints the seqC of all mobile hosts
    void PrintMHSeqC();

    // Writes to file the number of messages in the SBuffer of stations
    void SBufferMSSsize();
    // Writes to file the number of messages in AppCo for mobile hosts
    void WriteBufferCoMsg();
    // Prints for each station *s_i* the last reception time of messages from stations of *s_i*'s group
    void PrintMSSb();

    // Output streams
    std::ofstream SentAmoutDataFile;
    std::ofstream nbTheoricalSentMessages;
    std::ofstream nbSentMessagesFile;
    std::ofstream totalMessagesFile;
    std::ofstream AvgDeliverTimeFile;
    std::ofstream CollisionRateFile;
    std::ofstream energyConsumption;

    std::ofstream SBufferMHFile;
    std::ofstream RBufferMHFile;
    std::ofstream minSeqMHFile;

    std::ofstream SBufferSizeMSS;
    std::ofstream bufferCoMsgFile;

    // Total amount of forwarded data.
    unsigned int totalForwardedSize = 0;

    // Simulation parameters
    SimulationParameters* params;
};

Define_Module(Stats);
#endif /* STATS_H_ */
