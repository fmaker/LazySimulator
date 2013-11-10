/*
 * HillClimbing.h
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#ifndef HILLCLIMBING_H_
#define HILLCLIMBING_H_

#include "Strategy.h"

class HillClimbing: public Strategy {
public:
	HillClimbing();
	virtual ~HillClimbing();
	virtual strategy_ptr clone() const;
    virtual Result search(const_target_ptr target, const_device_ptr device);
    float getTemp(int step, int steps);
private:
    //double tmax;
    //double tmin;
    //int steps;
};




#endif /* HILLCLIMBING_H_ */
