#include "domain.h"


namespace domain {


cpr::Response SendPostRequest(const std::string& base_address,
                              const Query<cpr::Body, cpr::Header, cpr::Parameters>& query) {
    return  cpr::Post(cpr::Url{base_address + std::string{query.GetEndpoint()}}, query.GetHeader(),
                     query.GetBody(), cpr::Timeout{2000});
}

cpr::Response SendGetRequest(const std::string& base_address,
                             const Query<cpr::Body, cpr::Header, cpr::Parameters>& query) {
    return cpr::Get(cpr::Url{base_address + std::string{query.GetEndpoint()}}, query.GetHeader(),query.GetParameters(),
                        cpr::Timeout{2000});
}

Json::Value Parse(const std::string& msg) {
    Json::CharReaderBuilder builder;
    Json::Value parsed_val;
    std::string err;
    std::istringstream iss(msg);
    if (!Json::parseFromStream(builder, iss, &parsed_val, &err)) {
        return Json::Value();
    }
    return parsed_val;
}

}   //namespace domain
