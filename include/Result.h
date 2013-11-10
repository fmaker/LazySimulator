/*
 * Result.h
 *
 *  Created on: Dec 18, 2010
 *      Author: fmaker
 */

#ifndef RESULT_H_
#define RESULT_H_
#include "Config.h"
#include "Target.h"

class Result {
public:
	Result();
	virtual ~Result();
    int getNumTries() const;
    Config getConfig() const;
    void setNumTries(int numTries);
    void setConfig(Config c);
    void setTarget(const_target_ptr target);
    const_target_ptr getTarget() const;
    void setValue(double value);
    double getValue() const;
    int getTargetNumber() const;
    void setTargetNumber(int targetNumber);
    int getNumHits() const;
    void setNumHits(int numHits);
    int getTableSize() const;
    void setTableSize(int tableSize);
    double getDuration() const;
    void setDuration(double duration);
    double getTotalEnergy() const;
    void setTotalEnergy(double totalEnergy);
    bool getSuccess() const;
    double getTablePercent() const;
    void setTablePercent(double tablePercent);
    void setHitPercent(double hitPercent);
    double getHitPercent();
private:
	int numTries;
	int targetNumber;
	int numHits;
	int tableSize;
	Config config;
	const_target_ptr target;
	double value;
	double duration;
	double totalEnergy;
	double tablePercent;
	double hitPercent;
};

#endif /* RESULT_H_ */
