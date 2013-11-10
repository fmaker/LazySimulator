/*
 * File.cpp
 *
 *  Created on: Dec 16, 2010
 *      Author: fmaker
 */

#include <string>
#include <list>
#include <algorithm>
using namespace std;

#include "File.h"
#include "Exception.h"
#include "Device.h"
#include "Simulation.h"
#include "target/Energy.h"
#include "target/Quality.h"
#include "target/Ratio.h"
#include "strategy/DesignOfExperiments.h"
#include "strategy/HillClimbing.h"
#include "strategy/RandomGreedy.h"
#include "strategy/Genetic.h"
#include "Logfile.h"

File::File(string simFile) {

	// Setup doc and save simFile location
	this->simFile = string(simFile);

	// Create parser results
	numRuns = 0;
//	devices = new list<Device*>;
//	strategies = new list<Strategy>;
//	targets = new list<Target*>;
}

File::~File() {
	/*list<Device*>::iterator id;
	for(id=devices->begin();id!=devices->end();id++)
		cout << (*id)->getName() << endl;
		delete *id;*/
//	delete devices;
	/*list<Target*>::iterator it;
	for(it=targets->begin();it!=targets->end();it++)
		delete *it;*/
//	delete targets;
}

void File::parse(){

	// Make sure we can parse the file
	TiXmlDocument doc(simFile);
	if(!doc.LoadFile()){
		string exp_str = "Invalid simulation file: ";
		exp_str += simFile;

		throw(Exception(exp_str));
	}

	// Get root node and check
	TiXmlHandle hDoc = TiXmlHandle(&doc);
	TiXmlElement *pParent = hDoc.FirstChild("simulation").ToElement();
	if(!pParent) throw(Exception("Invalid root"));
	pParent->QueryIntAttribute("runs",&numRuns);
	pParent->QueryIntAttribute("seed",&seed);

	TiXmlNode* pChild;
	for(pChild = pParent->FirstChild(); pChild!=NULL; pChild = pChild->NextSibling()){

		// Ignore comments
		if(pChild->Type() == TiXmlNode::TINYXML_COMMENT){
			continue;
		}

		// Devicelist
		if(strcmp(pChild->Value(),"devicelist") == 0){
			parseDevices(pChild);
		}

		// Strategylist
		else if(strcmp(pChild->Value(),"strategylist") == 0){
			parseStrategies(pChild);
		}

		// Targetlist
		else if(strcmp(pChild->Value(),"targetlist") == 0){
			parseTargets(pChild);
		}

		// Outputlist
		else if(strcmp(pChild->Value(),"outputlist") == 0){
			parseOutputs(pChild);
		}
	}
}

int File::getNumRuns(){
	return numRuns;
}

void File::parseDevices(TiXmlNode* d){
	for(TiXmlElement* pElem=d->FirstChild()->ToElement();pElem != NULL; pElem=pElem->NextSiblingElement()){

		// Full filename of database file
		//string path = simFile.substr(0,simFile.find_last_of('/')+1);
		string fileName = pElem->Attribute("file");
		//string dbFile = path+fileName;
		string dbFile = fileName;

		// Try to create device
		try{
			Device *d = new Device(dbFile);
			devices.push_back(const_device_ptr(d));
		}
		catch (Exception &e){
			cerr << "Unable to process: " << dbFile << endl;
		}
	}
}

