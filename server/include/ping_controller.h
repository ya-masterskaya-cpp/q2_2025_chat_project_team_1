#pragma once

#include "const_api.h"
#include "http_utils.h"

#include <drogon/HttpController.h>
#include <json/json.h>


// REST API для проверки доступности сервера

class PingController : public drogon::HttpController<PingController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(PingController::Ping, std::string(api::PING), drogon::Get);
    METHOD_LIST_END

    void Ping(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};
