#include "parameterbuilder.hpp"


void genericParameterBuilder::addParam(const std::string& key, const std::string& value){
        if (key.empty())
        {
            throw std::invalid_argument("Parameter key cannot be empty");
        }
        std::pair<std::string, std::string> received_param(key, value);
        params.push_back(received_param);
        // adds the  received params to the vector
    }
     // sets the final URL to the baseURL to be modified and returned
    

    std::string genericParameterBuilder::parameterBuilder(const EncodingType& type){
        std::string paramterString;
        for(const auto& param : params){
            paramterString.append(encode(param.first, type) + "=" + encode(param.second, type) + "&");
        }
        if (!paramterString.empty()){paramterString.pop_back();}
        return paramterString; // returns the final URL
    }   

    std::string genericParameterBuilder::buildFormBody(){
        return parameterBuilder(EncodingType::Form); // returns the final URL
    }

    std::string genericParameterBuilder::buildQueryString(){
        return baseurl + "?" + parameterBuilder(EncodingType::RFC3986); // returns the final URL
    }

    bool genericParameterBuilder::isSafeForrfc3986(char c)
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

    



    std::string genericParameterBuilder::rfc3986encoding(const std::string& url){
        std::string escapedString;
        const char hex[] = "0123456789ABCDEF";
        for(char c : url){
            if(!isSafeForrfc3986(c)){
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
    std::string genericParameterBuilder::formEncoding(const std::string& string){
        std::string encoded;
        const char hex[] = "0123456789ABCDEF";

        for (unsigned char c : string)
        {
            if (c == ' ')
            {
                encoded.push_back('+');
            }
            else if (isSafeForrfc3986(c))
            {
                encoded.push_back(c);
            }
            else
            {
                encoded.push_back('%');
                encoded.push_back(hex[c >> 4]);
                encoded.push_back(hex[c & 0x0F]);
            }
        }
        return encoded;
    }


    std::string genericParameterBuilder::printParams()
    {
        std::string paramString;
        for(auto& param: params)
        {
            paramString.append(param.first + "=" + param.second + "&");
        }
        paramString.pop_back();
        return paramString;
    }

    std::string genericParameterBuilder::encode(const std::string& string, const EncodingType& encodingtype){
        switch (encodingtype)
        {
        case EncodingType::RFC3986:
            return rfc3986encoding(string);
            break;
        case EncodingType::Form:
            return formEncoding(string);
            break;
        default:
            throw std::invalid_argument("Unknown encoding type");
            break;
        }
    }