/*
 * Energy.h
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#ifndef ENERGY_H_
#define ENERGY_H_

#include "Target.h"
#include "Device.h"
#include "Config.h"

class Energy: public Target {
public:
	Energy();
	virtual ~Energy();
	void initLimits(device_ptr d);
	double getValue(Config *c) const;
	virtual target_ptr clone() const;
};

#endif /* ENERGY_H_ */
