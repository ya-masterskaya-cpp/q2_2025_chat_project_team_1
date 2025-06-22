#pragma once

#include "chat_service.h"
#include "room_manager.h"
#include "user_manager.h"
#include "token_manager.h"

#include <drogon/plugins/Plugin.h>


// Плагин для доступа к ChatService
class ChatServicePlugin : public drogon::Plugin<ChatServicePlugin> {
public:
    void initAndStart(const Json::Value &config) override;
    void shutdown() override;

    // Получить глобальный экземпляр ChatService
    static std::shared_ptr<chat::ChatService> GetService();

private:
    static std::shared_ptr<chat::ChatService> chat_service_;
};