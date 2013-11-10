/*
 * Worker.h
 *
 *  Created on: Dec 17, 2010
 *      Author: fmaker
 */

#ifndef WORKER_H_
#define WORKER_H_
#include "concurrent_queue.h"
#include "Simulation.h"
#include <pthread.h>
#include <boost/thread.hpp>
using namespace boost;

class Worker {
public:
	Worker();
	virtual ~Worker();
    concurrent_queue<Simulation*> *getQueue() const;
    void setQueue(concurrent_queue<Simulation*> *queue);
    void run();
    void join();
private:
	concurrent_queue<Simulation*> *queue;
	thread t;
    void runSim();
};

#endif /* WORKER_H_ */
