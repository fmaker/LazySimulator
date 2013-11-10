/*
 * RandomGreedy.h
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#ifndef RANDOMGREEDY_H_
#define RANDOMGREEDY_H_

#include "Strategy.h"
#include "Config.h"

class RandomGreedy : public Strategy {
public:
	RandomGreedy();
	virtual ~RandomGreedy();
	virtual boost::shared_ptr<Strategy> clone() const;
    Result search(const_target_ptr target, const_device_ptr device);
    void printStatus(int tries,double target,double best,float improved) const;

private:
	Device *device;
};

#endif /* RANDOMGREEDY_H_ */
