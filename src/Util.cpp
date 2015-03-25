#include "Util.h"

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
 * Stats helper
 */
void Stats::count_hit() {
	this->hits++;
}

void Stats::count_total() {
	this->requests++;
}

double Stats::get_rate() {
	return (double)hits/(double)requests;
}