void File::parseStrategies(TiXmlNode* s){
	int tries;
	s->ToElement()->QueryIntAttribute("tries",&tries);
	for(TiXmlElement* pElem=s->FirstChild()->ToElement();pElem != NULL; pElem=pElem->NextSiblingElement()){

		string type = pElem->Attribute("type");

		// Strategy to add with memoization option
		Strategy *s = NULL;

		if(type.compare("doe") == 0){
			s = new DesignOfExperiments();
			s->setDoeSize(128);
		}
		else if(type.compare("random_greedy") == 0){
			s = new RandomGreedy();
		}
		else if(type.compare("hill_climbing") == 0){
			s = new HillClimbing();
		}
		else if(type.compare("genetic") == 0){
			s = new Genetic();
		}

		// Always memoization
		s->setMemo(true);

		// Add to list
		int intValue;
		string strValue;
		double doubValue;
		if(pElem->QueryIntAttribute("doesize",&intValue) != TIXML_NO_ATTRIBUTE){
			s->setDoeSize(intValue);
		}
		if(pElem->QueryValueAttribute("onlynew",&strValue) != TIXML_NO_ATTRIBUTE){
			bool newRandom = to_bool(strValue);
			s->setOnlyNewRandom(newRandom);
		}
		if(pElem->QueryValueAttribute("random",&strValue) != TIXML_NO_ATTRIBUTE){
			bool randomInit = to_bool(strValue);
			s->setRandomInit(randomInit);
		}
//		if(pElem->QueryValueAttribute("memoization",&strValue) != TIXML_NO_ATTRIBUTE){
//			bool memo = to_bool(strValue);
//			s->setMemo(memo);
//		}
		if(pElem->QueryValueAttribute("limit_energy",&doubValue) != TIXML_NO_ATTRIBUTE){
			limit_t l;
			l.amountType = ENERGY;
			l.amount = doubValue;
			s->setLimited(true);
			s->setLimitType(l);
			s->setMemo(true);
		}
		if(pElem->QueryValueAttribute("limit_duration",&doubValue) != TIXML_NO_ATTRIBUTE){
			limit_t l;
			l.amountType = DURATION;
			l.amount = doubValue;
			s->setLimited(true);
			s->setLimitType(l);
			s->setMemo(true);
		}
		if(pElem->QueryValueAttribute("limit_memory",&doubValue) != TIXML_NO_ATTRIBUTE){
			limit_t l;
			l.amountType = MEMORY;
			l.amount = doubValue;
			s->setLimited(true);
			s->setLimitType(l);
			s->setMemo(true);
		}
		if(pElem->QueryValueAttribute("hit_percent",&doubValue) != TIXML_NO_ATTRIBUTE){
			s->fillToHitPercent = true;
			s->setHitPercentGoal(doubValue);
			s->setMemo(true);
		}
		if(pElem->QueryValueAttribute("doeLimits",&strValue) != TIXML_NO_ATTRIBUTE){
			bool doeLimits = to_bool(strValue);
			s->setDoeLimits(doeLimits);
			s->setDoeSize(128);
		}
		//tfunc format (line|cons|temp)tp1[:tp2] Ex: "line|5.0|1.0"
		if(pElem->QueryValueAttribute("tfunc",&strValue) != TIXML_NO_ATTRIBUTE){
			string func,tp1_str,tp2_str;
			temp_func function;
			int delim;
			float tp1 = 0,tp2 = 0;

			// Function is first 4 characters
			func = strValue.substr(0,4);
			strValue = strValue.substr(4+1,strValue.length()-4-1);

			// One parameter function
			if(func.compare("cons") == 0){
				function = CONSTANT;
				tp1_str = strValue.c_str();
			}
			// Two parameter function
			else{
				if(func.compare("line") == 0){
					function = LINEAR;
				}
				else if(func.compare("expo") == 0){
					function = EXPONENTIAL;
				}
				else {	// What the heck is this?
					cout << "Unknown temperature function: " << func << endl;
					exit(1);
				}

				// Find delimiter and use it to return tp1 and tp2
				delim = strValue.find(':');
				tp1_str = strValue.substr(0,delim-0);

				tp2_str = strValue.substr(delim+1,strValue.length()-delim);
				tp2 = std::atof(tp2_str.c_str());
			}

			tp1 = std::atof(tp1_str.c_str());

			s->setTempFunc(function,tp1,tp2);

		}

			s->setSeed(seed);
			s->setTries(tries);
			strategies.push_back(const_strategy_ptr(s));
	}
}

void File::parseTargets(TiXmlNode* t){
	int percent;
	t->ToElement()->QueryIntAttribute("targets",&numTargets);
	t->ToElement()->QueryIntAttribute("percent",&percent);

	for(TiXmlElement* pElem=t->FirstChild()->ToElement();pElem != NULL; pElem=pElem->NextSiblingElement()){
		string type = pElem->Attribute("type");

		// Strategy to add with memoization option
		Target *t = NULL;

		if(type.compare("energy") == 0){
			t = new Energy();
		}
		else if(type.compare("quality") == 0){
			t = new Quality();
		}
		else if(type.compare("ratio") == 0){
			Ratio *r = new Ratio();
			t = r;
		}

		// Add to list
		t->setPercent(percent/100.0F);
		targets.push_back(const_target_ptr(t));
	}
}

void File::parseOutputs(TiXmlNode* o){
	TiXmlElement* pElem=o->FirstChild()->ToElement();
//	for(TiXmlElement* pElem=o->FirstChild()->ToElement();pElem != NULL; pElem=pElem->NextSiblingElement()){

	string type = pElem->Attribute("type");
	string filename = pElem->Attribute("type");

	// Strategy to add with memoization option
	if(type.compare("logfile") == 0){
		Logfile l = Logfile(filename);
		l.setFilenameFormat(pElem->Attribute("filename"));
		l.setDelim(pElem->Attribute("delim")[0]);
		logfile = l;
	}
}

list<const_device_ptr> File::getDevices() const
{
    return devices;
}

list<const_strategy_ptr> File::getStrategies() const
{
    return strategies;
}

list<const_target_ptr> File::getTargets() const
{
    return targets;
}

bool to_bool(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    istringstream is(str);
    bool b;
    is >> boolalpha >> b;
    return b;
}

int File::getNumTargets() const
{
    return numTargets;
}

void File::setNumTargets(int numTargets)
{
    this->numTargets = numTargets;
}

void File::setTargets(list<const_target_ptr> targets)
{
    this->targets = targets;
}

int File::getSeed() const
{
    return seed;
}

void File::setSeed(int seed)
{
    this->seed = seed;
}

void File::setLogfile(Logfile logfile) {
	this->logfile = logfile;
}


Logfile File::getLogfile() const{
	return logfile;
}

string File::getSimFile() const{
	return simFile;
}




