#pragma once

#include <string>
#include <vector>
#include <curl/curl.h>
#include <stdexcept>


class genericParameterBuilder{
private:
    std::string baseurl;
    std::vector<std::pair<std::string, std::string>> params; // key value pair of query s
    
public:
    genericParameterBuilder(std::string baseurl)
        : baseurl(baseurl)
    {}
    enum class EncodingType{
        RFC3986,
        Form
    };

    void addParam(const std::string& key, const std::string& value);
    // sets the final URL to the baseURL to be modified and returned
    
    std::string parameterBuilder(const EncodingType& type);

    std::string buildFormBody();
    std::string buildQueryString();
    std::string encode(const std::string& string, const EncodingType& encodingtype);
    std::string formEncoding(const std::string& value);
    bool isSafeForrfc3986(char c);

    std::string rfc3986encoding(const std::string& url);

    std::string printParams();


};
