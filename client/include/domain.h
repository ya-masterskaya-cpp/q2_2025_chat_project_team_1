#ifndef DOMAIN_H
#define DOMAIN_H

#include <cpr/cpr.h>
#include <json/json.h>
#include <string>

namespace domain {

struct UserData {
    std::string name;
    std::string token;
};

struct ServerResponse {
    bool status;
    std::string msg;
    std::string error_msg;
};

cpr::Response SendPostRequest(const std::string& endpoint, const Json::Value& body);
cpr::Response SendPostRequest(const std::string& endpoint, const Json::Value& body, const std::string& token);
cpr::Response SendGetRequest(const std::string& endpoint);
cpr::Response SendGetRequest(const std::string& endpoint, const std::string& token);

}

#endif //DOMAIN_H
