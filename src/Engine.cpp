/*
 * Engine
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#include "Engine.h"
#include "Logfile.h"
#include <boost/thread/thread.hpp>
#include <pthread.h>
#include <boost/filesystem.hpp>

Engine::Engine()
{
	// Initialize
	runs = 0;
#if 0
	threads = 1;
#else
	threads = boost::thread::hardware_concurrency();
#endif


}

Engine::~Engine()
{
	vector<Worker*>::iterator iw;
	for(iw=workers.begin();iw!=workers.end();iw++){
		delete *iw;
	}

	if(!queue.empty())
		cout << "Remaining Simulations: " << endl;
	while(!queue.empty()){
		Simulation *s;
		queue.wait_and_pop(s);
		cout << *s << endl;
		delete s;
	}
}

void Engine::run(){
	// Build list of workers
	for(int i=0;i<threads;i++){
		Worker *w = new Worker();
		w->setQueue(&queue);
		workers.push_back(w);
	}

	// Start threads
	vector<Worker*>::iterator iw;
	for(iw=workers.begin();iw!=workers.end();iw++){
		(*iw)->run();
	}

	// Wait for threads to complete
	for(iw=workers.begin();iw!=workers.end();iw++){
		(*iw)->join();
	}
}

void Engine::pause(){

}

void Engine::stop(){

}

void Engine::setSimFile(File *simFile){
	this->simFile = simFile;
}

File* Engine::getSimFile() const {
	return this->simFile;
}

void Engine::buildQueue(){
	cout << "Building simulation queue..." << endl;

	// Get lists of each factor
	int runs = getSimFile()->getNumRuns();
	int seed = getSimFile()->getSeed();
	int numTargets = getSimFile()->getNumTargets();
	list<const_device_ptr> devices = getSimFile()->getDevices();
	list<const_target_ptr> targets = getSimFile()->getTargets();
	list<const_strategy_ptr> strategies = getSimFile()->getStrategies();
	Logfile logfile = getSimFile()->getLogfile();

	// Copy config file to results directory
	string cpCmd = "cp "+getSimFile()->getSimFile()+" "+logfile.getOutputDir() + "config.xml";
	string rmCmd = "rm "+logfile.getOutputDir()+"../latest";
	string lnCmd = "ln -s "+logfile.getOutputDir()+" "+logfile.getOutputDir()+"../latest";
	string rdOnly = "chmod -w "+logfile.getOutputDir() + "config.xml";
	boost::filesystem::create_directories(logfile.getOutputDir());
	std::system(cpCmd.c_str());
	std::system(rdOnly.c_str());
	std::system(rmCmd.c_str());
	std::system(lnCmd.c_str());

	// Devices
	int simNumber = 1;
	list<const_device_ptr>::iterator id;
	for(id=devices.begin(); id!=devices.end(); id++){

		// Strategies
		list<const_strategy_ptr>::iterator is;
		for(is=strategies.begin(); is!=strategies.end(); is++){

			// Targets
			list<const_target_ptr>::iterator it;
			for(it=targets.begin(); it!=targets.end(); it++){

				// Runs
				for(int i=0;i<runs;i++){
					device_ptr d = (*id)->clone();
					d->connect();
					strategy_ptr s = (*is)->clone();
					target_ptr t = (*it)->clone();
					Simulation *sim = new Simulation(i,d,s,t);
					sim->setSeed(seed);
					sim->setNumTargets(numTargets);

					Logfile *l = new Logfile(logfile);
					l->setFilename((*id)->getName(),(*is)->getName(),(*it)->getName(),i);
					sim->setLogfile(l);
					cout << "\tAdded Simulation # " << simNumber++
							<< "(" << l->getFilename() << ")" << endl;
					queue.push(sim);
				}
			}
		}
	}
}

