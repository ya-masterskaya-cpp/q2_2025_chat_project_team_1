#pragma once

#include "const_api.h"
#include "http_utils.h"

#include "chat_service_plugin.h"
#include "chat_websocket.h"

#include <drogon/HttpController.h>
#include <json/json.h>


// REST API для работы с пользователями: список всех клиентов

class UserController : public drogon::HttpController<UserController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(UserController::getOnlineUsers,  std::string(api::USERS_ONLINE), drogon::Get);
    METHOD_LIST_END

    void getOnlineUsers(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback);
};
