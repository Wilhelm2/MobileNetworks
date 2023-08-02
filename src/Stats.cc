/*
 * Stats.cpp
 *
 *  Created on: Aug 9, 2019
 *      Author: wilhelm
 */

#include "Stats.h"

Stats::Stats()
{

}

Stats::~Stats()
{
}

void Stats::initialize(int stage)
{
	cSimpleModule::initialize(stage);
	params = dynamic_cast<SimulationParameters*>(getModuleByPath((string(PACKAGE_NAME) + ".simParams").c_str()));

	SentAmoutDataFile.open("../../../data/SentAmoutDataFile.dat", std::ios::out);
	nbTheoricalSentMessages.open("../../../data/nbTheoricalSentMessagesFile.dat", std::ios::out);
	nbSentMessagesFile.open("../../../data/nbSentMessagesFile.dat", std::ios::out);
	totalMessagesFile.open("../../../data/totalMessages.dat", std::ios::out);
	AvgDeliverTimeFile.open("../../../data/AvgDeliverTimeFile.dat", std::ios::out);
	CollisionRateFile.open("../../../data/CollisionPercentage.dat", std::ios::out);
	energyConsumption.open("../../../data/energyConsumption.dat", std::ios::out);

	SBufferMHFile.open("../../../data/SBufferMH.dat", std::ios::out);
	RBufferMHFile.open("../../../data/RBufferMH.dat", std::ios::out);
	minSeqMHFile.open("../../../data/minSeqMHFile.dat", std::ios::out);

	SBufferSizeMSS.open("../../../data/SBufferSizeMSS.dat", std::ios::out);
	bufferCoMsgFile.open("../../../data/bufferCoMsgFile.dat", std::ios::out);

	scheduleAt(simTime() + params->SaveInterval, new cMessage());
}

void Stats::handleMessage(cMessage *msg)
{
	takeStats();
	scheduleAt(simTime() + params->SaveInterval, msg);
}

void Stats::takeStats()
{
	WriteSentAmountData();
	WriteNbTheoricalSentMessages();
	WriteNbSentMsg();
	WriteTotalMessages();
	WriteAvgDeliverTime();
	WriteCollisionRate();
	EnergyConsumption();

	WriteSBufferMH();
	WriteRBufferMH();
	WriteMinSeqMHFile();
	PrintMHSeqC();

	SBufferMSSsize();
	WriteBufferCoMsg();

	std::cerr << "SIMULATION TIME " << simTime() << " seconds" << endl;

	if (simTime() == 200)
	{
		SentAmoutDataFile.close();
		nbTheoricalSentMessages.close();
		nbSentMessagesFile.close();
		totalMessagesFile.close();
		AvgDeliverTimeFile.close();
		CollisionRateFile.close();
		energyConsumption.close();

		SBufferMHFile.close();
		RBufferMHFile.close();
		minSeqMHFile.close();

		SBufferSizeMSS.close();
		bufferCoMsgFile.close();

		exit(0);
	}
}

MSS* Stats::getMSSPointer(unsigned int idMSS)
{
	return dynamic_cast<MSS*>(getModuleByPath(("Arbre.Stations[" + to_string(idMSS) + "].app[0]").c_str()));
}

MH* Stats::getMHPointer(unsigned int idMH)
{
	return dynamic_cast<MH*>(getModuleByPath(("Arbre.Mobiles[" + to_string(idMH) + "].app[0]").c_str()));
}

Ipv4* Stats::getMSSIPv4Pointer(unsigned int idMSS)
{
	return dynamic_cast<Ipv4*>(getModuleByPath(("Arbre.Stations[" + to_string(idMSS) + "].ipv4.ip").c_str()));
}

Ipv4* Stats::getMHIPv4Pointer(unsigned int idMH)
{
	return dynamic_cast<Ipv4*>(getModuleByPath(("Arbre.Mobiles[" + to_string(idMH) + "].ipv4.ip").c_str()));
}

