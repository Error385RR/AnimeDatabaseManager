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
   // ~URLBUILDER(){}; // destructor

    void addParam(const std::string& key, const std::string& value){
        if (key.empty())
        {
            throw std::invalid_argument("Parameter key cannot be empty");
        }
        std::pair<std::string, std::string> received_param(key, value);
        params.push_back(received_param);
        // adds the  received params to the vector
    }
     // sets the final URL to the baseURL to be modified and returned
    
    auto paramterBuilder(){
        std::string paramterString;
        for(const auto& param : params){
            paramterString.append(escapedURL(param.first) + "=" + escapedURL(param.second) + "&");
        }
        if (!paramterString.empty()){paramterString.pop_back();}
        return paramterString; // returns the final URL
    }   

    std::string buildFormBody(){
        return paramterBuilder(); // returns the final URL
    }

    std::string buildQueryString(){
        return baseurl + "?" + paramterBuilder(); // returns the final URL
    }

    bool isSafe(char c)
    {
        return (
            (c >= 'A' && c <= 'Z') ||
            (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') ||
            c == '-' ||
            c == '_' ||
            c == '.' ||
            c == '~'
        );
    }


    std::string escapedURL(const std::string& url){
        std::string escapedString;
        const char hex[] = "0123456789ABCDEF";
        for(char c : url){
            if(!isSafe(c)){
                unsigned char byte = static_cast<unsigned char>(c);
                int high = byte / 16;
                int low = byte % 16;
                unsigned char first = hex[high];
                unsigned char second = hex[low];   
                escapedString.push_back('%');
                escapedString.push_back(first);
                escapedString.push_back(second);
            }
            else{
                escapedString.push_back(c);
            }
        }
        return escapedString;
    }

    std::string printParams()
    {
        std::string paramString;
        for(auto& param: params)
        {
            paramString.append(param.first + "=" + param.second + "&");
        }
        paramString.pop_back();
        return paramString;
    }

};
