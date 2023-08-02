/*
 * commun_f.h
 *
 *  Created on: May 7, 2019
 *      Author: dwilhelm
 */

#ifndef COMMUN_F_H_
#define COMMUN_F_H_

#include <omnetpp.h>
#include <string.h>
#include <algorithm>
using namespace std;

#include "Messages/timeout_m.h"

using namespace omnetpp;

typedef struct s_idMsg
{
	unsigned int idMH;
	unsigned int seqMH;
} idMsg;

typedef struct s_interval
{
	unsigned int begin;
	unsigned int end;
	bool contains(unsigned int seq) const
	{
		return seq >= begin && seq <= end;
	}
} AckInterval;

#define ID_EQ(id1,id2) (id1.idMH == id2.idMH && id1.seqMH == id2.seqMH)

#define PACKAGE_NAME "Arbre"

#endif /* COMMUN_F_H_ */
