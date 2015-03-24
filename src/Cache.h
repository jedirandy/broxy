#ifndef SRC_CACHE_H_
#define SRC_CACHE_H_

#include <string>
#include <iostream>
#include <unordered_map>
#include <list>
#include "Util.h"

using namespace std;

class Cache {
protected:
	unordered_map<string, string> cache_map;
	size_t max_size;
	size_t size;
	Curl curl;

	virtual bool add(string url, string data);
	virtual bool remove(string url);
	virtual string find(string url);

	virtual bool free(size_t input_size) = 0; // make space
	virtual bool can_fit_in(size_t input_size);
public:
	Cache(): cache_map(), max_size(0), size(0), curl() {};
	virtual ~Cache() {};
	virtual std::string fetch(std::string url) = 0;
	virtual size_t available();
};

/*
 * FIFOCache
 */
class FIFOCache : public Cache {
private:
	std::list<std::string> list;
public:
	FIFOCache();
	FIFOCache(size_t max_size);
	virtual ~FIFOCache();

	std::string fetch(std::string url);
	bool free(size_t input_size);
};

/*
 * LRUCache
 */
class LRUCache: public Cache {
private:
	std::list<std::string> list;
public:
	LRUCache();
	LRUCache(size_t max_size);
	virtual ~LRUCache();
};

#endif /* SRC_CACHE_H_ */
