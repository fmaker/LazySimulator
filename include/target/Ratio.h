/*
 * Ratio.h
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#ifndef RATIO_H_
#define RATIO_H_

#include "Target.h"

class Ratio: public Target {
public:
	Ratio();
	virtual ~Ratio();
	void initLimits(device_ptr d);
	double getValue(Config *c) const;
	virtual target_ptr clone() const;
};

#endif /* RATIO_H_ */
