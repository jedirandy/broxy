/*
 * Curl.h
 *
 *  Created on: Mar 24, 2015
 *      Author: sheng
 */
// Curl wrapper
#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

#include <curl/curl.h>
#include <string>
#include <iostream>

using namespace std;
// Curl helper class
class Curl {
private:
	CURL* curl;
public:
	Curl();
	virtual ~Curl();
	// sync GET
	std::string get(std::string url);
};

#endif /* SRC_UTIL_H_ */
