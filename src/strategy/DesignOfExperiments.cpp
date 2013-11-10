/*
 * DesignOfExperiments.cpp
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#include "DesignOfExperiments.h"
#include "Config.h"
#include <iostream>

const string doe = "design_of_experiments";

DesignOfExperiments::DesignOfExperiments() {
	setName(doe);
}

DesignOfExperiments::~DesignOfExperiments() {
}

strategy_ptr DesignOfExperiments::clone() const {
	return strategy_ptr(new DesignOfExperiments(*this));
}

Result DesignOfExperiments::search(const_target_ptr target,
		const_device_ptr device) {
	init(target,device);

	Config bestConfig;
	assert(findNearestConfig(target,bestConfig) == true); // must be initialized first

	Result r;
	r.setConfig(bestConfig);
	r.setDuration(getDuration());
	r.setNumHits(numHits);
	r.setNumTries(1);
	r.setTableSize(getTableSize());
	r.setTarget(target);
	r.setTotalEnergy(totalEnergy);
	r.setValue(target->getValue(&bestConfig));
	r.setTablePercent((double)getTableSize()/device->getNumConfigs() * 100);
	r.setHitPercent(getHitPercent(target));

	return r;
}

