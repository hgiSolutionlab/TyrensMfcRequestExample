#pragma once

#include <string>
#include <exception>
#include <stdexcept>
#include <curl/curl.h>
#include <memory>

class ApiWrapper
{
private:
	CURL* curl = NULL;
	struct curl_slist* headers = NULL;
	const std::string& endpoint;
	const std::string& token;
	void freeMemory();
	static size_t appendToStringBuffer(char* in, size_t size, size_t nmemb, std::string* out);

public:
	ApiWrapper(const std::string& endpoint, const std::string& token);
	~ApiWrapper();
	std::shared_ptr<std::string> request(const std::string& url);  // for now creates GET request & returns string representing json
};

class init_error : public std::runtime_error {
	init_error(const std::string& msg) : std::runtime_error(msg) {};
};