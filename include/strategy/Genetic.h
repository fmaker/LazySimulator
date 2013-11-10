/*
 * Genetic.h
 *
 *  Created on: Mar 8, 2012
 *      Author: fmaker
 */

#ifndef GENETIC_H_
#define GENETIC_H_
#include "Result.h"
#include "Strategy.h"

class Genetic : public Strategy {
public:
	Genetic();
	virtual ~Genetic();
	virtual strategy_ptr clone() const;
	Result search(const_target_ptr target, const_device_ptr device);
	Config selectBreeder(vector<Config> population);
	list<Config> mate(Config a, Config b, const_device_ptr d);
private:
	float fitness(Target *t, Config *c);
	void remove_duplicates(vector<Config> vec);
	unsigned int seedPopulation;
	unsigned int childrenCount;
	unsigned int populationLimit;
	float mutationProb;

};



#endif /* GENETIC_H_ */
