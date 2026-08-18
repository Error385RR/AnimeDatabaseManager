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


    void addParam(const std::string& key, const std::string& value);
    // sets the final URL to the baseURL to be modified and returned
    
    std::string parameterBuilder();

    std::string buildFormBody();
    std::string buildQueryString();

    bool isSafe(char c);

    std::string escapedURL(const std::string& url);

    std::string printParams();


};
