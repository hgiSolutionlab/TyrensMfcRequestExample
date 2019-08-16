#include "ApiWrapper.h"

ApiWrapper::ApiWrapper(const std::string& endpoint, const std::string& token) : endpoint(endpoint), token(token) {

	if (endpoint.empty()) {
		throw std::invalid_argument("endpoint");
	}

	if (token.empty()) {
		throw std::invalid_argument("token");
	}

	try {
		curl = curl_easy_init();

		if (!curl) {
			throw std::runtime_error(curl_easy_strerror(CURLE_FAILED_INIT));
		}

		const std::string authHeader = "Authorization: Bearer " + token;
		headers = curl_slist_append(headers, authHeader.c_str());

		if (!headers) {
			throw std::runtime_error("Failed to add auth header");
		}
	}
	catch (init_error& e) {
		freeMemory();
		throw e;
	}

};

ApiWrapper::~ApiWrapper() {
	freeMemory();
}

void ApiWrapper::freeMemory() {

	if (curl) {
		curl_easy_cleanup(curl);
	}

	if (headers) {
		curl_slist_free_all(headers);
	}
}

std::shared_ptr<std::string> ApiWrapper::request(const std::string& url = "") {

	auto resBuffer = std::make_shared<std::string>();
	const std::string requestUrl = endpoint + url;
	curl_easy_setopt(curl, CURLOPT_URL, requestUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, appendToStringBuffer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, resBuffer);

	CURLcode res;
	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		throw std::runtime_error(curl_easy_strerror(res));
	}

	return resBuffer;
}

size_t ApiWrapper::appendToStringBuffer(char* in, size_t size, size_t nmemb, std::string* out) {

	// Writing response into string

	std::size_t total_size = size * nmemb;

	if (total_size) {
		out->append(in, total_size);
	}

	return total_size;
};

