/*
 * Strategy.h
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#ifndef STRATEGY_H_
#define STRATEGY_H_
#include "Result.h"
#include "Config.h"
#include "Target.h"
#include "Logfile.h"
#include <string>
using namespace std;
#include <boost/shared_ptr.hpp>

typedef enum
{
	ENERGY,
	DURATION,
	MEMORY
}limit_type;

const int ENTRY_MEMORY = 4;

typedef struct
{
	limit_type amountType;
	double amount;
}limit_t;


typedef enum
{
	LINEAR,
	CONSTANT,
	EXPONENTIAL
}temp_func;

class Strategy {
public:
	Strategy();
	virtual ~Strategy();
	virtual boost::shared_ptr<Strategy> clone() const = 0;
	void initMemo(const_device_ptr device);
    void init(const_target_ptr target, const_device_ptr device);
    bool getMemo() const;
    void setMemo(bool memo);
    bool getLimited() const;
    void setLimited(bool lookup);
    string getName() const;
    void setName(string name);
    string toString() const;
    virtual Result search(const_target_ptr target, const_device_ptr device) = 0;
    int getSeed() const;
    void setSeed(int seed);
    int getTries() const;
    void setTries(int tries);
    bool lookupConfig(int configNum, Config &config);
    void resetNumHits();
    int getNumHits() const;
    void setNumHits(int numHits);
    double getDuration() const;
    double getTotalEnergy() const;
    int getTableSize() const;
    double getMemoMax(const_target_ptr target);
    double getMemoMin(const_target_ptr target);
    vector<bool> memoTableEntry;
    void setDoeSize(int doeSize);
    bool getOnlyNewRandom() const;
    void setOnlyNewRandom(bool onlyNewRandom);
    void setRandomInit(bool randomInit);
    Config getConfigRandom(const_device_ptr device);
    limit_t getLimitType() const;
    void setLimitType(limit_t lookupType);
    bool findNearestConfig(const_target_ptr target, Config &foundConfig);
	bool fillToHitPercent;
	double getHitPercent(const_target_ptr target);
    double getHitPercentGoal() const;
    void setHitPercentGoal(double hitPercentGoal);
    bool doeInitialized;
	vector<Config> getMemoTable() const;
    bool getDoeLimits() const;
    void setDoeLimits(bool doeLimits);
    string usedDoeConfigs;
    void setTempFunc(temp_func func, float tp1, float tp2);
protected:
    virtual void printStatusHeader() const;
    void memoizeConfig(Config c);
    static const int NUM_STATUS_PRINT = 10;
    int numHits;
    double duration;
    //double lowerBound;
    //double upperBound;
    vector<int> remainingConfigs;
    Config getNewConfigRandom(const_device_ptr device);
	bool initialized;
	void doeInitRandom(const_device_ptr device);
	void doeInit(const_device_ptr device);
    int getDoeSize() const;
    bool getInitDoe() const;
    bool randomInit;
    bool onlyNewRandom;
	bool overLimit;
    int tries;
    temp_func func;
	double tp1,tp2;
    list<Config> doeConfigs;
    double totalEnergy;
private:
    bool doeLimits;
    void saveConfig(Config &config);
    bool memo;
    bool limited;
    string name;
    int seed;
    vector<Config> memoTable;
	int doeSize;
	bool initDoe;
	limit_t limitType;
	double hitPercentGoal;
};

typedef boost::shared_ptr<Strategy> strategy_ptr;
typedef boost::shared_ptr<const Strategy> const_strategy_ptr;

ostream& operator<<(ostream& out, const Strategy& d);







#endif /* STRATEGY_H_ */
