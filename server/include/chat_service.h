#pragma once

#include "room_manager.h"
#include "token_manager.h"
#include "user_manager.h"

#include <memory>
#include <optional>
#include <string>
#include <vector>


namespace chat {

class ChatService {
public:
    ChatService(std::shared_ptr<UserManager> user_mgr,
                std::shared_ptr<RoomManager> room_mgr,
                std::shared_ptr<TokenManager> token_mgr);

    // Аутентификация
    std::shared_ptr<User> Register(const std::string& name, const std::string& password_hash);
    std::optional<std::string> Login(const std::string& name, const std::string& password_hash);
    bool Logout(const std::string& token);

    // Пользователи
    std::vector<std::string> GetOnlineUserNames() const;

    // Комнаты
    std::shared_ptr<Room> CreateRoom(const std::string& name);
    bool JoinRoom(const std::string& token, const std::string& room_name);
    bool LeaveRoom(const std::string& token);
    std::vector<std::string> GetRoomNames() const;
    std::string GetCurrentRoomName(const std::string& token) const;
    std::vector<std::string> GetUserNamesInCurrentRoom(const std::string& token) const;

private:
    std::shared_ptr<User> GetUserByToken(const std::string& token) const;

    std::shared_ptr<UserManager> user_manager_;
    std::shared_ptr<RoomManager> room_manager_;
    std::shared_ptr<TokenManager> token_manager_;
};

}  // namespace chat
