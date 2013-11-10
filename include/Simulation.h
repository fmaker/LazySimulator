/*
 * Simulation.h
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "Device.h"
#include "Strategy.h"
#include "Target.h"
#include "Result.h"
#include "Logfile.h"
#include <iostream>
using namespace std;
#include <boost/shared_ptr.hpp>
using namespace boost;

class Simulation
{
public:
	Simulation(int runNum, device_ptr d, strategy_ptr s, target_ptr t);
	virtual ~Simulation();
    device_ptr getDevice() const;
    int getRunNum() const;
    strategy_ptr getStrategy() const;
    target_ptr getTargetPrototype() const;
    void setDevice(device_ptr device);
    void setRunNum(int runNum);
    void setStrategy(strategy_ptr strategy);
    void setTargetPrototype(target_ptr target);
    string toString () const;
    void run();
    int getNumTargets() const;
    void setNumTargets(int numTargets);
    void initTargets();
    int getPercent() const;
    void setPercent(int percent);
    int getSeed() const;
    void setSeed(int seed);
    Logfile* getLogfile() const;
    void setLogfile(Logfile *logfile);
private:
    string simXmlFile;
    int seed;
	int numRuns;
	int numTargets;
	int percent;
	vector<target_ptr> targets;
	Logfile *logfile;
	device_ptr device;
	strategy_ptr strategy;
	target_ptr targetPrototype;
	void broadcastResult(Result r);

};

ostream& operator<<(ostream& out, const Simulation& s);



#endif /* SIMULATION_H_ */
