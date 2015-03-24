/*
 * Curl.cpp
 *
 *  Created on: Mar 24, 2015
 *      Author: sheng
 */

#include "../src/Util.h"

/*
 * Curl helper class
 */

// Callback called when a chunk is received
static size_t cb(void *buffer, size_t size, size_t nmemb, void *userp) {
	((std::string*) userp)->append((char*) buffer, size * nmemb);
	return size * nmemb;
}

Curl::Curl() {
	this->curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(this->curl, CURLOPT_WRITEFUNCTION, cb);
}

Curl::~Curl() {
	curl_easy_cleanup(this->curl);
}

std::string Curl::get(std::string url) {
	std::string buf;
	curl_easy_setopt(this->curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(this->curl, CURLOPT_WRITEDATA, &buf);
	curl_easy_perform(this->curl);
	return buf;
}

/*
 * Cache abstract class
 */
bool Cache::add(string url, string data) {
	std::pair<string, string> kv(url, data);
	this->cache_map.insert(kv);
	this->size -= data.size();
	return true;
}

bool Cache::remove(string url) {
	auto element = cache_map.find(url);
	if (element != cache_map.end()) {
		cout<< "removing " << url << endl;
		this->size += element->second.size();
		cache_map.erase(url);
		return true;
	}
	// not found
	return false;
}

string Cache::find(string url) {
	string result;
	auto element = cache_map.find(url);
	if (element != cache_map.end())
		result = element->second;
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
FIFOCache::FIFOCache() {
}

FIFOCache::FIFOCache(size_t max_size) {
	this->max_size = max_size;
	this->size = max_size;
}

FIFOCache::~FIFOCache() {
	this->list.clear();
}

string FIFOCache::fetch(string url) {
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
	return result;
}

bool FIFOCache::free(size_t input_size) {
	if (input_size > this->max_size)
		return false;
	if (input_size < this->size) {
		return true;
	}
	while(!can_fit_in(input_size)) {
		// clean up elements until available space
		remove(list.front());
		list.pop_front();
	}
	return true;
}
