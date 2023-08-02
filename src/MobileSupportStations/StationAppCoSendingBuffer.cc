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

#include "StationAppCoSendingBuffer.h"

StationAppCoSendingBuffer::StationAppCoSendingBuffer()
{
}

StationAppCoSendingBuffer::~StationAppCoSendingBuffer()
{
}

size_t StationAppCoSendingBuffer::size() const
{
	return buffer.size();
}

struct s_sendMsgMSS StationAppCoSendingBuffer::front() const
{
	return buffer.front();
}

struct s_sendMsgMSS StationAppCoSendingBuffer::back() const
{
	return buffer.back();
}

struct s_sendMsgMSS StationAppCoSendingBuffer::back()
{
	return buffer.back();
}

bool StationAppCoSendingBuffer::empty() const
{
	return buffer.empty();
}

bool StationAppCoSendingBuffer::contains(idMsg idMessage) const
{
	for (const s_sendMsgMSS& it : buffer)
	{
		if (ID_EQ(it.id, idMessage))
			return true;
	}
	return false;
}

list<struct s_sendMsgMSS> StationAppCoSendingBuffer::clear(unsigned int minSeq)
{
	list<struct s_sendMsgMSS> deletedMessages;

	list<struct s_sendMsgMSS>::iterator it = buffer.begin();
	while (it != buffer.end() && it->seqNC <= minSeq)
		it++;
	deletedMessages.insert(deletedMessages.end(), buffer.begin(), it);
	buffer.erase(buffer.begin(), it);
	return deletedMessages;
}

list<struct s_sendMsgMSS> StationAppCoSendingBuffer::clear()
{
	list<struct s_sendMsgMSS> res = buffer;
	buffer.clear();
	return res;
}

void StationAppCoSendingBuffer::nullifyTimeouts()
{
	for (struct s_sendMsgMSS& m : buffer)
		m.timeout = nullptr;
}

list<struct s_sendMsgMSS>::iterator StationAppCoSendingBuffer::searchMsgIt(unsigned int seq)
{
	for (list<struct s_sendMsgMSS>::iterator it = buffer.begin(); it != buffer.end(); it++)
	{
		if (it->seqNC == seq)
			return it;
	}
	cerr << "Message MSS SBuffer " << seq << " not found" << endl;
	throw "Message not found";
}

list<struct s_sendMsgMSS>::iterator StationAppCoSendingBuffer::searchMsgIt(idMsg idMessage)
{
	for (list<struct s_sendMsgMSS>::iterator it = buffer.begin(); it != buffer.end(); it++)
	{
		if (ID_EQ(it->id, idMessage))
			return it;
	}
	cerr << "Message MSS SBuffer " << idMessage.idMH << "," << idMessage.seqMH << " not found" << endl;
	throw "Message not found";
}

void StationAppCoSendingBuffer::updateSeq()
{
	unsigned int tmpseq = 1;
	for (list<struct s_sendMsgMSS>::iterator msg = buffer.begin(); msg != buffer.end(); msg++)
		msg->seqNC = tmpseq++;
}

vector<idMsg> StationAppCoSendingBuffer::getMessageIds() const
{
	vector<idMsg> res;
	for (struct s_sendMsgMSS m : buffer)
		res.push_back(m.id);
	return res;
}
