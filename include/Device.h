/*
 * Device.h
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <string>
#include <list>
#include <vector>
using namespace std;

#include <soci/soci.h>
#include <soci/sqlite3/soci-sqlite3.h>
using namespace soci;

#include <boost/shared_ptr.hpp>

#include <stdlib.h>
#include "Config.h"

const string energyColumn = "energy";
const string tableName = "results";

class Device {
public:
	Device(string dbFile);
	virtual ~Device();
	boost::shared_ptr<Device> clone() const;
	void connect();
	void disconnect();

	string getDbFile() const;
	string getName() const;
	void setDbFile(string dbFile);
	void setName(string name);

	string toString() const;

	double getMaxEnergy() const;
	double getMinEnergy() const;
	double getMaxQuality() const;
	double getMinQuality() const;
	double getMaxRatio() const;
	double getMinRatio() const;
	int getNumConfigs() const;
	Config getConfig(int configNum) const;
	int getConfigNum(cparams_t params) const;
    Config getConfigRandom() const;
	Config getRandomNeighbor(Config config) const;
	string getMaxValue(config_param p) const;
	string getMinValue(config_param p) const;
	string getNextValue(config_param p,string value) const;
	string getRandomParam(config_param p) const;
	const string* getParamArray(config_param p, int &size) const;
	const vector<string>* getParamVector(config_param p) const;
private:
	session *sql;
	string dbFile;
	string name;
	int numConfigs;
	// Vectors for device parameters in db
	vector<string> radio;
	vector<string> width;
	vector<string> zoom;
	vector<string> exposure_mode;
	vector<string> flash_mode;
	vector<string> color_mode;
	vector<string> whitebalance_mode;

	void init();
	vector<string>* getParamVectorW(config_param p);
};

typedef boost::shared_ptr<Device> device_ptr;
typedef boost::shared_ptr<const Device> const_device_ptr;

ostream& operator<<(ostream& out, const Device& d);

#endif /* DEVICE_H_ */
