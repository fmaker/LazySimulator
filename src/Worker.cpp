/*
 * Worker.cpp
 *
 *  Created on: Dec 17, 2010
 *      Author: fmaker
 */

#include "Worker.h"

Worker::Worker() {

}

Worker::~Worker() {
}

concurrent_queue<Simulation*> *Worker::getQueue() const
{
    return queue;
}

void Worker::setQueue(concurrent_queue<Simulation*> *queue)
{
    this->queue = queue;
}

void Worker::run(){
	t = thread(&Worker::runSim, this);
//	runSim();
}

void Worker::join(){
	t.join();
}

void Worker::runSim(){
	int simNum = 1;
	Simulation *s = NULL;

	while(!queue->empty()){
		// Consume next simulation
		if(queue->try_pop(s)){
			cout << "Simulation #: " << simNum++ << endl;
			cout << *s << endl;
			s->run();
		}
	}
}


