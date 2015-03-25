#ifndef SRC_CACHE_H_
#define SRC_CACHE_H_

#include <string>
#include <iostream>
#include <unordered_map>
#include <list>
#include <queue>
#include <deque>
#include <algorithm>
#include "Util.h"

#define MB 1024*1024
#define KB 1024

using namespace std;

enum CachePolicy {
	ZERO_POLICY,
	FIFO = 1,
	LRU = 2,
	MAXS = 3,
	RANDOM = 4,
	NUM_OF_POLICIES
};

// convert policy to string
std::string policy_to_string(CachePolicy p);

class Cache {
protected:
	unordered_map<string, string> cache_map;
	size_t max_size; // max capacity of the cache
	size_t size; // available space of the cache
	Curl curl; // Curl helper
	Stats stats; // Stats helper

	virtual bool add(const string& url, const string& data);
	virtual bool remove(const string& url);
	virtual string find(const string& url);

	virtual bool can_fit_in(size_t input_size);
	virtual void debug_info(bool is_hit);
public:
	Cache(size_t max_size);
	virtual ~Cache() {};

	virtual std::string fetch(const std::string& url) = 0;
	virtual size_t available();
	virtual std::string get_stats();
};

/*
 * FIFOCache
 */
class FIFOCache : public Cache {
private:
	std::list<std::string> list;
public:
	FIFOCache(size_t max_size);
	virtual ~FIFOCache();

	std::string fetch(const std::string& url) override;
	bool free(size_t input_size);
};

/*
 * LRUCache
 */
class LRUCache: public Cache {
private:
	std::deque<std::string> deque;
public:
	LRUCache(size_t max_size);
	virtual ~LRUCache();

	std::string fetch(const std::string& url) override;
	bool free(size_t input_size);
};

/*
 * MAXSCache
 */
class Entity {
public:
	size_t size;
	string url;

	Entity(string url, size_t size): size(size), url(url) {};
};

struct compareSize {
	bool operator()(const Entity& lhs, const Entity& rhs) const {
		return lhs.size < rhs.size;
	}
};

class MAXSCache: public Cache {
private:
	std::priority_queue<Entity, std::vector<Entity>, compareSize> pq;
public:
	MAXSCache(size_t max_size);
	virtual ~MAXSCache();

	std::string fetch(const std::string& url) override;
	bool free(size_t input_size);

	virtual std::string get_stats() override;
};

/*
 * Random Cache
 */
class RandomCache: public Cache {
private:
	std::list<std::string> list;
	RandomGenerator randomGenerator;
public:
	RandomCache(size_t max_size);
	virtual ~RandomCache();

	std::string fetch(const std::string& url) override;
	bool free(size_t input_size);
};

#endif /* SRC_CACHE_H_ */
