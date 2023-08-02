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

#ifndef MOBILEHOSTS_MHSTATE_H_
#define MOBILEHOSTS_MHSTATE_H_

// Class to represent the state of a host
class MHState
{
public:
	enum class STATE
	{
		Join, Init, Connecting, ConnectionEstablished
	};

	MHState();
	virtual ~MHState();
	void moveToNewCell();
	bool isConnectionEstablished();
	STATE getState();
	void setConnectionToEstablished();
	void setConnectionToConnecting();
	bool isInitiatingConnection();

private:
	STATE state;
};

#endif /* MOBILEHOSTS_MHSTATE_H_ */
