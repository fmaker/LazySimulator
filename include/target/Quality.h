/*
 * Quality.h
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#ifndef QUALITY_H_
#define QUALITY_H_

#include "Target.h"

class Quality: public Target {
public:
	Quality();
	virtual ~Quality();
	void initLimits(device_ptr d);
	double getValue(Config *c) const;
	virtual target_ptr clone() const;
};

#endif /* QUALITY_H_ */
