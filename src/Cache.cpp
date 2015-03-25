#include "Cache.h"

std::string policy_to_string(CachePolicy p) {
	switch(p) {
	case FIFO:
		return "FIFO";
	case LRU:
		return "LRU";
	case MAXS:
		return "MAXS";
	case RANDOM:
		return "Random";
	default:
		return "N/A";
	}
}

/*
 * Cache abstract class
 */
Cache::Cache(size_t max_size) :
		cache_map(), max_size(max_size), size(max_size), curl(), stats() {
}

bool Cache::add(const string& url, const string& data) {
	std::pair<string, string> kv(url, data);
	this->cache_map.insert(kv);
	this->size -= data.size();
	return true;
}

bool Cache::remove(const string& url) {
	auto element = cache_map.find(url);
	if (element != cache_map.end()) {
#ifdef _DEBUG
		cout << "removing " << url << endl;
#endif
		this->size += element->second.size();
		cache_map.erase(url);
		return true;
	}
	// not found
	return false;
}

string Cache::find(const string& url) {
	string result;
	auto element = cache_map.find(url);
	stats.count_total();
	if (element != cache_map.end()) {
		// hit
		stats.count_hit();
		result = element->second;
	}
	return result;
}

bool Cache::can_fit_in(size_t input_size) {
	if (input_size < this->size)
		return true;
	return false;
}

size_t Cache::available() {
	return this->size;
}

std::string Cache::get_stats() {
	return "hits/total: " + std::to_string(stats.hits) + "/"
			+ std::to_string(stats.requests) + "\n"
			+ "hit rate: " + std::to_string(stats.get_rate()) + "\n"
			+ "number of cached elements: " + std::to_string(cache_map.size()) + "\n"
			+ "available/total size: " + std::to_string(size/KB) + "/"
			+ std::to_string(max_size/KB) + " KBytes\n";
}

void Cache::debug_info(bool is_hit) {
	if (is_hit) {
		cout << "Cache hit" << endl;
	} else {
		cout << "Cache miss" << endl;
	}
}
/*
 * FIFO Class
 */
FIFOCache::FIFOCache(size_t max_size) :
		Cache(max_size) {
}

FIFOCache::~FIFOCache() {
	this->list.clear();
}

std::string FIFOCache::fetch(const std::string& url) {
	string result;
	result = find(url);
#ifdef _DEBUG
	debug_info(!result.empty());
#endif
	if (result.empty()) {
		result = this->curl.get(url);
		if (free(result.size())) {
			// update cache
			add(url, result);
			// add to the queue
			list.push_back(url);
		}
	}
	return result;
}

bool FIFOCache::free(size_t input_size) {
	if (input_size > this->max_size)
		return false;
	if (input_size < this->size) {
		return true;
	}
	while (!can_fit_in(input_size)) {
		remove(list.front());
		list.pop_front();
	}
	return true;
}

/*
 * LRU Cache
 */
LRUCache::LRUCache(size_t max_size) :
		Cache(max_size) {
}

LRUCache::~LRUCache() {
}

string LRUCache::fetch(const string& url) {
	string result;
	result = find(url);
#ifdef _DEBUG
	debug_info(!result.empty());
#endif
	if (result.empty()) {
		result = this->curl.get(url);
		if (free(result.size())) {
			// update cache
			add(url, result);
			// add to the queue
			deque.push_front(url);
		}
	} else {
		// update deque
		auto i = std::find(deque.begin(), deque.end(), url);
		auto copy = *i;
		deque.erase(i);
		deque.push_front(copy);
	}

	return result;
}

bool LRUCache::free(size_t input_size) {
	if (input_size > this->max_size)
		return false;
	if (input_size < this->size) {
		return true;
	}
	while (!can_fit_in(input_size)) {
		remove(deque.back());
		deque.pop_back();
	}
	return true;
}

/*
 * MAXS Cache
 */
MAXSCache::MAXSCache(size_t max_size) :
		Cache(max_size) {
}

MAXSCache::~MAXSCache() {
}

std::string MAXSCache::fetch(const string& url) {
	string result;
	result = find(url);
#ifdef _DEBUG
	debug_info(!result.empty());
#endif
	if (result.empty()) {
		result = this->curl.get(url);
		if (free(result.size())) {
			// update cache
			add(url, result);
			// add to the priority queue
			size_t size = result.size();
			Entity e(url, size);
			pq.push(e);
		}
	}
	return result;
}

bool MAXSCache::free(size_t input_size) {
	if (input_size > this->max_size)
		return false;
	if (input_size < this->size) {
		return true;
	}
	while (!can_fit_in(input_size)) {
		remove(pq.top().url);
		pq.pop();
	}
	return true;
}

std::string MAXSCache::get_stats() {
	std::string stats = Cache::get_stats();
	if (!pq.empty())
		stats += "largest size item: " + pq.top().url + "\n";
	return stats;
}

/*
 * Random Cache
 */
RandomCache::RandomCache(size_t max_size) :
		Cache(max_size), randomGenerator() {
}

RandomCache::~RandomCache() {
}

std::string RandomCache::fetch(const string& url) {
	string result;
	result = find(url);
#ifdef _DEBUG
	debug_info(!result.empty());
#endif
	if (result.empty()) {
		result = this->curl.get(url);
		if (free(result.size())) {
			// update cache map
			add(url, result);
			// add
			list.push_back(url);
		}
	}
	return result;
}

bool RandomCache::free(size_t input_size) {
	if (input_size > this->max_size)
		return false;
	if (input_size < this->size) {
		return true;
	}
	while (!can_fit_in(input_size)) {
		auto iter = list.begin();
		int index = randomGenerator.get_number(0, list.size() - 1);
		std::advance(iter, index);
		remove(*iter);
		list.erase(iter);
	}
	return true;
}

