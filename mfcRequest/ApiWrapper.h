#pragma once

#include <curl/curl.h>
#include <string>
#include <stdexcept>
#include <memory>

class ApiWrapper
{
private:
	CURL* curl = nullptr;
	struct curl_slist* headers = nullptr;
	const std::string endpoint;
	const std::string token;
	void freeMemory();
	static size_t appendToStringBuffer(char* in, size_t size, size_t nmemb, std::string* out);

public:
	ApiWrapper(const std::string& endpoint, const std::string& token);
	~ApiWrapper();
	std::shared_ptr<std::string> request(const std::string& url);  // for now creates GET request & returns string representing json
};

namespace api_exception {
	struct init_error : public std::runtime_error {
		init_error(std::string& msg);
	};

	struct req_error : public std::runtime_error {
		req_error(std::string& msg);
	};
}

