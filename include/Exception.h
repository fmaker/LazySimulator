/*
 * Exception.h
 *
 *  Created on: Dec 14, 2010
 *      Author: fmaker
 */

#ifndef EXCEPTION_H_
#define EXCEPTION_H_

#include <string>
using namespace std;

class Exception {

public:
   Exception(const string& msg) : msg_(msg) {}
  ~Exception( ) {}

   string getMessage( ) const {return(msg_);}
private:
   string msg_;
};

#endif /* EXCEPTION_H_ */
