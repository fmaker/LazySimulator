/*
 * HillClimbing.cpp
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#include "HillClimbing.h"
#include "random.h"
#include <math.h>
#include <iomanip>

const string hc = "hill_climbing";

HillClimbing::HillClimbing() {
	setName(hc);
}

HillClimbing::~HillClimbing() {
}

strategy_ptr HillClimbing::clone() const{
	return strategy_ptr(new HillClimbing(*this));
}

Result HillClimbing::search(const_target_ptr target, const_device_ptr device){
	init(target,device);
	Result r;
	// Start out with best configuration found so far
	Config config,bestConfig;
	bool validBestConfig = findNearestConfig(target,bestConfig);

	// Initialize temperature
	double tfactor = -log(tp2/tp1);
	double t = tp2;

	double dE;

	if(validBestConfig){
		config = bestConfig;
	}
	else{
		// Start with random config
		config = device->getConfigRandom();
		assert(config.getConfigNum() != 0);
		bestConfig = config;
	}

	// While haven't run out of steps and value not good enough
	int i=0,accepts=0,improves=0;
#ifdef DEBUG
	cout << "tmin:" << tp1 << " tmax: " << tp2 << endl;
	cout << "Step\tTemp\tTarget\tBest\tEnergy\tAccept\tImprove" << endl;
#endif
	resetNumHits();
	while (i < tries &&
			!(target->getValue(&bestConfig) < target->getUpperBound() &&
			target->getValue(&bestConfig) > target->getLowerBound()) &&
			!overLimit){
#ifdef DEBUG
		cout << "Step: " << i << " out of: " << tries << endl;
#endif

		// Calculate temperature
		//t = tp2 * exp(tfactor * ((double) i / (double) tries));
		t = getTemp(i,tries);

		Config neighbor = device->getRandomNeighbor(config);

		memoizeConfig(neighbor);

		// If delta is positive, neighbor is farther, accept with temperature based probablity
		// Otherwise, neighbor is closer, accept no matter what
		dE = 100*(target->getValueDelta(&neighbor) - target->getValueDelta(&config))
				/target->getValue(); // Convert to percent
		if (dE < 0 || exp(-dE/t) > randfloat()){
			// Accept new state
			config = neighbor;
#ifdef DEBUG
			accepts++;
			// If better than current config
			if(dE < 0)
				improves++;
#endif

			// If best found so far
			if(target->getValueDelta(&bestConfig) > target->getValueDelta(&config)){
				bestConfig = config;
			}
		}


#ifdef DEBUG
		if(i == 0 || i%(tries/10) == 0 || i == tries-1)
			cout << i << '\t' << t << '\t'  << setprecision(3) << target->getValue() << '\t' << target->getValue(&bestConfig)<< '\t' << target->getValue(&config)
			<< '\t' <<  accepts << '\t' << improves << endl;
#endif
		if(overLimit)
			break;
		i++;
	}

	r.setTarget(target);
	r.setConfig(bestConfig);
	r.setDuration(duration);
	r.setNumHits(numHits);
	r.setNumTries(i);
	r.setTableSize(getTableSize());
	r.setTargetNumber(target->getValue());
	r.setTotalEnergy(totalEnergy);
	r.setValue(target->getValue(&bestConfig));
	r.setTablePercent((double)getTableSize()/device->getNumConfigs() * 100);
	r.setHitPercent(getHitPercent(target));

	return r;
}

float HillClimbing::getTemp(int step, int steps)
{
	float temp,tfactor,
		  x = (float) step,
		  x_max = (float) steps;
	switch(func){
	// tp1 is initial probability
	case LINEAR:
		temp = ((tp1-tp2)/(500))*((double) step) + tp2;
		break;
	case CONSTANT:
		temp = tp1;
		break;
	case EXPONENTIAL:
		tfactor = -log(tp2/tp1);
		temp = tp2 * exp(tfactor * ((double) step / (double) tries));
		break;
	default:
		cout << "Temperature function invalid: " << this->func << endl;
		exit(2);
	}

	return temp;
}
