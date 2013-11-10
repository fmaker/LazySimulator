/*
 * Device.cpp
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#include <iostream>
#include <limits>
using namespace std;

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include "Device.h"
#include "Exception.h"
#include "Config.h"
#include "random.h"

Device::Device(string dbFile) {
	numConfigs = 0;

	// Check if database file exists
	if(!exists(dbFile)){
		throw Exception("Device database: "+dbFile+" doesn't exist");
	}

	// Database file
	this->setDbFile(dbFile);

	// Set name parameter as filename
	path p(dbFile);
	this->setName(p.stem().string());

	//connect();
}

boost::shared_ptr<Device> Device::clone() const {
	return device_ptr(new Device(*this));
}

Device::~Device() {
//	delete sql;
}

void Device::connect(){

	// Create session for database
	try{
		sql = new session(sqlite3, this->getDbFile());
		init();

	}
	catch (exception const &e){
		cerr << "Exception: " << e.what() << endl;
	}
}

void Device::disconnect(){
	sql->close();
}

string Device::getDbFile() const
{
    return dbFile;
}

string Device::getName() const
{
    return name;
}

void Device::setDbFile(string dbFile)
{
    this->dbFile = dbFile;
}

void Device::setName(string name)
{
    this->name = name;
}

string Device::toString() const{
	return getName();
}

ostream& operator<<(ostream& out, const Device& d){
	return out << d.toString();
}

void Device::init(){

	// Set number of configurations
	*sql << "SELECT COUNT(*) FROM "<< tableName <<";", into(numConfigs);

	// Iterate through param vectors and save values
	for(int i=0;i<NUM_CONFIG_PARAMS;i++){
		stringstream ss;
		ss << "SELECT DISTINCT " << colNames[i] << " FROM results;";
		rowset<string> rs = (sql->prepare << ss.str());

	    vector<string>* v = getParamVectorW((config_param) i);
		for (rowset<string>::const_iterator it = rs.begin(); it != rs.end(); ++it)
		{
		     v->push_back(*it);
		}
#ifdef DEBUG
		cout << "size of " << colNames[i] << " = " << v->size() << endl;
#endif
	}


}

const vector<string>* Device::getParamVector(config_param p) const{
	const vector<string>* paramVectors[] =
		{&radio,&exposure_mode,&flash_mode,&color_mode,&width,&whitebalance_mode,&zoom};
	return paramVectors[p];
}

// Writeable b/c C++ transfers const vector to contents (ARG!)
vector<string>* Device::getParamVectorW(config_param p){
	vector<string>* paramVectors[] =
		{&radio,&exposure_mode,&flash_mode,&color_mode,&width,&whitebalance_mode,&zoom};
	return paramVectors[p];
}

string Device::getRandomParam(config_param p) const{
	//int size;
	int choice;
	//const string* param_array = getParamArray(p, size);
	const vector<string> *param_vector = getParamVector(p);
	choice = rand() % param_vector->size();
	return param_vector->at(choice);
}

const string* Device::getParamArray(config_param p, int &size) const{
	const string* paramArrays[] =
		{radio_array,exposure_mode_array,flash_mode_array,color_mode_array,
				width_array,whitebalance_mode_array,zoom_array};
	switch(p){
		case RADIO:
			size = size_str(radio_array);
			break;
		case EXPOSURE_MODE:
			size = size_str(exposure_mode_array);
			break;
		case FLASH_MODE:
			size = size_str(flash_mode_array);
			break;
		case COLOR_MODE:
			size = size_str(color_mode_array);
			break;
		case WIDTH:
			size = size_str(width_array);
			break;
		case WHITEBALANCE_MODE:
			size = size_str(whitebalance_mode_array);
			break;
		case ZOOM:
			size = size_str(zoom_array);
			break;
		case NUM_CONFIG_PARAMS:	// Ignore
		default:
			break;
	}
	return paramArrays[p];
}

// Using order of array
string Device::getMaxValue(config_param p) const{
	int size;
	const string* pa = getParamArray(p,size);
	const vector<string>* paramVectors[] =
		{&radio,&exposure_mode,&flash_mode,&color_mode,&width,&whitebalance_mode,&zoom};
	const vector<string>* pv = paramVectors[p];

	// Search backwards in paramVals until max is found in vector
	vector<string>::const_iterator it;
	for(int i=size-1;i>=0;i--){
		for(it=pv->begin();it!=pv->end();it++){
			if(pa[i].compare(*it) == 0)
				return *it;
		}
	}
	return "";
}

string Device::getMinValue(config_param p) const{
	int size;
	const string* pa = getParamArray(p,size);
	const vector<string>* paramVectors[] =
		{&radio,&exposure_mode,&flash_mode,&color_mode,&width,&whitebalance_mode,&zoom};
	const vector<string>* pv = paramVectors[p];

	// Search backwards in paramVals until max is found in vector
	vector<string>::const_iterator it;
	for(int i=0;i<size;i++){
		for(it=pv->begin();it!=pv->end();it++){
			if(pa[i].compare(*it) == 0)
				return *it;
		}
	}
	return "";
}

string Device::getNextValue(config_param p,string value) const{
	const vector<string>* paramVectors[] =
		{&radio,&exposure_mode,&flash_mode,&color_mode,&width,&whitebalance_mode,&zoom};
	const vector<string>* v = paramVectors[p];
	string retValue = "";

	// If last value, return first
	if(v->back().compare(value) == 0){
		retValue = v->front();
	}
	// Otherwise search until find value then return the next one
	else{
		vector<string>::const_iterator it;
		for(it=v->begin();it!=v->end();it++){
			if(it->compare(value) == 0){
				retValue = *(++it);
			}
		}
	}
	return retValue;
}

double Device::getMaxEnergy() const{
	double maxEnergy;
	sql->once << "SELECT MAX(energy) FROM "+tableName+";",into(maxEnergy);
	return maxEnergy;
}

double Device::getMinEnergy() const{
	double minEnergy;
	sql->once << "SELECT MIN(energy) FROM "+tableName+" WHERE energy > 0;",into(minEnergy);
	return minEnergy;
}

double Device::getMaxQuality() const{
	double maxQuality;
	sql->once << "SELECT MAX(quality) FROM "+tableName+";",into(maxQuality);
	return maxQuality;
}

double Device::getMinQuality() const{
	double minQuality;
	sql->once << "SELECT MIN(quality) FROM "+tableName+" WHERE quality > 0;",into(minQuality);
	return minQuality;
}

double Device::getMaxRatio() const{
	double maxRatio;
	sql->once << "SELECT MAX(quality/energy) FROM "
			  << tableName << " WHERE quality > 0 AND energy > 0;",into(maxRatio);
	return maxRatio;
}

double Device::getMinRatio() const{
	double minRatio;
	sql->once << "SELECT MIN(quality/energy) FROM "
			  << tableName << " WHERE quality > 0 AND energy > 0;",into(minRatio);
	return minRatio;
}

int Device::getNumConfigs() const{
	return this->numConfigs;
}

Config Device::getConfig(int configNum) const{
	Config c;
	string exposure_mode,flash_mode,color_mode,
		   whitebalance_mode,radio,width,height,zoom;
	double power,energy,duration,quality;

	stringstream ss;
	ss << "SELECT "
	   << "radio,"
	   << "exposure_mode,"
	   << "flash_mode,"
	   << "color_mode,"
	   << "width,"
	   << "height,"
	   << "whitebalance_mode,"
	   << "zoom,"
	   << "power,"
	   << "energy,"
	   << "duration,"
	   << "quality "
	   << "FROM "<< tableName
	   <<" WHERE config_num=" << configNum << ";";

	string configParams = ss.str();

/*	radio = "bluetooth";
	exposure_mode = "auto";
	flash_mode = "auto";
	color_mode = "auto";
	width = "640";
	height="480";
	whitebalance_mode = "auto";
	zoom = "72";
	energy = 10;
	power = 10;
	quality = 10;
	duration = 10;*/

	sql->once << configParams,
			into(radio),into(exposure_mode),into(flash_mode),
			into(color_mode),into(width),into(height),
			into(whitebalance_mode),into(zoom),into(power),
			into(energy),into(duration),into(quality);

	// Fill in parameters
	c.setConfig_num(configNum);
	c.setExposure_mode(exposure_mode);
	c.setFlash_mode(flash_mode);
	c.setColor_mode(color_mode);
	c.setWidth(width);
	c.setHeight(height);
	c.setWhitebalance_mode(whitebalance_mode);
	c.setZoom(zoom);
	c.setPower(power);
	c.setEnergy(energy);
	c.setRadio(radio);
	c.setDuration(duration);
	c.setQuality(quality);

	return c;
}

