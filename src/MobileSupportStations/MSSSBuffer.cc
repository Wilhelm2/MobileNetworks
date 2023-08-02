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

#include "MSSSBuffer.h"

MSSSBuffer::MSSSBuffer()
{
}

MSSSBuffer::~MSSSBuffer()
{
}

struct s_sendMsgMSS MSSSBuffer::extractPendingMsg(idMsg idMessage)
{
	return pendingMsg.extractMsg(idMessage);
}

struct s_sendMsgMSS MSSSBuffer::extractFirstPendingMsg()
{
	struct s_sendMsgMSS m = pendingMsg.front();
	pendingMsg.buffer.erase(pendingMsg.begin());
	return m;
}

void MSSSBuffer::pendingMsgPushBack(struct s_sendMsgMSS m)
{
	pendingMsg.push_back(m);
}

bool MSSSBuffer::hasPendingMessages()
{
	return pendingMsg.empty();
}

list<struct s_sendMsgMSS>::iterator MSSSBuffer::push_backBroadcastedMessages(idMsg idM, unsigned int seqS)
{
	struct s_sendMsgMSS m = extractPendingMsg(idM);
	TimeoutAppS_h* timeout = new TimeoutAppS_h();
	timeout->setSeq(seqS);

	m.seqNC = seqS;
	m.timeout = timeout;
	buffer.push_back(m);

	seqC = seqS;
	return --buffer.end();
}

struct s_sendMsgMSS MSSSBuffer::getBroadcastedMessage(unsigned int seq)
{
	return buffer.searchMsg(seq);
}

void MSSSBuffer::setTimerBroadcastedMessage(cMessage*m, unsigned int seq)
{
	buffer.setTimer(m, seq);
}

bool MSSSBuffer::broadcastedMessagesContain(unsigned int seq)
{
	return buffer.contains(seq);
}

list<struct s_sendMsgMSS>::iterator MSSSBuffer::beginStillBroadcastedMessages()
{
	list<struct s_sendMsgMSS>::iterator it = buffer.begin();
	while (it != buffer.end() && !it->timeout) // skips messages till the first that is broadcasted
		it++;
	return it;
}

vector<TimeoutAppS_h*> MSSSBuffer::resendBroadcastedMessages(unsigned int id_h)
{
	vector<TimeoutAppS_h*> timeoutsToSchedule;
	list<struct s_sendMsgMSS>::iterator it = beginStillBroadcastedMessages();
	while (it != buffer.end() && it->MHdelivContains(id_h)) // skips messages till the first that the mobile has not already delivered
		it++;

	// resends the others if they are not broadcasted (must resend them so that the mh can acknowledge them, which it must even if it has already delivered them !)
	for (; it != buffer.end(); it++)
	{
		if (!it->timeout)
		{
			TimeoutAppS_h* timeout = new TimeoutAppS_h();
			timeout->setSeq(it->seqNC);
			buffer.setTimer(timeout, it->seqNC);
			timeoutsToSchedule.push_back(timeout);
		}
	}
	return timeoutsToSchedule;
}

vector<cMessage*> MSSSBuffer::checkStopBroadcastMessages(MobileHostsMap& Mobiles,
		const vector<AckInterval>& ackIntervals, unsigned int idMSS)
{
	vector<cMessage*> timeouts;
	list<struct s_sendMsgMSS>::iterator itSBuf = buffer.begin();
	vector<AckInterval>::const_iterator itIntervals = ackIntervals.begin();
	while (itSBuf != buffer.end() && itIntervals != ackIntervals.end())
	{
		if (itIntervals->contains(itSBuf->seqNC) && Mobiles.allMHDelivered(itSBuf->seqNC, idMSS))
		{
			if (itSBuf->timeout && Mobiles.allMHDelivered(itSBuf->seqNC, idMSS))
			{
				timeouts.push_back(itSBuf->timeout);
				itSBuf->timeout = nullptr;
			}
			itSBuf++;
		}
		else
			itIntervals++;
	}
	return timeouts;
}

void MSSSBuffer::setBC(BroadcastController* BC)
{
	this->BC = BC;
}

unsigned int MSSSBuffer::getSeqC()
{
	return seqC;
}

void MSSSBuffer::incrementSeqC()
{
	seqC++;
}

bool MSSSBuffer::contains(idMsg idMessage) const
{
	return buffer.contains(idMessage) || pendingMsg.contains(idMessage);
}

vector<idMsg> MSSSBuffer::notContainedMessages(vector<idMsg> messages)
{
	vector<idMsg> notContained;
	for (const idMsg& msg : messages)
		if (!contains(msg))
			notContained.push_back(msg);
	return notContained;
}

bool MSSSBuffer::empty() const
{
	return buffer.empty();
}

size_t MSSSBuffer::size() const
{
	return buffer.size() + pendingMsg.size();
}

