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
#include <random>

#define CURL_TIMEOUT 5L // timeout in seconds

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

// Stats helper class
class Stats {
public:
	uint32_t hits;
	uint32_t requests;

	Stats(): hits(0), requests(0) {};
	virtual ~Stats() {};

	void count_hit();
	void count_total();
	double get_rate();
};

// Random generator
class RandomGenerator {
private:
	mt19937 mt;
public:
	RandomGenerator();
	virtual ~RandomGenerator(){};
	int get_number(int from, int to);
};

#endif /* SRC_UTIL_H_ */
