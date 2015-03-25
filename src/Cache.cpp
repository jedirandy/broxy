#include "Cache.h"

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
		cout << "removing " << url << endl;
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
	if (result.empty()) {
		cout << "Cache miss" << endl;
		result = this->curl.get(url);
		if (free(result.size())) {
			// update cache
			add(url, result);
			// add to the queue
			list.push_back(url);
		}
	} else {
		cout << "Cache hit" << endl;
	}
	cout << stats.get_rate() << endl;
	return result;
}

bool FIFOCache::free(size_t input_size) {
	if (input_size > this->max_size)
		return false;
	if (input_size < this->size) {
		return true;
	}
	while (!can_fit_in(input_size)) {
		// clean up elements until available space
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
	if (result.empty()) {
		cout << "Cache miss" << endl;
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
		cout << "Cache hit" << endl;
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
		// clean up elements until available space
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
	if (result.empty()) {
		cout << "Cache miss" << endl;
		result = this->curl.get(url);
		if (free(result.size())) {
			// update cache
			add(url, result);
			// add to the priority queue
			size_t size = result.size();
			Entity e(url, size);
			pq.push(e);
		}
	} else {
		cout << "Cache hit" << endl;
	}
	cout << stats.get_rate() << endl;
	if (!pq.empty())
		cout << "top " << pq.top().url << endl;
	return result;
}

bool MAXSCache::free(size_t input_size) {
	if (input_size > this->max_size)
		return false;
	if (input_size < this->size) {
		return true;
	}
	while (!can_fit_in(input_size)) {
		// clean up elements until available space
		remove(pq.top().url);
		pq.pop();
	}
	return true;
}
