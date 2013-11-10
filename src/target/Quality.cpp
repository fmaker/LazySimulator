/*
 * Quality.cpp
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#include "Quality.h"

const string q = "quality";

Quality::Quality() {
	setName(q);
}

Quality::~Quality() {
}

void Quality::initLimits(device_ptr d){
//	Target::setDevice(d);
	minValue = d->getMinQuality();
	maxValue = d->getMaxQuality();
}

double Quality::getValue(Config *c) const{
	return c->getQuality();
}

target_ptr Quality::clone() const{
	return target_ptr(new Quality(*this));
}
