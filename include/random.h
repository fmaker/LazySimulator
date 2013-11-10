/*
 * random.h
 *
 *  Created on: Dec 31, 2010
 *      Author: fmaker
 */

// http://www.geekpedia.com/tutorial39_Random-Number-Generation.html

#ifndef RANDOM_H_
#define RANDOM_H_
#include <stdlib.h>
#include <assert.h>

//generates a psuedo-random integer between 1 and max
inline int randint(int max)
{
	assert(max > 0);
    return ((rand()%(max))+1);
};

//generates a psuedo-random integer between 0 and max-1
inline int randint0(int max)
{
    return ((rand()%(max)));
};

//generates a psuedo-random float between 0.0 and 0.999...
inline float randfloat()
{
    return rand()/(float(RAND_MAX)+1);
}

#endif /* RANDOM_H_ */
