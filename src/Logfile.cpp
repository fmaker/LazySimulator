/*
 * Logfile.cpp
 *
 *  Created on: Dec 16, 2010
 *      Author: fmaker
 */

#include "Logfile.h"
#include <stdlib.h>
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

// Standard deliminator for gnuplot
const char defaultDelim = ' ';

Logfile::Logfile(){
}

Logfile::Logfile(string fileName)  	{
	Logfile();
	this->fileName = fileName;
	this->setDelim(defaultDelim);
}

Logfile::~Logfile() {
}

char Logfile::getDelim() const
{
    return delim;
}

void Logfile::setDelim(char delim)
{
    this->delim = delim;
}

string Logfile::getFilename() const
{
    return fileName;
}

void Logfile::setFilenameFormat(string filenameFormat)
{
    outputDir = filenameFormat;
    outputDir = filenameFormat + getTimeStamp("%m_%d_%Y_%H_%M_%S") + "/";
}

string Logfile::getOutputDir() const{
	return outputDir;
}

void Logfile::setFilename(string device,
				 string strategy,
				 string target,
				 int run){

	stringstream ss;
	ss << run;

    string filename = outputDir + filenameFormat;
    filename += device + "/";
    filename += strategy + "/";
    filename += ss.str() + "/";
    this->doeFileName = filename + target +"_doe.dat";
    filename += target + ".dat";
    this->fileName = filename;

}

string Logfile::getTimeStamp(const char* format)
{
	time_t now = time(0);
	const tm *t = localtime(&now);
	stringstream ss;
	const time_put<char>& dateWriter = 
		use_facet<time_put<char> >(ss.getloc());
	int n = strlen(format);
	dateWriter.put(ss,ss,' ',t,format,format+n);
	return ss.str();
}

void Logfile::init(){
	path logpath = path(fileName);
	create_directories(logpath.parent_path());
	outputFile = new ofstream(fileName.c_str(),ios_base::out);
	*outputFile << "#Run" << delim << "Target" << delim
				<< "Result" << delim << "Tries" << delim
				<< "Duration" << delim << "TableHits" << delim
				<< "Entries" << delim << "EnergyUsed" << delim
				<< "Success"<< delim << "TablePercent" << delim
				<< "HitPercent" << endl;
}

void Logfile::write(Result r){
	*outputFile << r.getTargetNumber() << delim << r.getTarget()->getValue() << delim
				<< r.getValue() << delim << r.getNumTries() << delim
				<< r.getDuration() << delim << r.getNumHits() << delim
				<< r.getTableSize() << delim << r.getTotalEnergy() << delim
				<< (r.getSuccess() ? 1 : 0) << delim
				<< r.getTablePercent() << delim << r.getHitPercent() <<endl;

}

void Logfile::close(){
	if(outputFile){
		outputFile->close();
		delete outputFile;
		outputFile = NULL;
	}
}


