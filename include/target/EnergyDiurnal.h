/*
 * EnergyDiurnal.h
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#ifndef ENERGYDIURNAL_H_
#define ENERGYDIURNAL_H_

#include "Energy.h"

class EnergyDiurnal: public Energy {
public:
	EnergyDiurnal();
	virtual ~EnergyDiurnal();
	auto_ptr<Target> buildTarget();
};

#endif /* ENERGYDIURNAL_H_ */
