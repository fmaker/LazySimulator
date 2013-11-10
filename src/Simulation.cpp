/*
 * Simulation.cpp
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#include "Simulation.h"
#include "Target.h"
#include "random.h"
#include "targets.h"
#include <assert.h>
#include <list>
#include <sstream>

Simulation::Simulation(int runNum,
		device_ptr d,
		strategy_ptr s,
		target_ptr t){
	setRunNum(runNum);
	setDevice(d);
	setStrategy(s);
	setTargetPrototype(t);
	setSeed(NULL);

	// Load limits for target on this device
	getTargetPrototype()->initLimits(getDevice());
	getStrategy()->initMemo(d);
	getStrategy()->init(getTargetPrototype(),device);
}

Simulation::~Simulation(){
}

void Simulation::run(){
	// Create list of targets
	initTargets();

	if(strategy->fillToHitPercent)
	{
		do
		{
			target_ptr t = getTargetPrototype()->clone();
			t->setValueRandom();
			Result result = strategy->search(t,getDevice());
			cout << result.getTableSize() << endl;
		}while(strategy->getHitPercent(getTargetPrototype()) < strategy->getHitPercentGoal() / 100.0);
//		strategy->setLookup(true);
		cout << strategy->getTableSize() << endl;
	}

	// Use strategy to search for target
	vector<target_ptr>::iterator it;
	logfile->init();

	// If used doe add an entry, mainly for init energy and duration
	if(strategy->doeInitialized){
		vector<Config> doeConfigs = strategy->getMemoTable();
		ofstream doeWriter(logfile->doeFileName.c_str());
		doeWriter << targetPrototype->getName() << " " << targetPrototype->getPercent() << "%"<< endl;
		for(int i=0;i<doeConfigs.size();i++){
			if(strategy->memoTableEntry[i]){
				doeWriter << targetPrototype->getValue(&doeConfigs[i]) << endl;
			}
		}
		doeWriter << endl << strategy->usedDoeConfigs;
		doeWriter.close();
		Result r;
		r.setTargetNumber(0);
		r.setNumTries(0);
		target_ptr t= getTargetPrototype()->clone();
		t->setValue(0);
//		r.setConfig();
		r.setTarget(t);
		r.setValue(0);
		r.setNumHits(0);
		r.setTableSize(strategy->getTableSize());
		r.setDuration(strategy->getDuration());
		r.setTotalEnergy(strategy->getTotalEnergy());
		r.setTablePercent(100*(double)strategy->getTableSize()/device->getNumConfigs());
		r.setHitPercent(strategy->getHitPercent(t));
		logfile->write(r);

	}

	int i = 1;
	for(it=targets.begin();it!=targets.end();it++){
//#ifdef DEBUG
		cout << strategy->toString() << " running Target #: " << i << endl;
//#else
//		if(i%100 == 0){
//			cout << ".";
//			flush(cout);
//		}
//#endif
		Result r;
//		if(strategy->getLookup())
//		{
//			Config c;
//			strategy->findNearestConfig(*it,c);
//			r.setNumTries(1);
//			r.setConfig(c);
//			r.setTarget(*it);
//			r.setValue((*it)->getValue(&c));
//			r.setNumHits(0);
//			r.setTableSize(strategy->getTableSize());
//			if(i==1)
//				r.setDuration(strategy->getDuration());
//			else
//				r.setDuration(0.0);
//			if(i==1)
//				r.setTotalEnergy(strategy->getTotalEnergy());
//			else
//				r.setTotalEnergy(0.0);
//			r.setTablePercent((double)strategy->getTableSize()/device->getNumConfigs());
//		}
//		else
//		{
			r = strategy->search(*it,getDevice());
//		}
		r.setTargetNumber(i++);
		logfile->write(r);
	}
	logfile->close();
	cout << endl;

	/*	// Print out memo table
		int numEntries = strategy->memoTableEntry.size();
		for(int i=0;i<numEntries;i++){
		if(strategy->memoTableEntry[i])
		cout << getDevice()->getConfig(i)<< endl;
		}
		cout << strategy->getName() << endl;
		cout << strategy->getTableSize() << endl;*/

}

void Simulation::initTargets(){
	double max,min;

	if(strategy->getDoeLimits()){
		assert(strategy->doeInitialized);
		max = strategy->getMemoMax(getTargetPrototype());
		min = strategy->getMemoMin(getTargetPrototype());
	}
	else{
		max = getTargetPrototype()->getMaxValue();
		min = getTargetPrototype()->getMinValue();
	}
	// Need new seed for every function call
	//unsigned int baseSeed = getSeed()+getRunNum()*numTargets;
	int runNum = getRunNum();

	for(int i=0;i<numTargets;i++){
		// Get random value and check it before adding to list
		target_ptr t = getTargetPrototype()->clone();
		t->setSeed(int_constants[runNum][i]);
		t->setValueRandom(min,max);

		//t->initLimits(getDevice());
		assert(t->getValue() < max);
		assert(t->getValue() > min);

		// Add to list of targets
		targets.push_back(t);
	}
}

device_ptr Simulation::getDevice() const
{
	return device;
}

int Simulation::getRunNum() const
{
	return numRuns;
}

shared_ptr<Strategy> Simulation::getStrategy() const
{
	return strategy;
}

target_ptr Simulation::getTargetPrototype() const
{
	return targetPrototype;
}

void Simulation::setDevice(device_ptr device)
{
	this->device = device;
}

void Simulation::setRunNum(int runNum)
{
	this->numRuns = runNum;
}

void Simulation::setStrategy(strategy_ptr strategy)
{
	this->strategy = strategy;
}

void Simulation::setTargetPrototype(target_ptr target)
{
	this->targetPrototype = target;
}

void Simulation::setLogfile(Logfile *logfile){
	this->logfile = logfile;
}

Logfile* Simulation::getLogfile() const{
	return logfile;
}

string Simulation::toString() const{
	stringstream ret;
	ret << "Run #: " << getRunNum() << endl;
	ret << "Device: " << getDevice()->toString() << endl;
	ret << "Strategy: " << getStrategy()->toString() << endl;
	ret << "Target: " << getTargetPrototype()->toString() << endl;
	return ret.str();
}

ostream& operator<<(ostream& out, const Simulation& s){
	return out << s.toString();
}

int Simulation::getNumTargets() const
{
	return numTargets;
}

void Simulation::setNumTargets(int numTargets)
{
	this->numTargets = numTargets;
}

int Simulation::getPercent() const
{
	return percent;
}

void Simulation::setPercent(int percent)
{
	this->percent = percent;
}

int Simulation::getSeed() const
{
	return seed;
}

void Simulation::setSeed(int seed)
{
	this->seed = seed;
}





