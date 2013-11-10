/*
 * Genetic.cpp
 *
 *  Created on: Mar 8, 2012
 *      Author: fmaker
 */

#include "Genetic.h"
#include "Config.h"
#include <iostream>
#include <cmath>
#include <string>

// Functor to allow target to be used in compare to determine value
struct compare
{
	const_target_ptr target;

	compare(const_target_ptr target){
		this->target = target;
	}

	// Fitness function
	bool operator() (Config a, Config b){
		double valA, valB;
		valA = target->getValueDelta(&a);
		valB = target->getValueDelta(&b);
		return valA < valB;
	}
};

struct same_config
{
	const_target_ptr target;

	same_config(const_target_ptr target){
		this->target = target;
	}

	// Fitness function
	bool operator() (Config a, Config b){
		return target->getValue(&a) == target->getValue(&b);
	}
};

void Genetic::remove_duplicates(vector<Config> vec)
{
	//vec.erase(unique(vec.begin(), vec.end()), vec.end());
}

Genetic::Genetic() {
	setName("genetic");
	seedPopulation = 40;
	childrenCount = 20;
	populationLimit = 20;
	mutationProb = 0.10;
}

Genetic::~Genetic() {
}

Config Genetic::selectBreeder(vector<Config> population)
{
	// Function exponentially decays from 1 to 1/populationSize at last entry
	// r(x) = exp(-x/(-populationSize/ln(1/populationSize)))
	const int size = population.size();

	// Using inverse transform sampling we transform the uniform distribution into
	// our desired distribution
	int choice = -1;
	const int min = 0;
	const int max = size-1; // log start 1 but array starts at zero
	const float tau = -size/ log(1.0/size);

	// Small chance we'll be outside the range, so try again
	while (choice < min || choice > max){
		float r = (float)rand() / (float)RAND_MAX;
		float fc = log(1-r)*-tau;
		choice  = (int) fc;
	}
#ifdef DEBUG
	cout << "Choice = " << choice << " out of " << size << endl;
#endif
	return population[choice];
}

list<Config> Genetic::mate(Config a, Config b, const_device_ptr device)
{
	unsigned int i;
	cparams_t child1,child2;
	Config Child1, Child2;
	list<Config> children;


	// ORDERING MUST MATCH cparams_t STRUCT!!!
	string (Config::*gene_getters[])() const = {
			&Config::getRadio,
			&Config::getExposure_mode,
			&Config::getFlash_mode,
			&Config::getColor_mode,
			&Config::getWidth,
			&Config::getWhitebalance_mode,
			&Config::getZoom};

	//


	// Loop over all config parameter getters
	for(i=0; i<sizeof(gene_getters)/sizeof(gene_getters[0]); i++){
		float r, mp1, mp2;
		bool mutate1, mutate2;
		string c1, c2;
		string a_val = (a.*gene_getters[i])();
		string b_val = (b.*gene_getters[i])();

#ifdef DEBUG
		cout << "Parent A: " << a_val << endl;
		cout << "Parent B: " << b_val << endl;
#endif

		r = ((float)rand() / (float) RAND_MAX);
#ifdef DEBUG
		cout << "Random int is: " << r << endl;
#endif
		if(r > 0.5){
			c1 = a_val;
			c2 = b_val;
		}
		else{
			c2 = a_val;
			c1 = b_val;
		}

#ifdef DEBUG
		cout << "Child 1: " << c1 << endl;
		cout << "Child 2: " << c2 << endl;
		cout << endl;
#endif
		mp1 = ((float)rand() / (float) RAND_MAX);
		mp2 = ((float)rand() / (float) RAND_MAX);
		mutate1 = mp1 < mutationProb;
		mutate2 = mp2 < mutationProb;

		int size = device->getParamVector((config_param) i)->size();
		if(mutate1){
			string c1n = c1;
			if(size > 1){
				while(c1n == c1){
					c1n = device->getRandomParam((config_param) i);
				}c1 = c1n;
			}
		}
		if(mutate2){
			string c2n = c2;
			if(size > 1){
				while(c2n == c2){
					c2n = device->getRandomParam((config_param) i);
				}c2 = c2n;
			}
		}

		switch(i){
			case RADIO:
				child1.radio = c1;
				child2.radio = c2;
				break;
			case EXPOSURE_MODE:
				child1.exposure_mode = c1;
				child2.exposure_mode = c2;
				break;
			case FLASH_MODE:
				child1.flash_mode = c1;
				child2.flash_mode = c2;
				break;
			case COLOR_MODE:
				child1.color_mode = c1;
				child2.color_mode = c2;
				break;
			case WIDTH:
				child1.width = c1;
				child2.width = c2;
				break;
			case WHITEBALANCE_MODE:
				child1.whitebalance_mode = c1;
				child2.whitebalance_mode = c2;
				break;
			case ZOOM:
				child1.zoom = c1;
				child2.zoom = c2;
				break;
		}
	}

	// Return children list
	Child1 = device->getConfig(device->getConfigNum(child1));
	Child2 = device->getConfig(device->getConfigNum(child2));
	children.push_back(Child1);
	children.push_back(Child2);
	return children;
}

