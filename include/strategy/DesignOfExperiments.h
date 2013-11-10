/*
 * DesignOfExperiments.h
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#ifndef DESIGNOFEXPERIMENTS_H_
#define DESIGNOFEXPERIMENTS_H_

#include "Strategy.h"

class DesignOfExperiments: public Strategy {
public:
	DesignOfExperiments();
	virtual ~DesignOfExperiments();
	virtual strategy_ptr clone() const;
	virtual Result search(const_target_ptr target, const_device_ptr device);
private:
};

#endif /* DESIGNOFEXPERIMENTS_H_ */
