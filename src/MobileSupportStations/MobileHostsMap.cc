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

#include "MobileHostsMap.h"

MobileHostsMap::MobileHostsMap()
{
}

MobileHostsMap::~MobileHostsMap()
{
}

void MobileHostsMap::addSession(Session Ses, unsigned int idMH)
{
	if (!isRegistered(idMH))
	{
		Mobiles[idMH] =
		{};
	}
	Mobiles[idMH].push_back(Ses);
}

void MobileHostsMap::registerHost(Session mh)
{
	mh.getCoMsgDeliver().nullifyTimeouts();
	if (isSessionRegistered(mh.id, mh.Ses))
		removeSession(mh.id, mh.Ses);
	addSession(mh, mh.id);
}

void MobileHostsMap::removeSession(unsigned int idMH, unsigned int Ses)
{
	if (isRegistered(idMH)) // session might already be removed
	{
		map<unsigned int, vector<Session> >::iterator mh = Mobiles.find(idMH);
		for (vector<Session>::iterator session = mh->second.begin(); session != mh->second.end(); session++)
		{
			if (session->Ses == Ses)
			{
				mh->second.erase(session);
				break;
			}
		}
		if (mh->second.empty())
			Mobiles.erase(mh);
	}
}

vector<cMessage*> MobileHostsMap::deleteSessions(unsigned int idMH, unsigned int Ses)
{
	vector<cMessage*> timeoutsToErase;
	if (isRegistered(idMH))
	{
		for (vector<Session>::iterator it = Mobiles[idMH].begin(); it != Mobiles[idMH].end();
		/*in loop due to erase*/)
		{
			if (it->Ses < Ses) // delete this Session
			{
				if (it->handoffTimeout)
					timeoutsToErase.push_back(it->handoffTimeout);
				it = Mobiles[idMH].erase(it);
			}
			else
				it++;
		}
		if (Mobiles[idMH].empty())
			Mobiles.erase(idMH);
	}
	return timeoutsToErase;
}

vector<Session>::iterator MobileHostsMap::findSession(unsigned int idMH, unsigned int Ses)
{
	assert(isRegistered(idMH));
	auto mh = Mobiles.find(idMH);
	vector<Session>::iterator it;
	for (it = mh->second.begin(); it != mh->second.end(); it++)
	{
		if (it->Ses == Ses)
			break;
	}
	return it;
}

vector<Session>::iterator MobileHostsMap::getSessionIfNotInHandoff(unsigned int idMH)
{
	auto mh = Mobiles.find(idMH);
	vector<Session>::iterator it;
	for (it = mh->second.begin(); it != mh->second.end(); it++)
	{
		if (!it->isInHandoff())
			return it;
	}
	return it;
}

vector<Session>::iterator MobileHostsMap::getSessionIfRecvReq1(unsigned int idMH)
{
	auto mh = Mobiles.find(idMH);
	vector<Session>::iterator it;
	for (it = mh->second.begin(); it != mh->second.end(); it++)
	{
		if (it->state == Session::CONNECTION_STATE::Rcv_Req1)
			return it;
	}
	return it;
}

vector<Session>::iterator MobileHostsMap::getSessionInCharge(unsigned int idMH, unsigned int idMSS)
{
	auto mh = Mobiles.find(idMH);
	vector<Session>::iterator it;
	for (it = mh->second.begin(); it != mh->second.end(); it++)
	{
		if (it->MSSinCharge == idMSS)
			return it;
	}
	return it;
}

bool MobileHostsMap::isRegistered(unsigned int idMH)
{
	return Mobiles.find(idMH) != Mobiles.end();
}

bool MobileHostsMap::isSessionRegistered(unsigned int idMH, unsigned int Ses)
{
	if (isRegistered(idMH))
	{
		for (const Session& it : Mobiles[idMH])
		{
			if (it.Ses == Ses)
				return true;
		}
	}
	return false;
}

bool MobileHostsMap::hasSessionNotInHadoff(unsigned int idMH)
{
	if (isRegistered(idMH))
	{
		for (const Session& it : Mobiles[idMH])
		{
			if (!it.isInHandoff())
				return true;
		}
	}
	return false;
}

bool MobileHostsMap::hasSessionRecvReq1(unsigned int idMH)
{
	if (isRegistered(idMH))
	{
		for (const Session& it : Mobiles[idMH])
		{
			if (it.state == Session::CONNECTION_STATE::Rcv_Req1)
				return true;
		}
	}
	return false;
}

