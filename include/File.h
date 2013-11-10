/*
 * File.h
 *
 *  Created on: Dec 16, 2010
 *      Author: fmaker
 */

#ifndef FILE_H_
#define FILE_H_
#include <tinyxml.h>
#include <string>
#include <list>
#include "Simulation.h"
#include "Device.h"
#include "Strategy.h"
#include "Target.h"
#include "Logfile.h"
using namespace std;

class File {
public:
	File(string simFile);
	virtual ~File();
	int getNumRuns();
	void parse();
	list<const_device_ptr> getDevices() const;
    list<const_strategy_ptr> getStrategies() const;
    list<const_target_ptr> getTargets() const;
    void parseDevices(TiXmlNode *devices);
    void parseStrategies(TiXmlNode *strategies);
    void parseTargets(TiXmlNode *targets);
    void parseOutputs(TiXmlNode *outputs);
    int getNumTargets() const;
    void setNumTargets(int numTargets);
    void setTargets(list<const_target_ptr> targets);
    int getSeed() const;
    void setSeed(int seed);
    Logfile getLogfile() const;
    void setLogfile(Logfile logfile);
    string getSimFile() const;

private:
	string simFile;
	int seed;
	int numRuns;
	int numTargets;
	list<const_device_ptr> devices;
	list<const_target_ptr> targets;
	list<const_strategy_ptr> strategies;
	Logfile logfile;

};

bool to_bool(string str);

#endif /* FILE_H_ */
