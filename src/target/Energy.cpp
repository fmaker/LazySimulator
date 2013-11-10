/*
 * Energy.cpp
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#include "Energy.h"
using namespace std;

const string e = "energy";

Energy::Energy() {
	setName(e);
}

Energy::~Energy() {
}

void Energy::initLimits(device_ptr d){
	minValue = d->getMinEnergy();
	maxValue = d->getMaxEnergy();
}

double Energy::getValue(Config *c) const{
	return c->getEnergy();
}

target_ptr Energy::clone() const{
	return target_ptr(new Energy(*this));
}
