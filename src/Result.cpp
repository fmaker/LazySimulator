/*
 * Result.cpp
 *
 *  Created on: Dec 18, 2010
 *      Author: fmaker
 */

#include "Result.h"

Result::Result() {

}

Result::~Result() {
}

int Result::getNumTries() const
{
    return numTries;
}

Config Result::getConfig() const
{
    return config;
}

void Result::setNumTries(int numTries)
{
    this->numTries = numTries;
}

void Result::setConfig(Config config)
{
    this->config = config;
}
void Result::setTarget(const_target_ptr target){
	this->target = target;
}
const_target_ptr Result::getTarget() const{
	return target;
}
void Result::setValue(double value){
	this->value = value;
}
double Result::getValue() const{
	return value;
}

int Result::getTargetNumber() const
{
    return targetNumber;
}

void Result::setTargetNumber(int targetNumber)
{
    this->targetNumber = targetNumber;
}

int Result::getNumHits() const{
	return numHits;
}

void Result::setNumHits(int numHits){
	this->numHits = numHits;
}

int Result::getTableSize() const{
	return tableSize;
}

void Result::setTableSize(int tableSize){
	this->tableSize = tableSize;
}

double Result::getDuration() const
{
    return duration;
}

void Result::setDuration(double duration)
{
    this->duration = duration;
}

double Result::getTotalEnergy() const
{
    return totalEnergy;
}

void Result::setTotalEnergy(double totalEnergy)
{
    this->totalEnergy = totalEnergy;
}

bool Result::getSuccess() const{
	return (getValue() <= getTarget()->getUpperBound() &&
			getValue() >= getTarget()->getLowerBound());
}

double Result::getTablePercent() const
{
    return tablePercent;
}

void Result::setTablePercent(double tablePercent)
{
    this->tablePercent = tablePercent;
}


void Result::setHitPercent(double hitPercent)
{
    this->hitPercent = hitPercent;
}

double Result::getHitPercent()
{
    return hitPercent;
}