bool MobileHostsMap::hasSessionInCharge(unsigned int idMH, unsigned int idMSS)
{
	if (isRegistered(idMH))
	{
		for (const Session& it : Mobiles[idMH])
		{
			if (it.MSSinCharge == idMSS)
				return true;
		}
	}
	return false;
}

list<struct s_sendMsgMSS> MobileHostsMap::acknowledgeAppCo(unsigned int idMH, unsigned int Ses, unsigned int seqC)
{
	if (isSessionRegistered(idMH, Ses))
	{
		vector<Session>::iterator session = findSession(idMH, Ses);
		session->seqc = seqC;
		return session->getCoMsgDeliver().clear(seqC);
	}
	return
	{};
}

void MobileHostsMap::increasesAckh(unsigned int idMH)
{
	if (isRegistered(idMH))
	{
		for (Session& it : Mobiles[idMH])
			it.seqh++;
	}
}

unsigned int MobileHostsMap::nbHostsInCell(unsigned int idMSS)
{
	unsigned int hostsInCell = 0;
	for (auto mh : Mobiles)
	{
		for (const Session& connection : mh.second)
		{
			if (connection.MSSinCharge == idMSS)
				hostsInCell++;
		}
	}
	return hostsInCell;
}

unsigned int MobileHostsMap::searchMinSeq()
{
	unsigned int minVal = (Mobiles.empty() ? 0 : Mobiles.begin()->second.begin()->seqc);
	for (const auto& mh : Mobiles)
	{
		for (const Session& session : mh.second)
			minVal = min(session.seqc, minVal);
	}
	return minVal;
}

vector<unsigned int> MobileHostsMap::getMinSeqCRegisteredMH(unsigned int idMSS)
{
	vector<unsigned int> minMH;
	unsigned int localMin;
	for (const auto& mh : Mobiles)
	{
		localMin = mh.second.begin()->seqc;
		for (Session connection : mh.second)
		{
			if (connection.MSSinCharge == idMSS)
				localMin = min(localMin, connection.seqc);
		}
		minMH.push_back(localMin);
	}
	return minMH;
}

list<struct s_sendMsgMSS> MobileHostsMap::clear()
{
	list<struct s_sendMsgMSS> timeoutsToCancel;
	list<struct s_sendMsgMSS> tmp;
	for (const auto& mh : Mobiles)
	{
		for (Session connection : mh.second)
		{
			tmp = connection.getCoMsgDeliver().clear();
			timeoutsToCancel.insert(timeoutsToCancel.end(), tmp.begin(), tmp.end());
		}
	}
	Mobiles.clear();
	return timeoutsToCancel;
}

bool MobileHostsMap::empty()
{
	return Mobiles.empty();
}

bool MobileHostsMap::allMHDelivered(unsigned int seq, unsigned int idMSS)
{
	bool delivered;
	for (const auto& mh : Mobiles)
	{
		for (const Session& session : mh.second)
		{
			if (session.isInCharge(idMSS)) // only broadcasts messages for mobiles in its cell
			{
				delivered = false;
				for (const AckInterval& ackInterval : session.ackIntervals)
				{
					if (ackInterval.contains(seq))
					{
						delivered = true;
						break;
					}
				}
				if (!delivered)
					return false;
			}
		}
	}
	return true;
}

vector<tuple<unsigned int, unsigned int, unsigned int>> MobileHostsMap::createAckSSVectorConnectedHosts(
		unsigned int idMSS)
{
	vector<tuple<unsigned int, unsigned int, unsigned int>> vseqAckSS;
	for (const auto& mh : Mobiles)
	{
		for (const Session& session : mh.second)
		{
			if (session.isInCharge(idMSS))
			{
				if (session.getCoMsgDeliver().empty())
					vseqAckSS.push_back(make_tuple(session.id, session.seqc, session.Ses));
				else
					vseqAckSS.push_back(make_tuple(session.id, session.getCoMsgDeliver().front().seqNC, session.Ses));
			}
		}
	}
	return vseqAckSS;
}

vector<idMsg> MobileHostsMap::createAckHVectorConnectedHosts(unsigned int idMSS)
{
	vector<idMsg> vseqAckH;
	for (const auto& mh : Mobiles)
	{
		for (const Session& session : mh.second)
		{
			if (session.isInCharge(idMSS))
				vseqAckH.push_back( { session.id, session.seqh });
		}
	}
	return vseqAckH;
}
