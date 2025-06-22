#include "chat_service_plugin.h"


std::shared_ptr<chat::ChatService> ChatServicePlugin::chat_service_ = nullptr;

void ChatServicePlugin::initAndStart(const Json::Value &) {
    auto user_mgr = std::make_shared<chat::UserManager>();
    auto room_mgr = std::make_shared<chat::RoomManager>();
    auto token_mgr = std::make_shared<chat::TokenManager>();

    // Создание обязательной комнаты "general"
    room_mgr->CreateRoom(chat::GENERAL_ROOM);

    // Инициализация ChatService
    chat_service_ = std::make_shared<chat::ChatService>(user_mgr, room_mgr, token_mgr);
}

void ChatServicePlugin::shutdown() {
    chat_service_.reset();
}

std::shared_ptr<chat::ChatService> ChatServicePlugin::GetService() {
    return chat_service_;
}