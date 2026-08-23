#include "oauth2.hpp"

    OAuth2Client::OAuth2Client(const OAuthConfig& oauthConfig) 
        : config(oauthConfig)      
    {
        if(config.clientID.empty())
        {
            config.clientID = getClientID(config.clientIDfilepath);
        }
    }

    std::string OAuth2Client::getClientID(const std::string& filepath){
        std::string clientidfromfile;
        std::fstream clientidfile;
        clientidfile.open(filepath, std::ios::in);

        if(!clientidfile.is_open())
        {
            throw std::runtime_error("Could not open client ID file");
        }

        std::getline(clientidfile, clientidfromfile);

        return clientidfromfile;
    }



    // std::string OAuth2Client::pkceCodeChallenge(const std::string& verifier){return PKCE::createCodeChallenge(verifier);}

    std::string OAuth2Client::generateState(){return PKCE::generateVerifier();}

    std::string OAuth2Client::generateAuthorizationURL(){
        genericParameterBuilder builder(config.authorizationEndpoint);
        
        builder.addParam("client_id", config.clientID);
        builder.addParam("response_type", "code");
        builder.addParam("redirect_uri", config.redirectURI);
        if(!config.scope.empty()){builder.addParam("scope", config.scope);}
        if(config.state){
            flow.state = generateState();
            builder.addParam("state", flow.state);
        }
        
        if(config.usePKCE)
        {
            flow.verifier = PKCE::generateVerifier();
            if (config.codeChallengeMethod == PKCEMethod::SHA256)
            {
                flow.codeChallenge = PKCE::createCodeChallenge(flow.verifier);
                flow.codeChallengeMethodString = "S256";
            }else{
                flow.codeChallenge = flow.verifier;
                flow.codeChallengeMethodString = "plain";
            }
            
            builder.addParam("code_challenge", flow.codeChallenge);
            builder.addParam("code_challenge_method", flow.codeChallengeMethodString);
        }

       return builder.buildQueryString();
    }

    std::string OAuth2Client::generateTokenBody(){
        genericParameterBuilder builder(config.tokenEndpoint);

        builder.addParam("client_id", config.clientID);
        builder.addParam("grant_type", "authorization_code");
        builder.addParam("redirect_uri", config.redirectURI);
        builder.addParam("code", flow.codeForToken);
        if(config.usePKCE){builder.addParam("code_verifier", flow.verifier);}
        if(!config.clientSecret.empty()){builder.addParam("client_secret", config.clientSecret);}
        return builder.buildFormBody();
    }

    json OAuth2Client::exchangeCodeForToken(const std::vector<httphandler::HttpHeader>& headers){
        std::vector<httphandler::HttpHeader> requestHeaders = headers;

        requestHeaders.push_back(
            {"Content-Type","application/x-www-form-urlencoded"}
        );
        std::string tokenBody = generateTokenBody();

        httphandler::HttpResponse tokenGeneratedstruct = httpHandler.exchangeToken(tokenBody,config.tokenEndpoint, requestHeaders);
        
        std::string tokenGenerated = tokenGeneratedstruct.body;

        json tokenJson = json::parse(tokenGenerated);
        
        return tokenJson;
    }

    json OAuth2Client::authenticate(){
        std::string authURL = generateAuthorizationURL();
        //temporary
        std::cout <<"Click On The URL and Grant Access In The Browser: "<< authURL <<std::endl;
        flow.codeForToken = httpHandler.waitForAuthorizationCode();
    
        json tokenJson = exchangeCodeForToken();

        return tokenJson;
    }

    void OAuth2Client::printConfigStruct(){
        std::cout << config.baseURL<<std::endl;
        std::cout << config.tokenEndpoint <<std::endl;
        std::cout << config.clientID <<std::endl;
        std::cout << config.clientSecret <<std::endl;
        std::cout << config.redirectURI <<std::endl;
        std::cout << config.clientID <<std::endl;
        std::cout << config.usePKCE <<std::endl;
        std::cout << config.state <<std::endl;
        std::cout << static_cast<int>(config.codeChallengeMethod) <<std::endl;
    }
