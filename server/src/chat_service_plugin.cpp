#include "chat_service_plugin.h"


chat::ChatService* ChatServicePlugin::chat_service_ = nullptr;

void ChatServicePlugin::initAndStart(const Json::Value &) {
    auto db = drogon::app().getPlugin<DatabasePlugin>()->GetDB();

    // Создаём ChatService как static локальный объект, живущий весь runtime.
    static chat::ChatService service(*db);
    chat_service_ = &service;
}

void ChatServicePlugin::shutdown() {
    chat_service_ = nullptr;
}

chat::ChatService* ChatServicePlugin::GetService() {
    return chat_service_;
}