unsigned int MSSSBuffer::getMaxSeqNC() const
{
//    assert(!buffer.empty());
	return (buffer.empty() ? seqC : buffer.back().seqNC);
}

unsigned int MSSSBuffer::getMinSeqNC() const
{
//    assert(!buffer.empty());
	return (buffer.empty() ? seqC : buffer.front().seqNC);
}

unsigned int MSSSBuffer::getMinBroadcastedSeqNC() const
{
//    assert(!buffer.empty());
	if (buffer.empty())
		return seqC;
	for (const auto& m : buffer.buffer)
	{
		if (m.timeout)
			return m.seqNC;
	}
	return seqC;
}

vector<cMessage*> MSSSBuffer::clearSBuffer(unsigned int minSeq, unsigned int idMSS, BroadcastController *BC)
{
	return buffer.clear(minSeq, idMSS, BC);
}

vector<cMessage*> MSSSBuffer::clear()
{
	vector<cMessage*> timeoutsToCancel = buffer.clear();
	pendingMsg.clear();
	return timeoutsToCancel;
}

// determine sequence number to give to mh
unsigned int MSSSBuffer::determineSeqC(unsigned int id_h) const
{
	unsigned int min = getMinSeqNC();
	list<s_sendMsgMSS>::const_iterator it = buffer.buffer.begin();
	while (it != buffer.buffer.end() && !it->timeout)
	{
		it++; // jump the messages that the station might has already aknowledged to other stations (and which the station sends as AppCoMsg if the host did not deliver them)
		min++;
	}
	for (; it != buffer.buffer.end(); it++)
	{
		if (it->MHdelivContains(id_h))
			min++;
		else
			break;
	}
	return min - 1;
}

vector<idMsg> MSSSBuffer::getIdNotDeliveredMessages(unsigned int idMH, unsigned int seqACK) const
{
	vector<idMsg> idNotDeliveredMessages;
	for (const s_sendMsgMSS& it : buffer.buffer)
	{
		if (it.seqNC > seqACK && !it.MHdelivContains(idMH))
			idNotDeliveredMessages.push_back(it.id);
	}
	for (const s_sendMsgMSS& it : pendingMsg.buffer)
	{
		if (!it.MHdelivContains(idMH))
			idNotDeliveredMessages.push_back(it.id);
	}
	return idNotDeliveredMessages;
}

vector<idMsg> MSSSBuffer::getIdMessagesReceivedAfter(const vector<unsigned int>& timeVector) const
{
	vector<idMsg> msgReceivedAfter = buffer.getIdMessagesReceivedAfter(timeVector);
	vector<idMsg> pendingMsgReceivedAfter = pendingMsg.getIdMessagesReceivedAfter(timeVector);
	msgReceivedAfter.insert(msgReceivedAfter.end(), pendingMsgReceivedAfter.begin(), pendingMsgReceivedAfter.end());
	return msgReceivedAfter;
}

vector<tuple<idMsg, unsigned int>> MSSSBuffer::getMessagesOrderBroadcastedMessages() const
{
	return buffer.getMessagesOrder();
}

vector<idMsg> MSSSBuffer::getIdMessagesNotDeliveredNorBroadcasted(unsigned int idMH)
{
	vector<idMsg> messages;
	list<struct s_sendMsgMSS>::iterator it = buffer.begin();

	// Messages not broadcasted anymore, meaning that the station has already acknowledged them to other stations,
	// and that other stations might therefore deleted those messages and therefore send them in AppCo
	while (it != buffer.end() && !it->timeout)
	{
		if (!it->MHdelivContains(idMH))
			messages.push_back(it->id);
		it++;
	}
	return messages;
}

void MSSSBuffer::tagAlreadyDeliveredMessagesByMH(const vector<unsigned int>& recvRsp1Vector,
		const vector<idMsg>& notDeliveredMessages, const vector<idMsg>& msg_rcv, unsigned int id_h)
{
	buffer.tagAlreadyDeliveredMessagesByMH(recvRsp1Vector, notDeliveredMessages, msg_rcv, id_h);
	pendingMsg.tagAlreadyDeliveredMessagesByMH(recvRsp1Vector, notDeliveredMessages, msg_rcv, id_h);
}

void MSSSBuffer::tagAlreadyDeliveredMessagesByMH(unsigned int id_h, unsigned int seqC)
{
	buffer.tagAlreadyDeliveredMessagesByMH(id_h, seqC);
}

void MSSSBuffer::tagAlreadyDeliveredMessagesByMH(const list<struct s_sendMsgMSS>& acknowledgedAppCo, unsigned int idMH)
{
	for (const struct s_sendMsgMSS& m : acknowledgedAppCo)
	{
		if (buffer.contains(m.id))
		{
			list<struct s_sendMsgMSS>::iterator it = buffer.searchMsgIt(m.id);
			it->AddMHDeliv(idMH);
		}
	}
}
