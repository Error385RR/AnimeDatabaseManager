#pragma once

#include <vector>
#include <string>
#include <openssl/rand.h>
#include <openssl/sha.h>

class PKCE {
public:
    static std::string base64url_encode(const std::vector<unsigned char>& buffer) {
        const char base64table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
        uint64_t accumulator = 0;
        int bit_count = 0;
        std::string verifier;

        // Loop through the buffer and encode each byte into a base64 character
        for(size_t i = 0; i < buffer.size(); i++) {
            accumulator = (accumulator << 8) | buffer[i];
            bit_count += 8;
            while(bit_count >= 6) {
                bit_count -= 6;
                auto value = (accumulator >> bit_count) & 0b111111;
                verifier += base64table[value];
            }
        }   
        
        if(bit_count > 0) {
            auto value = (accumulator << (6 - bit_count)) & 0b111111;
            verifier += base64table[value];
        }
        
        return verifier;
    }

    static std::string generateVerifier(){
        std::string verifier;
        std::vector<unsigned char> buffer(32);
    
        RAND_bytes(buffer.data(), static_cast<int>(buffer.size()));
        return base64url_encode(buffer); //returns the verifier
    }

    static std::string createCodeChallenge(const std::string& verifier){
        std::vector<unsigned char> hash(SHA256_DIGEST_LENGTH);
        
        SHA256(
            reinterpret_cast<const unsigned char*>(verifier.data()),
            verifier.size(),
            hash.data()
        );
        std::string codeChallenge = base64url_encode(hash);
        return codeChallenge;
    }
};