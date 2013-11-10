/*
 * Target.cpp
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#include "Target.h"
#include <stdlib.h>
#include <math.h>
#include <iostream>

Target::Target() {
	percent = 0.0;
}

Target::~Target() {
}

void Target::setValueRandom(){
	setValue(randdouble(getMinValue(),getMaxValue()));
}

void Target::setValueRandom(double min,double max){
	setValue(randdouble(min,max));
}

void Target::setSeed(unsigned int seed)
{
    this->seed = seed;
//    srand(seed);
	// ASSUMES 64-BIT!
//	unsigned short seed16v[] = {0x0000,
//								(seed & 0xFFFF0000) >> 16,
//								(seed & 0xFFFF)};
//	seed48_r(seed16v, &seedbuf);

}

string Target::getName() const
{
    return name;
}

void Target::setName(string name)
{
    this->name = name;
}

string Target::toString() const{
	return getName();
}

ostream& operator<<(ostream& out, const Target& t){
	return out << t.toString();
}

double Target::getMaxValue() const
{
    return maxValue;
}

// Minimum non-zero value
double Target::getMinValue() const
{
    return minValue;
}

double Target::getValue() const
{
    return value;
}

double Target::getValueDelta(Config *c) const{
	double delta = fabs(getValue() - getValue(c));
	return delta;
}

double Target::getValueDelta(double value) const{
	double delta = fabs(getValue() - value);
	return delta;
}

void Target::setValue(double value)
{
    this->value = value;
}

// This method has no meaning for the superclass
void Target::initLimits(device_ptr d){
	minValue = 0;
	maxValue = 0;
}

// From: http://www.geekpedia.com/tutorial39_Random-Number-Generation.html
//generates a psuedo-random double between 0.0 and 0.999...
double Target::randdouble()
{
//    return rand()/(double(RAND_MAX)+1);
	return rand_r(&seed)/(double(RAND_MAX)+1);
//	double result;
//	drand48_r(&seedbuf,&result);
//	return result;
}

//generates a psuedo-random double between 0.0 and max
double Target::randdouble(double max)
{
    return randdouble()*max;
}

//generates a psuedo-random double between min and max
double Target::randdouble(double min, double max)
{
    if (min>max)
    {
        return randdouble()*(min-max)+max;
    }
    else
    {
        return randdouble()*(max-min)+min;
    }
}

float Target::getPercent() const
{
    return percent;
}

void Target::setPercent(float percent)
{
    this->percent = percent;
}

double Target::getUpperBound() const{
	return getValue() * (1 + getPercent());
}

double Target::getLowerBound() const{
	return getValue() * (1 - getPercent());
}

bool Target::withinBounds(Config c) const{
	return (getValue(&c) < getUpperBound() &&
			getValue(&c) > getLowerBound());
}



