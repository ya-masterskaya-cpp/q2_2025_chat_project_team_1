#pragma once

#include "const_api.h"
#include "http_utils.h"

#include "chat_service_plugin.h"
#include "chat_websocket.h"

#include <drogon/HttpController.h>
#include <json/json.h>


// REST API для работы с сообщениями: отправка, сохранение в БД сервера, выгрузка с сервера
// Важно! При запросе на отправку SendMessage сохранение не выполняется, сохранение происходит по запросу UploadMessage

class MessageController : public drogon::HttpController<MessageController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(MessageController::SendMessage, std::string(api::MESSAGE_SEND), drogon::Post);
        ADD_METHOD_TO(MessageController::UploadMessage, std::string(api::MESSAGE_UPLOAD), drogon::Post);
        ADD_METHOD_TO(MessageController::GetRecentMessages, std::string(api::MESSAGE_RECENT), drogon::Get);
        //ADD_METHOD_TO(MessageController::GetRoomMessagesPage, std::string(api::MESSAGE_PAGE), drogon::Get); // TODO если будет нужен постраничный вывод
    METHOD_LIST_END

    void SendMessage(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void UploadMessage(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    void GetRecentMessages(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
    //void GetRoomMessagesPage(const drogon::HttpRequestPtr& req, std::function<void(const drogon::HttpResponsePtr&)>&& callback);
};
