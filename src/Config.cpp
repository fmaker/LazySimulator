/*
 * Config.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: fmaker
 */

#include "Config.h"
#include "random.h"
#include "Device.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
using namespace std;

Config::Config() {
}

Config::~Config() {
}


double Config::getEnergy() const{
	return this->energy;
}

string Config::getColor_mode() const
{
    return color_mode;
}

string Config::getExposure_mode() const
{
    return exposure_mode;
}

string Config::getFlash_mode() const
{
    return flash_mode;
}

double Config::getPower() const
{
    return power;
}

string Config::getWhitebalance_mode() const
{
    return whitebalance_mode;
}

string Config::getWidth() const
{
    return width;
}

void Config::setColor_mode(string color_mode)
{
    this->color_mode = color_mode;
}

void Config::setExposure_mode(string exposure_mode)
{
    this->exposure_mode = exposure_mode;
}

void Config::setFlash_mode(string flash_mode)
{
    this->flash_mode = flash_mode;
}

void Config::setPower(double power)
{
    this->power = power;
}

void Config::setWhitebalance_mode(string whitebalance_mode)
{
    this->whitebalance_mode = whitebalance_mode;
}

void Config::setWidth(string width)
{
    this->width = width;
}

int Config::getConfigNum() const
{
    return config_num;
}

string Config::getHeight() const
{
    return height;
}

string Config::getZoom() const
{
    return zoom;
}

void Config::setConfig_num(int config_num)
{
    this->config_num = config_num;
}

void Config::setEnergy(double energy)
{
    this->energy = energy;
}

void Config::setHeight(string height)
{
    this->height = height;
}

void Config::setZoom(string zoom)
{
    this->zoom = zoom;
}

double Config::getDuration() const
{
    return duration;
}

void Config::setDuration(double duration)
{
    this->duration = duration;
}

double Config::getQuality() const
{
    return quality;
}

void Config::setQuality(double quality)
{
    this->quality = quality;
}

string Config::getRadio() const
{
    return radio;
}

void Config::setRadio(string radio)
{
    this->radio = radio;
}

string Config::toString() const{
	stringstream ss;
	ss << "Config Num: " << config_num << endl;
	ss << "Energy: " << energy << endl;
	ss << "Quality: " << quality << endl;
	ss << "Power: " << power << endl;
	ss << "Radio: " << radio << endl;
	ss << "Duration: " << duration << endl;
	ss << "Color mode: " << color_mode << endl;
	ss << "Exposure mode: " << exposure_mode << endl;
	ss << "Flash mode: " << flash_mode << endl;
	ss << "Whitebalance mode: " << whitebalance_mode << endl;
	ss << "Zoom: " << zoom << endl;
	ss << "Width: " << width << endl;
	ss << "Height: " << height << endl;
	return ss.str();
}

ostream& operator<<(ostream& out, const Config& c){
	return out << c.toString();
}











