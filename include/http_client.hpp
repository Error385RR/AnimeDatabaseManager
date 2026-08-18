#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <memory>



class httphandler {
public:
    enum class HttpMethod
    {
        GET,
        POST
    };

    struct HttpResponse
    {
        long statusCode;
        std::string body;
    };
    
    struct HttpHeader
    {
        std::string name;
        std::string value;
    };

    httphandler();
    

    HttpResponse get(const std::string& url, std::vector<HttpHeader>& headers);

    HttpResponse post(const std::string& url, const std::vector<HttpHeader>& headers, const std::string& body);
    
    static size_t writecallback
    (
        char *data,
        size_t size,
        size_t nmemb,
        void *userdata
    );

    HttpResponse perform(
        const std::string& url,
        const std::vector<HttpHeader>& headers,
        const HttpMethod& method,
        const std::string& body = ""
    );

    std::string waitForAuthorizationCode();

    HttpResponse exchangeToken(
        const std::string& tokenBody,
        const std::string& tokenEndpoint,
        const std::vector<HttpHeader>& headers
    );
};


class CurlRuntime{
private:

    CurlRuntime();
   
public:
    ~CurlRuntime();
    static std::shared_ptr<CurlRuntime> getInstance();

};