/*
 * Strategy.cpp
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#include "Config.h"
#include "Strategy.h"
#include "random.h"
#include <math.h>
#include <iostream>
#include <limits>
#include <time.h>

Strategy::Strategy() {
	initialized = false;
	doeInitialized = false;
	initDoe = false;
	doeLimits = false;

    onlyNewRandom = false;
    overLimit = false;
    fillToHitPercent = false;
	setLimited(false);
	usedDoeConfigs = "";
}

Strategy::~Strategy() {
}

void Strategy::initMemo(const_device_ptr device){
	Config c;

	memoTable.resize(device->getNumConfigs()+1,c);
	memoTableEntry.resize(device->getNumConfigs()+1,false);
    remainingConfigs.resize(device->getNumConfigs());

	for(int i=0;i<device->getNumConfigs();i++){
		remainingConfigs[i] = i+1;
	}
}


void Strategy::init(const_target_ptr target, const_device_ptr device){

//    upperBound = target->getUpperBound();
//    lowerBound = target->getLowerBound();

    if((!fillToHitPercent && !getLimited()) || !initialized)
    {
    	numHits = 0;
    	duration = 0.0;
    	totalEnergy = 0.0;
    	initialized = true;
    }
    if(!doeInitialized && getInitDoe())
    {
    	if(randomInit)
    		doeInitRandom(device);
    	else
    		doeInit(device);
		doeInitialized = true;
	}

}

vector<Config> Strategy::getMemoTable() const{
	return memoTable;
}

bool Strategy::getMemo() const
{
    return memo;
}

void Strategy::setMemo(bool memo)
{
    this->memo = memo;
}

bool Strategy::getLimited() const
{
	return limited;
}
void Strategy::setLimited(bool limited)
{
	this->limited = limited;
}

string Strategy::getName() const
{
    return name;
}

void Strategy::setName(string name)
{
    this->name = name;
}

string Strategy::toString() const{
	return getName();
}

ostream& operator<<(ostream& out, const Strategy& s){
	return out << s.toString();
}

int Strategy::getSeed() const
{
    return seed;
}

void Strategy::setSeed(int seed)
{
    this->seed = seed;
}

int Strategy::getTries() const
{
    return tries;
}

void Strategy::setTries(int tries)
{
    this->tries = tries;
}

void Strategy::printStatusHeader() const{
#ifdef DEBUG
	cout << endl;
	cout << "Tries\t\tTarget\t\tBestValue\tTaken" << endl;
	cout << "--------------------------------------------------" << endl;
#endif
}

void Strategy::resetNumHits()
{
    this->numHits = 0;
}

// True if config found, False otherwise
bool Strategy::lookupConfig(int configNum, Config &config){
	bool retValue = false;
	if(memoTableEntry[configNum]){
		config = memoTable[configNum];
		retValue = true;
		numHits++;
	}
	return retValue;
}

limit_t Strategy::getLimitType() const
{
    return limitType;
}

void Strategy::setLimitType(limit_t limitType)
{
    this->limitType = limitType;
    stringstream ss;
	ss << "+limit_";
	switch(limitType.amountType)
	{
	case ENERGY:
		ss << "energy=" << this->limitType.amount;
		break;
	case DURATION:
		ss << "duration=" << this->limitType.amount;
		break;
	case MEMORY:
		ss << "memory=" << this->limitType.amount;
		break;
	}
	setName(getName()+ss.str());
}

void Strategy::setHitPercentGoal(double hitPercent)
{
    this->hitPercentGoal = hitPercent;
    stringstream ss;
    ss << hitPercent;
	setName(getName()+"_"+ss.str()+"percent");
}

double Strategy::getHitPercentGoal() const
{
    return hitPercentGoal;
}

void Strategy::saveConfig(Config &config){
	int configNum = config.getConfigNum();
	memoTableEntry[configNum] = true;
	memoTable[configNum] = config;
}

int Strategy::getNumHits() const
{
    return numHits;
}

void Strategy::setNumHits(int numHits)
{
    this->numHits = numHits;
}

int Strategy::getTableSize() const{
	int count = 0;
	for(vector<bool>::const_iterator im=memoTableEntry.begin();im!=memoTableEntry.end();im++){
		if(*im)
			count++;
	}
	return count;
}

double Strategy::getMemoMax(const_target_ptr target){
	double max = 0;
	for(int i=0;i<memoTable.size();i++){
		if(memoTableEntry[i]){
			if(target->getValue(&memoTable[i]) > max){
				max = target->getValue(&memoTable[i]);
			}
		}
	}
	return max;
}

double Strategy::getMemoMin(const_target_ptr target){
	double min = numeric_limits<double>::max();
	for(int i=0;i<memoTable.size();i++){
		if(memoTableEntry[i]){
			if(target->getValue(&memoTable[i]) < min){
				min = target->getValue(&memoTable[i]);
			}
		}
	}
	return min;
}

double Strategy::getHitPercent(const_target_ptr target)
{
	int i=0,numInTable = 0;
	Config c;
	target_ptr t = target->clone();
	for(t->setValue(t->getMinValue());t->getValue()<t->getMaxValue();t->setValue(t->getValue()*(1+t->getPercent())))
	{
		i++;
		//cout << t->getValue() << endl;
		if(findNearestConfig(t,c))
			if(t->withinBounds(c))
				numInTable++;
	}
	return (double)numInTable/i;
}

// Return nearest Config in memozation table
bool Strategy::findNearestConfig(const_target_ptr target, Config &foundConfig){
	bool foundBest = false, foundEntry = false;
	Config c,bestConfig;
	assert(memoTable.size() == memoTableEntry.size());
	for(unsigned int i=0;i<memoTable.size();i++){
		// Only if entry is valid
		if(memoTableEntry[i]){
			c = memoTable[i];

			// If we haven't found an entry yet or this entry is the best
			// so far, save it
			if(!foundBest ||
				target->getValueDelta(&c) < target->getValueDelta(&bestConfig)){
				bestConfig = c;
				foundEntry = true;
				foundBest = true;
			}
		}
	}
	foundConfig = bestConfig;
	if(foundBest)
		numHits++;
	return foundEntry;
}

void Strategy::memoizeConfig(Config c) {

	if(getMemo()){
		int configNum = c.getConfigNum();
		if(!lookupConfig(configNum,c)){
			saveConfig(c);
			duration += c.getDuration();
			totalEnergy += c.getEnergy();

			if(limited)
			{
				switch(limitType.amountType)
				{
				case ENERGY:
					limitType.amount -= c.getEnergy();
					break;
				case DURATION:
					limitType.amount -= c.getDuration();
					break;
				case MEMORY:
					limitType.amount -= ENTRY_MEMORY*1; // Each new entry takes one memory space
					break;
				}
				if(limitType.amount <= 0)
				{
					overLimit = true;
					cout << "Overlimit!" << endl;
				}
				else{
#ifdef DEBUG
					cout << "Remaining: " << limitType.amount << endl;
#endif
				}
			}
		}
	}
}


double Strategy::getDuration() const{
	return duration;
}

double Strategy::getTotalEnergy() const{
	return totalEnergy;
}

Config Strategy::getConfigRandom(const_device_ptr device){
	return device->getConfig(randint(device->getNumConfigs()));
}

Config Strategy::getNewConfigRandom(const_device_ptr device){
	int configNum;
	if(remainingConfigs.size() && getMemo()){ // might need a -1 here for numConfigs
		int r = randint(remainingConfigs.size());
		configNum = remainingConfigs[r];
		vector<int>::iterator it;
		for(it=remainingConfigs.begin();it!=remainingConfigs.end();it++){
			if(*it == configNum){
				remainingConfigs.erase(it);
				break;
			}
		}
		return device->getConfig(configNum);
	}
	// If already tried all the new random ones, return anything
	else{
		return device->getConfig(randint(device->getNumConfigs()));
	}
}

void Strategy::doeInitRandom(const_device_ptr device) {

	while(getTableSize() < getDoeSize()){
		Config c = getConfigRandom(device);
		memoizeConfig(c);
	}
#ifdef DEBUG
		cout << "Random Initialized Table Size: " << getTableSize() << endl;
#endif
}

void Strategy::doeInit(const_device_ptr device) {
	stringstream ss;
	list<Config> doeList;

	int experiments = getDoeSize();
	// Initialize with maximums since i=0 will toggle values
	cparams_t exp_config;
	exp_config.radio = device->getMaxValue(RADIO);
	exp_config.flash_mode =  device->getMaxValue(FLASH_MODE);
	exp_config.color_mode = device->getMaxValue(COLOR_MODE);
	exp_config.width = device->getMaxValue(WIDTH);
	exp_config.zoom = device->getMaxValue(ZOOM);
	exp_config.exposure_mode = device->getMaxValue(EXPOSURE_MODE);
	exp_config.whitebalance_mode = device->getMaxValue(WHITEBALANCE_MODE);

	// Get sizes to check which don't have max and min
	bool maxAndmin[NUM_CONFIG_PARAMS];
	for(int i=0;i<NUM_CONFIG_PARAMS;i++){
		if(device->getMaxValue((config_param) i).compare(device->getMinValue((config_param) i)) == 0){
			maxAndmin[i]=false;
		}
		else{
			maxAndmin[i]=true;
		}
	}

	// Iterate over each experiment
	for (int i = 0; i < experiments; i++) {
		int quant_param = 0;
		int qual_param = 0;

		// Use j as num counts until toggle and iterate through array
			for (unsigned int j = 0; j < sizeof(quant_params) / sizeof(config_param); j++) {

				// Qualitative Parameters
				// If power of two divisible into i, then toggle value
				if (i % (int) pow(2U, j) == 0) {

					// Toggle config value based on index
					switch (quant_params[quant_param]){
					case RADIO:
						// If doesn't have min and max, don't break and go to next mode
						if(maxAndmin[quant_param]){
							if (exp_config.radio.compare(device->getMinValue(RADIO)) == 0) {
								exp_config.radio = device->getMaxValue(RADIO);
							} else {
								exp_config.radio = device->getMinValue(RADIO);
							}
							break;
						}
						else{
							quant_param++;
						}
					case FLASH_MODE:
						if(maxAndmin[quant_param]){
							if (exp_config.flash_mode.compare(device->getMinValue(FLASH_MODE)) == 0) {
								exp_config.flash_mode = device->getMaxValue(FLASH_MODE);
							} else {
								exp_config.flash_mode = device->getMinValue(FLASH_MODE);
							}
							break;
						}
						else{
							quant_param++;
						}
					case COLOR_MODE:
						if(maxAndmin[quant_param]){
							if (exp_config.color_mode.compare(device->getMinValue(COLOR_MODE)) == 0) {
								exp_config.color_mode = device->getMaxValue(COLOR_MODE);
							} else {
								exp_config.color_mode = device->getMinValue(COLOR_MODE);
							}
							break;
						}
						else{
							quant_param++;
						}
					case WIDTH:
						if(maxAndmin[quant_param]){
							if (exp_config.width.compare(device->getMinValue(WIDTH)) == 0) {
								exp_config.width = device->getMaxValue(WIDTH);
							} else {
								exp_config.width = device->getMinValue(WIDTH);
							}
							break;
						}
						else{
							quant_param++;
						}
						break;
					case ZOOM:
						if(maxAndmin[quant_param]){
							if (exp_config.zoom.compare(device->getMinValue(ZOOM)) == 0) {
								exp_config.zoom = device->getMaxValue(ZOOM);
							} else {
								exp_config.zoom = device->getMinValue(ZOOM);
							}
							break;
						}
						else{
							quant_param++;
						}
						break;
					case NUM_CONFIG_PARAMS: // Ignore
					case EXPOSURE_MODE:
					case WHITEBALANCE_MODE:
						break;
					}
				}

				// Quantitative Parameters
				if (i % (int) pow(2U, j) == 0) {
					switch (qual_params[qual_param]) {
					case EXPOSURE_MODE:
						if (exp_config.exposure_mode.compare(device->getMaxValue(EXPOSURE_MODE)) == 0) {
							exp_config.exposure_mode = device->getMinValue(EXPOSURE_MODE);
						} else {
							exp_config.exposure_mode =
									device->getNextValue(EXPOSURE_MODE,exp_config.exposure_mode);
						}
						break;
					case WHITEBALANCE_MODE:
						if (exp_config.whitebalance_mode.compare(device->getMaxValue(WHITEBALANCE_MODE)) == 0) {
							exp_config.whitebalance_mode = device->getMinValue(WHITEBALANCE_MODE);
						} else {
							exp_config.whitebalance_mode =
									device->getNextValue(WHITEBALANCE_MODE,exp_config.whitebalance_mode);
						}
						break;
					case RADIO:
					case FLASH_MODE: // Ignore
					case COLOR_MODE:
					case WIDTH:
					case ZOOM:
					case NUM_CONFIG_PARAMS:
						break;
					}
				}

				quant_param++;
				qual_param++;
			}

				ss << exp_config.radio<< "\t";
				if(exp_config.radio.compare("wifi") == 0){
					ss << "\t";
				}
				ss << exp_config.flash_mode<< "\t";
				ss << exp_config.color_mode<< "\t";
				if(exp_config.radio.compare("RGB") == 0){
					ss << "\t\t";
				}
				ss << exp_config.width<< "\t";
				ss << exp_config.zoom<< "\t";
				ss << exp_config.exposure_mode<< "\t";
				if(exp_config.exposure_mode.compare("auto") == 0||
				   exp_config.exposure_mode.compare("center") == 0||
				   exp_config.exposure_mode.compare("night") == 0){
					ss << "\t";
				}
				ss << exp_config.whitebalance_mode << endl;

//				cout << device->getConfigNum(exp_config) << endl;

				// Put config in memo table
				Config c = device->getConfig(device->getConfigNum(exp_config));
				memoizeConfig(c);

				// Save DoE configs in seperate table
				doeList.push_back(c);
			}

	usedDoeConfigs = ss.str();
	doeConfigs = doeList;
}

int Strategy::getDoeSize() const {
	return this->doeSize;
}

void Strategy::setDoeSize(int doeSize) {
	this->doeSize = doeSize;
	initDoe = true;
	if(getName().find("doeinit")==-1){
	    stringstream ss;
	    ss << "+doeInit=" << getDoeSize();
		setName(getName()+ss.str());
	}

}

bool Strategy::getInitDoe() const{
	return initDoe;
}

bool Strategy::getOnlyNewRandom() const{
	return onlyNewRandom;
}

void Strategy::setOnlyNewRandom(bool onlyNewRandom){
	this->onlyNewRandom = onlyNewRandom;
	if(onlyNewRandom){
		if(getName().find("onlyNew")==-1)
			setName(getName()+"+onlyNew");
	}
}

void Strategy::setRandomInit(bool randomInit){
	this->randomInit = randomInit;
	if(randomInit){
		if(getName().find("randomInit")==-1){
		    stringstream ss;
		    ss << "+randomInit=" << getDoeSize();
			setName(getName()+ss.str());
		}
	}
}

bool Strategy::getDoeLimits() const
{
    return doeLimits;
}

void Strategy::setDoeLimits(bool doeLimits)
{
    this->doeLimits = doeLimits;
    if(this->doeLimits){
		if(getName().find("doeLimits")==-1)
			setName(getName()+"+doeLimits");
    }
}

/*
void Strategy::setTempLimit(double min, double max)
{
    stringstream ss;
    this->tmin = min;
    this->tmax = max;

    ss << min << ":" << max;
	setName(getName()+"_"+ss.str()+"tRange");
}*/

void Strategy::setTempFunc(temp_func func, float tp1, float tp2)
{
    stringstream ss;
    string funcName;

	this->func = func;
	this->tp1 = tp1;
	this->tp2 = tp2;

	switch(func){
	case CONSTANT:
		funcName = "cons";
		break;
	case LINEAR:
		funcName = "line";
		break;
	case EXPONENTIAL:
		funcName = "expo";
		break;
	}

    ss << funcName << ":" << tp1 << ":" << tp2;
	setName(getName()+"_"+ss.str()+"tfunc");
}





