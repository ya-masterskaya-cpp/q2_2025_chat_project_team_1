#include "domain.h"

namespace domain {

cpr::Response SendPostRequest(const std::string& endpoint, const Json::Value& body) {
    Json::StreamWriterBuilder writer;
    writer["emitUTF8"] = true;
    writer["enableYAMLCompatibility"] = false;
    cpr::Header headers = {{"Content-Type", "application/json"}};
    return cpr::Post(cpr::Url{endpoint}, headers, cpr::Body{Json::writeString(writer, body)},cpr::Timeout{2000});
}

cpr::Response SendPostRequest(const std::string& endpoint, const Json::Value& body,
                                    const std::string& token) {
    Json::StreamWriterBuilder writer;
    writer["emitUTF8"] = true;
    writer["enableYAMLCompatibility"] = false;
    cpr::Header headers = {{"Content-Type", "application/json"}};
    headers["Authorization"] = "Bearer " + token;
    return cpr::Post(cpr::Url{endpoint}, headers, cpr::Body{Json::writeString(writer, body)},cpr::Timeout{2000});
}

cpr::Response SendGetRequest(const std::string& endpoint) {
    cpr::Header headers;
    return cpr::Get(cpr::Url{endpoint}, headers,cpr::Timeout{2000});
}

cpr::Response SendGetRequest(const std::string& endpoint, const std::string& token) {
    cpr::Header headers;
    headers["Authorization"] = "Bearer " + token;
    return cpr::Get(cpr::Url{endpoint}, headers,cpr::Timeout{2000});
}

Json::Value Parse(const std::string& msg) {
    Json::CharReaderBuilder builder;
    Json::Value parsed_val;
    std::string err;
    std::istringstream iss(msg);
    if (!Json::parseFromStream(builder, iss, &parsed_val,&err)) {
        assert(false);
    }
    return parsed_val;
}

}   //namespace domain
