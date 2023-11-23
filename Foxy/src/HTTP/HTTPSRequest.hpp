#pragma once
#include "Pch/Common.hpp"
#include "R.A.G.Engine/Joaat.hpp"
#include "curl.h"

class curlWrapper {
public:
    curlWrapper() {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
    }
    ~curlWrapper() {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }
    bool setUrl(const std::string& url) {
        return curl_easy_setopt(curl, CURLOPT_URL, url.c_str()) == CURLE_OK;
    }
    bool setFollowLocation(bool follow) {
        return curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, follow ? 1L : 0L) == CURLE_OK;
    }
    bool setHeader(const std::string& header) {
        headers = curl_slist_append(headers, header.c_str());
        return curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers) == CURLE_OK;
    }
    bool setHeaders(const std::vector<std::string>& hdrs) {
        for (const auto& header : hdrs) {
            headers = curl_slist_append(headers, header.c_str());
        }
        return curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers) == CURLE_OK;
    }
    bool setPostFields(const std::string& fields) {
        bool ret = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str()) == CURLE_OK;
        if (ret) {
            ret = curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, static_cast<long>(fields.length())) == CURLE_OK;
        }
        return ret;
    }
    bool setWriteFunction(curl_write_callback callback) {
        return curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback) == CURLE_OK;
    }
    bool setWriteData(void* data) {
        return curl_easy_setopt(curl, CURLOPT_WRITEDATA, data) == CURLE_OK;
    }
    CURLcode perform() {
        return curl_easy_perform(curl);
    }
    std::string post(const std::string& url, const std::string& data, const std::vector<std::string>& headers = {}) {
        setUrl(url);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        setFollowLocation(true);
        setHeaders(headers);
        setPostFields(data);
        setWriteFunction(writeCallback);
        setWriteData(&responseBody);
        CURLcode result = perform();
        if (result != CURLE_OK) {
            throw std::runtime_error("Curl error: " + std::string(curl_easy_strerror(result)));
        }
        return responseBody;
    }
private:
    CURL* curl;
    struct curl_slist* headers;
    std::string responseBody;
    std::string responseHeaders;

    static size_t writeCallback(char* ptr, size_t size, size_t nmemb, void* data) {
        size_t numBytes = size * nmemb;
        std::string* response = static_cast<std::string*>(data);
        response->append(ptr, numBytes);
        return numBytes;
    }
    static size_t headerCallback(char* ptr, size_t size, size_t nmemb, void* data) {
        size_t numBytes = size * nmemb;
        std::string* response = static_cast<std::string*>(data);
        response->append(ptr, numBytes);
        return numBytes;
    }
};