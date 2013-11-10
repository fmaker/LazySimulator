/*
 * RandomGreedy.cpp
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#include "RandomGreedy.h"
#include <iostream>

const string rg = "random_greedy";

RandomGreedy::RandomGreedy() {
	setName(rg);
}

RandomGreedy::~RandomGreedy() {
}

strategy_ptr RandomGreedy::clone() const{
	return strategy_ptr(new RandomGreedy(*this));
}

Result RandomGreedy::search(const_target_ptr target, const_device_ptr device){
	init(target,device);
	Result r;

	// Start the search for a better target
	Config bestConfig,nextConfig;
	int taken = 0;
	// First loop is just to get starting configuration
	int i,tries = 0;

	// Start out with best configuration found so far
	bool validBestConfig = findNearestConfig(target,bestConfig);

	// Look for another config, unless this one is within bounds
	if(!(validBestConfig && target->withinBounds(bestConfig)) &&
			(remainingConfigs.size() > 0) &&
			!overLimit){

		for(i=0;i<getTries();i++){
			// Try another random configuration
			if(onlyNewRandom){
				nextConfig = getNewConfigRandom(device);
			}
			else{
				nextConfig = getConfigRandom(device);
			}

			// If no bestConfigOtherwise current config is best
			if(!validBestConfig){
				bestConfig = nextConfig;
				validBestConfig = true;
			}

			memoizeConfig(nextConfig);

			// Test value
			// If not between limits, something wrong
			double configValue = target->getValue(&nextConfig);
			//		assert(configValue <= target->getMaxValue() && configValue >= 0); // Can be zero even though minValue > 0
			assert(configValue >= 0);

			// If config better than best
			if(target->getValueDelta(configValue) < target->getValueDelta(&bestConfig)){
				bestConfig = nextConfig;
				taken++;
			}
			// Header on first loop
			if(i == 0)
				Strategy::printStatusHeader();

			// Success if between bounds
			double bestValue = target->getValue(&bestConfig);
			if((bestValue >= target->getLowerBound() && bestValue <= target->getUpperBound())){
				printStatus(i,target->getValue(),target->getValue(&bestConfig),taken);
				break;
			}
			if(i % (NUM_STATUS_PRINT) == 0)
				printStatus(i,target->getValue(),target->getValue(&bestConfig),taken);


			if(overLimit)
				break;
		}
		tries = i;
	}

	// Setup result
	r.setConfig(bestConfig);
	r.setTarget(target);
	r.setValue(target->getValue(&bestConfig));
	r.setNumTries(tries);
	r.setNumHits(numHits);
	r.setTableSize(getTableSize());
	r.setDuration(getDuration());
	r.setTotalEnergy(totalEnergy);
	r.setTablePercent((double)getTableSize()/device->getNumConfigs() * 100);
	r.setHitPercent(getHitPercent(target));

//	cout << "-----------------------" << endl;
//	int m=0;
//	for(vector<bool>::iterator it=memoTableEntry.begin();it!=memoTableEntry.end();it++){
//		cout << m;
//		if(*it)
//			cout << " 1";
//		else
//			cout << " 0";
//		cout << endl;
//		m++;
//	}
//	cout << "-----------------------" << endl;

	return r;
}

void RandomGreedy::printStatus(int tries,double target,double best,float taken) const{
#ifdef DEBUG
	cout << tries << "\t\t"
		 << target << "\t\t"
		 << best << "\t\t"
		 << taken << ""
		 << endl;
#endif
}


