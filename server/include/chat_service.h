#pragma once

#include "token_manager.h"
#include "token_generator.h"
#include "db_wrapper.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>


namespace chat {

class ChatService {
public:
    ChatService(std::shared_ptr<IRCDBWrapper> db_wrapper,
                std::shared_ptr<TokenManager> token_mgr);

    // Аутентификация
    bool Register(const std::string& name, const std::string& password_hash);
    std::optional<std::string> Login(const std::string& name, const std::string& password_hash);
    bool Logout(const std::string& token);

    // Пользователи
    std::vector<std::string> GetOnlineUserNames() const;

    // Комнаты
    bool CreateRoom(const std::string& name);
    bool JoinRoom(const std::string& token, const std::string& room_name);
    bool LeaveRoom(const std::string& token);
    bool HasRoom(const std::string& name) const;
    std::vector<std::string> GetRoomNames() const;
    std::optional<std::string> GetCurrentRoomName(const std::string& token) const;
    std::vector<std::string> GetUserNamesInRoom(const std::string& room_name) const;

    // Сообщения
    bool SaveMessage(const std::string& token, const std::string& text); // Сохраняет сообщение в БД, применяется при отправке
    std::vector<postgres::MessageRecord> GetRecentMessages(const std::string& room_name, int max_items) const;
    std::vector<postgres::MessageRecord> GetRoomMessagesPage(const std::string& room_name, int offset, int limit) const;

    std::optional<postgres::UserRecord> GetUserByToken(const std::string& token) const;
    std::optional<std::string> GetTokenByUserName(const std::string& name) const;

    std::optional<std::string> GetUserNameById(const postgres::UserId& id) const;

private:
    std::shared_ptr<IRCDBWrapper> db_wrapper_;
    std::shared_ptr<TokenManager> token_manager_; // Токены сбрасываются при падении сервера, требуется новый вход по сохраненным в БД логину и хэшу пароля
};

} // namespace chat
