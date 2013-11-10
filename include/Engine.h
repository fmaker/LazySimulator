/*
 * Engine.h
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#ifndef ENGINE_H_
#define ENGINE_H_

#include "concurrent_queue.h"
#include "Simulation.h"
#include "File.h"
#include "Worker.h"
#include <vector>
using namespace std;

class Engine
{
public:
	Engine();
	virtual ~Engine();

	void setSimFile(File *s);
	File* getSimFile() const;

	void buildQueue();

	// State control
	void run();
	void stop();
	void pause();
private:
	// Simulation variables
	int runs;
	int threads;
	vector<Worker*> workers;
	File *simFile;
	concurrent_queue<Simulation*> queue;

};


#endif /* ENGINE_H_ */
