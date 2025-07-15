#ifndef QUERY_HANDLER_H
#define QUERY_HANDLER_H


#include <functional>
#include <string>

namespace domain {

using ProcessFunction = std::function<void()>;

template<typename Body, typename Header, typename Parameters>
class Query
{
public:

    Query& SetToken(const std::string& token) {
        header_["Authorization"] = "Bearer " + token;
        return *this;
    }

    Query& SetHeader(Header&& header) {
        header_ = std::move(header);
        return *this;
    }

    const Header& GetHeader() const {
        return header_;
    }

    Query& SetBody(Body&& body) {
        body_  = std::move(body);
        return *this;
    }

    const Body& GetBody() const {
        return body_;
    }

    Query& SetParameters(Parameters&& parameters) {
        parameters_  = std::move(parameters);
        return *this;
    }

    const Parameters& GetParameters() const {
        return parameters_;
    }

    Query& SetEndpoint(std::string_view endpoint) {
        endpoint_ = endpoint;
        return *this;
    }

    std::string_view GetEndpoint() const {
        return endpoint_;
    }

    Query& SetProcessFunction(ProcessFunction process_function) {
        process_function_ = process_function;
    }

    void Process() {
        process_function_();
    }

private:
    std::string_view endpoint_;

    Body body_;
    Header header_;
    Parameters parameters_;
    ProcessFunction process_function_;
};

template<typename Body, typename Header, typename Parameters>
class QueryHandler {
public:
    Query<Body,Header,Parameters>* AddEndpoint(std::string_view endpoint) {
        auto [it, ret] = endpoints_.try_emplace(endpoint);
        it->second.SetEndpoint(endpoint);
        return &it->second;
    }

    Query<Body,Header,Parameters>& At(std::string_view endpoint) {
        return endpoints_.at(endpoint);
    }

    void SetUrl(const std::string& url) {
        url_ = url;
    }

    const std::string& GetUrl() const {
        return url_;
    }

private:
    std::unordered_map<std::string_view, Query<Body,Header,Parameters>> endpoints_;
    std::string url_;
};

}   //namespace domain

#endif // QUERY_HANDLER_H
