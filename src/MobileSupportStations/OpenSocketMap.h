//
// Copyright (C) OpenSim Ltd.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef OPENSOCKETMAP_H_
#define OPENSOCKETMAP_H_

#include <map>
#include "inet/common/INETDefs.h"
#include "inet/transportlayer/contract/tcp/TcpSocket.h"

using namespace inet;

/**
 * Contains the TCP connections between stations
 */
class OpenSocketMap
{

public:
	std::map<int, TcpSocket *> socketMap; // must be pointers

	OpenSocketMap()
	{
	}

	/**
	 * Destructor does NOT delete the socket objects.
	 */
	~OpenSocketMap()
	{
	}

	/**
	 * Finds the socket for the given message.
	 */
	TcpSocket *findSocketFor(cMessage *msg);

	/**
	 * Adds the given socket.
	 */
	void addSocket(TcpSocket *socket);

	/**
	 * Removes the given socket.
	 */
	TcpSocket *removeSocket(TcpSocket *socket);

	/**
	 * Returns the number of sockets stored.
	 */
	unsigned int size() const
	{
		return socketMap.size();
	}

	/**
	 * Deletes the socket objects.
	 */
	void deleteSockets();

	/**
	 * Returns an iterator to the begin of the map
	 */
	std::map<int, TcpSocket *>::iterator begin();

	/**
	 * Returns an iterator to the end of the map
	 */
	std::map<int, TcpSocket *>::iterator end();

};

#endif /* OPENSOCKETMAP_H_ */