void Stats::WriteSentAmountData()
{
	Ipv4* I;
	unsigned int DataTransmittedMH = 0;
	unsigned int DataTransmittedMSS = 0;

	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		I = getMSSIPv4Pointer(i);
		DataTransmittedMSS += I->totalForwardedSize;
	}
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		I = getMHIPv4Pointer(i);
		DataTransmittedMH += I->totalForwardedSize;
	}
	MSS* station;
	// DATA SIZE MSS WIFI
	unsigned int totalForwardedSizeWifiMSS = 0;
	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		station = getMSSPointer(i);
		totalForwardedSizeWifiMSS += station->stats.totalSizeWifiMessages;
	}
	SentAmoutDataFile << simTime() << " " << DataTransmittedMSS + DataTransmittedMH - totalForwardedSize << " "
			<< DataTransmittedMH + totalForwardedSizeWifiMSS << " " << DataTransmittedMSS - totalForwardedSizeWifiMSS
			<< endl;
	totalForwardedSize = DataTransmittedMSS + DataTransmittedMH;
}

void Stats::WriteNbTheoricalSentMessages()
{

	MSS* station;
	MH* mh;
	unsigned int nbWiredMessages = 0, nbWifiMSSMessages = 0, nbWifiMHMessages = 0;
	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		station = getMSSPointer(i);
		nbWiredMessages += station->stats.theoreticalNbWiredMessages;
		nbWifiMSSMessages += station->stats.theoreticalNbWifiMessages;
		station->stats.theoreticalNbWifiMessages = 0;
		station->stats.theoreticalNbWiredMessages = 0;
	}
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		mh = getMHPointer(i);
		nbWifiMHMessages += mh->stats.theoreticalNbWifiMessages;
		mh->stats.theoreticalNbWifiMessages = 0;
	}
	nbTheoricalSentMessages << simTime() << " " << nbWiredMessages + nbWifiMSSMessages + nbWifiMHMessages << " "
			<< nbWifiMHMessages + nbWifiMSSMessages << " " << nbWiredMessages << endl;
}

void Stats::WriteNbSentMsg()
{
	Ipv4* I;
	unsigned int totalMSSWireless = 0;
	unsigned int totalMSSWired = 0;
	unsigned int totalMHWireless = 0;
	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		I = getMSSIPv4Pointer(i);
		std::cerr << "MSS " << i << " nbwireless messages " << I->nbWlanPackets << " nbwired messages "
				<< I->nbWiredPackets << endl;
		totalMSSWireless += I->nbWlanPackets;
		totalMSSWired += I->nbWiredPackets;
		I->nbWlanPackets = 0;
		I->nbWiredPackets = 0;
	}
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		I = getMHIPv4Pointer(i);
		totalMHWireless += I->nbWlanPackets;
		I->nbWlanPackets = 0;
		I->nbWiredPackets = 0;
	}
	nbSentMessagesFile << simTime() << " " << (totalMSSWireless + totalMSSWired + totalMHWireless)\
 << " "
			<< totalMSSWired << " " << (totalMSSWireless + totalMHWireless) << endl;
}

void Stats::WriteTotalMessages()
{
	MH* mh;
	unsigned int totalDelivered = 0, totalSent = 0;
	totalMessagesFile << simTime() << " ";
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		mh = getMHPointer(i);
		totalDelivered += mh->stats.totalDelivered;
		totalSent += mh->stats.broadcastedMessagesSinceLastMeasure;
	}
	totalMessagesFile << totalSent << " " << totalDelivered << " " << totalSent * params->nbMobileHosts << endl;
}

void Stats::WriteAvgDeliverTime()
{
	BroadcastController *B = dynamic_cast<BroadcastController *>(getModuleByPath("Arbre.BCApp.app[0]"));
	AvgDeliverTimeFile << simTime() << " ";
	AvgDeliverTimeFile << (B->totalDelivered > 0 ? (B->totalTimeDelivered / B->totalDelivered) : 0) << endl;
	B->totalTimeDelivered = 0;
	B->totalDelivered = 0;
}

