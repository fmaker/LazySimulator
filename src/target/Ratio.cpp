/*
 * Ratio.cpp
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#include "Ratio.h"

const string r = "ratio";

Ratio::Ratio() {
	setName(r);
}

Ratio::~Ratio() {
}

void Ratio::initLimits(device_ptr d){
//	Target::setDevice(d);
	minValue = d->getMinRatio();
	maxValue = d->getMaxRatio();
}

double Ratio::getValue(Config *c) const{
	return (c->getQuality()/c->getEnergy());
}

target_ptr Ratio::clone() const{
	return target_ptr(new Ratio(*this));
}

