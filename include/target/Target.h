/*
 * Target.h
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#ifndef TARGET_H_
#define TARGET_H_
#include "Device.h"
#include "Config.h"
#include <string>
using namespace std;
#include <boost/shared_ptr.hpp>
#include <stdlib.h>

class Target {
public:
	Target();
	virtual ~Target();
	virtual boost::shared_ptr<Target> clone() const = 0;
	void setSeed(unsigned int seed);
    string getName() const;
    void setName(string name);
    string toString() const;
    void setValueRandom();
    void setValueRandom(double min,double max);
    double getMaxValue() const;
    double getMinValue() const;
    virtual void initLimits(device_ptr d) = 0;
    virtual double getValue(Config *c) const = 0;
    double getValueDelta(Config *c) const;
    double getValueDelta(double value) const;
    double getValue() const;
    void setValue(double value);
    float getPercent() const;
    void setPercent(float percent);
    double getUpperBound() const;
    double getLowerBound() const;
    bool withinBounds(Config c) const;
    double randdouble();
    double randdouble(double max);
    double randdouble(double min, double max);
protected:
	double maxValue;
	double minValue;
private:
	unsigned int seed;
	float percent;
	string name;
	double value;

};

typedef boost::shared_ptr<Target> target_ptr;
typedef boost::shared_ptr<const Target> const_target_ptr;

ostream& operator<<(ostream& out, const Target& t);

#endif /* TARGET_H_ */
