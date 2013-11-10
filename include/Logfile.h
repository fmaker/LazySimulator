/*
 * Logfile.h
 *
 *  Created on: Dec 16, 2010
 *      Author: fmaker
 */

#ifndef LOGFILE_H_
#define LOGFILE_H_

#include "Result.h"
#include "Target.h"
#include "Device.h"
#include "Strategy.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

static const string deviceSym="%d";
static const string strategySym="%s";
static const string targetSym="%t";
static const string runSym="%r";

class Logfile {

	public:
		Logfile();
		Logfile(string fileName);
		virtual ~Logfile();
		char getDelim() const;
		void setDelim(char delim);
		string getFilename() const;
		void setFilenameFormat(string fileName);
		void setFilename(string device,
				string strategy,
				string target,
				int run);	
		void init();
		void write(Result r);
		void close();
		string getOutputDir() const;
		string doeFileName;
	private:
		ofstream *outputFile;
		string fileName;
		string filenameFormat;
		string outputDir;
		char delim;
		string getTimeStamp(const char* format);

};

#endif /* LOGFILE_H_ */
