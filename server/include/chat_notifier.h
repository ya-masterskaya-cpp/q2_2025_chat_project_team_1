#pragma once

#include "chat_ws_controller.h"

#include <string>
#include <vector>


/**
 * @brief Прослойка для REST-контроллеров, предоставляющая доступ к уведомлениям пользователей.
 * @details Внутренне работает через зарегистрированную реализацию, по умолчанию — WebSocket.
 */
class ChatNotifier {
public:
    /**
     * @brief Рассылает сообщение всем пользователям в комнате.
     */
    static void Broadcast(const std::string& token, const std::string& message)
    {
        ChatWebSocket::Broadcast(token, message);
    }

    /**
     * @brief Возвращает список подключённых пользователей.
     */
    static std::vector<std::string> GetConnectedUsers()
    {
        return ChatWebSocket::GetConnectedUsers();
    }
};
