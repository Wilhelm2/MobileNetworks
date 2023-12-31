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

#include "OpenSocketMap.h"
#include "inet/applications/common/SocketTag_m.h"
#include "inet/common/INETDefs.h"
#include "inet/common/packet/Message.h"
#include "inet/common/packet/Packet.h"

TcpSocket* OpenSocketMap::findSocketFor(cMessage* msg) {
    auto& tags = getTags(msg);
    int connId = tags.getTag<SocketInd>()->getSocketId();
    auto i = socketMap.find(connId);
    ASSERT(i == socketMap.end() || i->first == i->second->getSocketId());
    return (i == socketMap.end()) ? nullptr : i->second;
}

void OpenSocketMap::addSocket(TcpSocket* socket) {
    ASSERT(socketMap.find(socket->getSocketId()) == socketMap.end());
    socketMap[socket->getSocketId()] = socket;
}

TcpSocket* OpenSocketMap::removeSocket(TcpSocket* socket) {
    auto i = socketMap.find(socket->getSocketId());
    if (i != socketMap.end())
        socketMap.erase(i);
    return socket;
}

void OpenSocketMap::deleteSockets() {
    for (auto& elem : socketMap)
        delete elem.second;
    socketMap.clear();
}

std::map<int, TcpSocket*>::iterator OpenSocketMap::begin() {
    return socketMap.begin();
}

std::map<int, TcpSocket*>::iterator OpenSocketMap::end() {
    return socketMap.end();
}
