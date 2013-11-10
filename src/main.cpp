//============================================================================
// Name        : LazySimulator.cpp
// Author      : Frank Maker
// Version     :
// Copyright   : (c) 2010, Frank Maker
// Description : Lazy Simulator
//============================================================================

#include <iostream>
#include <string>
#include <stdlib.h>
#include "File.h"
#include "Engine.h"
#include "Exception.h"
using namespace std;

#include  <boost/filesystem.hpp>
namespace fs = boost::filesystem;

int main(int argc, char* argv[]) {
	if(argc != 2){
		cout << "Usage: " << argv[0] << " simulation.xml";
	}
	else{
		string simulation_file = argv[1];
//		fs::path p(argv[1]);
//		fs::path full_p =  fs::complete(p);
//		simulation_file =  full_p.string();
//		cout << simulation_file << endl;
		try{
			File file = File(simulation_file);
			file.parse();

			Engine engine;
			engine.setSimFile(&file);
			engine.buildQueue();
			engine.run();


		}
		catch (Exception &e){
			cerr << e.getMessage() << endl;
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
