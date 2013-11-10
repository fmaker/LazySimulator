/*
 * EnergyDiurnal.cpp
 *
 *  Description: Solar power model using sine wave with amplitude of
 *  			 14.2 Klux (measured emprically) resulting in 1W at 4V
 *
 *  Created on: Dec 15, 2010
 *      Author: fmaker
 */

#include "EnergyDiurnal.h"

const string ed = "energy_diurnal";

EnergyDiurnal::EnergyDiurnal() {
	setName(ed);
}

EnergyDiurnal::~EnergyDiurnal() {
}

auto_ptr<Target> EnergyDiurnal::buildTarget(){
	return auto_ptr<Target>(new EnergyDiurnal());
}