void Stats::WriteCollisionRate()
{
	MSS* station;
	MH* mh;
	unsigned int totalReceivedMH = 0;
	unsigned int totalReceivedMSS = 0;
	unsigned int totalExpectedMH = 0;
	unsigned int totalExpectedMSS = 0;
	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		station = getMSSPointer(i);
		totalReceivedMSS += station->stats.receivedMessages;
		station->stats.receivedMessages = 0;
		totalExpectedMSS += station->stats.totalExpectedReceive;
		station->stats.totalExpectedReceive = 0;
	}
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		mh = getMHPointer(i);
		totalReceivedMH += mh->stats.receivedMessages;
		mh->stats.receivedMessages = 0;
		totalExpectedMH += mh->stats.broadcastedMessagesSinceLastMeasure;
		mh->stats.broadcastedMessagesSinceLastMeasure = 0;
	}
	CollisionRateFile << simTime() << " "
			<< ((float) (totalReceivedMH + totalReceivedMSS)) / (totalExpectedMH + totalExpectedMSS) << endl;
}

void Stats::EnergyConsumption()
{
	power::SimpleEpEnergyStorage* E;
	char tmp[40];
	units::values::J consumed = units::values::J(0);
	energyConsumption << simTime() << " ";
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		sprintf(tmp, "Arbre.Mobiles[%d].energyStorage", i);
		E = dynamic_cast<power::SimpleEpEnergyStorage*>(getModuleByPath(tmp));
		consumed += E->getNominalEnergyCapacity() - E->getResidualEnergyCapacity();
	}
	energyConsumption << consumed / params->nbMobileHosts << endl;
}

void Stats::WriteSBufferMH()
{
	MH* mh;
	float avg = 0;
	SBufferMHFile << simTime() << " ";
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		mh = getMHPointer(i);
		SBufferMHFile << mh->SBuffer.buffer.size() << " ";
		avg += mh->SBuffer.buffer.size();
	}
	SBufferMHFile << (avg / params->nbMobileHosts) << endl;
}

void Stats::WriteRBufferMH()
{
	MH* mh;
	float avg = 0;
	RBufferMHFile << simTime() << " ";
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		mh = getMHPointer(i);
		avg += mh->RBuffer.size();
	}
	RBufferMHFile << (avg / params->nbMobileHosts) << endl;
}

void Stats::WriteMinSeqMHFile()
{
	MH* mh;
	minSeqMHFile << simTime() << " ";
	for (unsigned int i = 0; i < params->nbMobileHosts; i++)
	{
		mh = getMHPointer(i);
		minSeqMHFile << mh->id_h << ":" << mh->seqNC << " ";
	}
	minSeqMHFile << endl;
}

void Stats::PrintMHSeqC()
{
	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		MSS* station = getMSSPointer(i);
		for (auto elt : station->Mobiles.Mobiles)
		{
			unsigned int max = 0;
			for (Session session : elt.second)
			{
				if (max < session.seqc)
					max = session.seqc;
			}
			cerr << elt.first << "," << max << " ";
		}
		cerr << endl;
	}
}

void Stats::SBufferMSSsize()
{
	SBufferSizeMSS << simTime() << " ";
	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		MSS* station = getMSSPointer(i);
		SBufferSizeMSS << station->SBuffer.size() << " ";
		std::cerr << "MSS " << i << " : " << station->SBuffer.size() << " | ";
	}
	std::cerr << endl;
	SBufferSizeMSS << endl;
}

void Stats::WriteBufferCoMsg()
{
	MSS* station;
	unsigned int totalBufferCoMsg = 0;
	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		station = getMSSPointer(i);
		totalBufferCoMsg += station->stats.totalBufferCoMessages;
		station->stats.totalBufferCoMessages = 0;
	}
	bufferCoMsgFile << simTime() << " " << totalBufferCoMsg << endl;
}

void Stats::PrintMSSb()
{
	for (unsigned int i = 0; i < params->nbMobileSupportStations; i++)
	{
		MSS* station = getMSSPointer(i);
		for (unsigned int i = 0; i < station->MSSb.size(); i++)
			cerr << station->MSSb[i] << " : " << station->MSSb.getLastMessageTime(i) << endl;
	}
}

