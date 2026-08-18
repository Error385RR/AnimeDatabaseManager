
#include <stdexcept>
#include "http_client.hpp"  
#include "httplib.h"


httphandler::httphandler(){
    CurlRuntime::getInstance();
}



httphandler::HttpResponse httphandler::get(const std::string& url, std::vector<HttpHeader>& headers){
    return perform(url, headers, HttpMethod::GET);
};

httphandler::HttpResponse httphandler::post(const std::string& url, const std::vector<HttpHeader>& headers, const std::string& body){
    return perform(url, headers, HttpMethod::POST, body);
}

size_t httphandler::writecallback
(
    char *data,
    size_t size,
    size_t nmemb,
    void *userdata
){
    size_t total = size * nmemb;

    std::string *buffer = static_cast<std::string*>(userdata);

    buffer->append(data, total);    

    return total;
}

httphandler::HttpResponse httphandler::perform(
    const std::string& url,
    const std::vector<HttpHeader>& headers,
    const HttpMethod& method,
    const std::string& body
)
{
    CURL *curl;  
    curl = curl_easy_init();
    
    HttpResponse response;

    if(curl== nullptr ){
        throw std::runtime_error("Failed to Initalize Curl for Get request");
    }

    std::string data;
    struct curl_slist* headersslist = nullptr;
    long httpCode;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writecallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    for(const auto& i : headers){
        std::string curlheader = i.name + ": " + i.value;
        headersslist = curl_slist_append(headersslist, curlheader.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headersslist);

    if(method == HttpMethod::POST){
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
    }

    auto curlperformresult = curl_easy_perform(curl);

    if(curlperformresult != CURLE_OK)
    {
        curl_slist_free_all(headersslist);
        curl_easy_cleanup(curl);
        throw std::runtime_error(curl_easy_strerror(curlperformresult));
    }
    
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    response.body = data;
    response.statusCode = httpCode;

    curl_slist_free_all(headersslist);
    curl_easy_cleanup(curl);
    
    return response;
}

std::string httphandler::waitForAuthorizationCode(){
    httplib::Server server;
    std::string code;
    server.Get(
        "/callback",
        [&](const httplib::Request& req, httplib::Response& res)
        {
            code = req.get_param_value("code");
            if (code.empty())
            {
                res.set_content(
                    "Authorization failed.",
                    "text/html"
                );
                throw std::runtime_error("Code not returned");
            }
            
            res.set_content(
                "Login successful",
                "text/html"
            );

            server.stop();
        });

    server.listen(
        "localhost",
        8080
    );
    
    return code;
}

httphandler::HttpResponse httphandler::exchangeToken(
    const std::string& tokenBody,
    const std::string& tokenEndpoint,
    const std::vector<HttpHeader>& headers
){
    HttpResponse token = post(tokenEndpoint, headers, tokenBody);
    return token;
}



CurlRuntime::CurlRuntime()
{
    auto result = curl_global_init(CURL_GLOBAL_DEFAULT);

    if (result != CURLE_OK)
    {
        throw std::runtime_error(
            "Could not initialize Curl: " +
            std::string(curl_easy_strerror(result))
        );
    }
}

CurlRuntime::~CurlRuntime()
{
    curl_global_cleanup();
}

std::shared_ptr<CurlRuntime> CurlRuntime::getInstance()
{
    static std::shared_ptr<CurlRuntime> instance =
        std::shared_ptr<CurlRuntime>(new CurlRuntime());

    return instance;
}