strategy_ptr Genetic::clone() const{
	return strategy_ptr(new Genetic(*this));
}

Result Genetic::search(const_target_ptr target, const_device_ptr device)
{
	init(target,device);
	unsigned int i = 0, t = 0, gens;
	vector<Config> population;
	bool sortedPopulation = false;
	Config bestConfig;
	bool validBestConfig = findNearestConfig(target,bestConfig);

	srand((unsigned)time(0));

	//cout << "Search for: " << target->getValue() << endl;

	// TODO: Use all DoE configs in initial population
	// Add bestConfig into population
	if(validBestConfig){
		population.push_back(bestConfig);
		t++;
	}

	// Start with DoE population if available, otherwise random
	if(doeInitialized){
		population = vector<Config>(doeConfigs.begin(),doeConfigs.end());
	}

	// Create initial population
	while(population.size() < seedPopulation &&
			!(target->getValue(&bestConfig) < target->getUpperBound() &&
					target->getValue(&bestConfig) > target->getLowerBound())){
		Config c = device->getConfigRandom();
		memoizeConfig(c);

		// Make sure not already included
		bool addConfig = true;
		for(vector<Config>::iterator it = population.begin(); it != population.end(); it++){
			if((*it).getConfigNum() == c.getConfigNum())
				addConfig = false;
		}
		if(addConfig){
			population.push_back(c);
			t++;
		}
	}

	// Determine how many generations we can run
	gens = (tries - population.size()) / (float) childrenCount;

	while (t < gens &&
			!(target->getValue(&bestConfig) < target->getUpperBound() &&
					target->getValue(&bestConfig) > target->getLowerBound())
			&& !overLimit){

		sort(population.begin(), population.end(), compare(target));
		sortedPopulation = true;

		vector<Config> children;
		for(i=0; i<childrenCount; i += 2){
			Config parent1 = selectBreeder(population);
			Config parent2 = selectBreeder(population);

			list<Config> kids = mate(parent1, parent2, device);
			for(list<Config>::iterator it = kids.begin(); it != kids.end(); it++){
				children.push_back(*it);
				memoizeConfig(*it);
				t++;
			}

		}

		// Add children to population
		sortedPopulation = false;
		for(vector<Config>::iterator it = children.begin(); it != children.end(); it++, i++){
			population.push_back(*it);
		}
		sort(population.begin(), population.end(), compare(target));
		sortedPopulation = true;

		vector<Config>::iterator iv;
		iv = unique(population.begin(), population.end(), same_config(target));
		population.resize(iv - population.begin());

		// Cull population down to populationLimit
		population.resize(populationLimit);

		bestConfig = population.front();

		//cout << endl << "Target = " << target->getValue() << endl;
		//cout << endl << "Best = " << target->getValue(&bestConfig) << endl;

	}

	Result r;
	r.setTarget(target);
	r.setConfig(bestConfig);
	r.setDuration(duration);
	r.setNumHits(numHits);
	r.setNumTries(t);
	r.setTableSize(getTableSize());
	r.setTargetNumber(target->getValue());
	r.setTotalEnergy(totalEnergy);
	r.setValue(target->getValue(&bestConfig));
	r.setTablePercent((double)getTableSize()/device->getNumConfigs() * 100);
	r.setHitPercent(getHitPercent(target));
	return r;
}