int Device::getConfigNum(cparams_t params) const{
	int count,config_num;
	stringstream ss;
	ss << "SELECT "
	   << "count(),"
	   << "config_num"
	   << " FROM "<< tableName
	   << " WHERE "
	   << "radio=" << '"' << params.radio << '"' << " AND "
	   << "exposure_mode=" << '"' << params.exposure_mode << '"' << " AND "
	   << "flash_mode=" << '"' << params.flash_mode << '"' << " AND "
	   << "color_mode=" << '"' << params.color_mode << '"' << " AND "
	   << "width=" << '"' << params.width << '"' << " AND "
	   << "whitebalance_mode=" << '"' << params.whitebalance_mode << '"' << " AND "
	   << "zoom=" << '"' << params.zoom << '"'
	   << ";";

	sql->once << ss.str(),into(count),into(config_num);

	assert(count == 1);

	return config_num;
}

Config Device::getRandomNeighbor(Config config) const{
	Config neighbor = Config(config);
	string newChoiceStr;
	bool changed = false;
	while(!changed){
		switch(randint0(NUM_CONFIG_PARAMS)){
			case RADIO:
				if(radio.size()-1 > 0){
					newChoiceStr = radio[randint(radio.size()-1)];
					if(neighbor.getRadio().compare(newChoiceStr) != 0){
						neighbor.setRadio(newChoiceStr);
						changed = true;
					}
				}
				break;
			case EXPOSURE_MODE:
				if(exposure_mode.size()-1 > 0){
					newChoiceStr = exposure_mode[randint(exposure_mode.size()-1)];
					if(neighbor.getExposure_mode().compare(newChoiceStr) != 0){
						neighbor.setExposure_mode(newChoiceStr);
						changed = true;
					}
				}
				break;
			case FLASH_MODE:
				if(flash_mode.size()-1 > 0){
					newChoiceStr = flash_mode[randint(flash_mode.size()-1)];
					if(neighbor.getFlash_mode().compare(newChoiceStr) != 0){
						neighbor.setFlash_mode(newChoiceStr);
						changed = true;
					}
				}
				break;
			case COLOR_MODE:
				if(color_mode.size()-1 > 0){
					newChoiceStr = color_mode[randint(color_mode.size()-1)];
					if(neighbor.getColor_mode().compare(newChoiceStr) != 0){
						neighbor.setColor_mode(newChoiceStr);
						changed = true;
					}
				}
				break;
			case WHITEBALANCE_MODE:
				if(whitebalance_mode.size()-1 > 0){
					newChoiceStr = whitebalance_mode[randint(whitebalance_mode.size()-1)];
					if(neighbor.getWhitebalance_mode().compare(newChoiceStr) != 0){
						neighbor.setWhitebalance_mode(newChoiceStr);
						changed = true;
					}
				}
				break;
			case WIDTH:
				if(width.size()-1 > 0){
					newChoiceStr = width[randint(width.size()-1)];
					if(neighbor.getWidth() != newChoiceStr){
						neighbor.setWidth(newChoiceStr);
						changed = true;
					}
				}
				break;
			case ZOOM:
				if(zoom.size()-1 > 0){
					newChoiceStr = zoom[randint(zoom.size()-1)];
					if(neighbor.getZoom() != newChoiceStr){
						neighbor.setZoom(newChoiceStr);
						changed = true;
					}
				}
				break;
			}
		}

	cparams_t p;
	p.color_mode = neighbor.getColor_mode();
	p.exposure_mode = neighbor.getExposure_mode();
	p.flash_mode = neighbor.getFlash_mode();
	p.radio = neighbor.getRadio();
	p.whitebalance_mode = neighbor.getWhitebalance_mode();
	p.width = neighbor.getWidth();
	p.zoom = neighbor.getZoom();

	return getConfig(getConfigNum(p));
}

Config Device::getConfigRandom() const{
	return getConfig(randint(getNumConfigs()));
}
