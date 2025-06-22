
#include "chat_service.h"
#include "token_generator.h"

namespace chat {

ChatService::ChatService(std::shared_ptr<UserManager> user_mgr,
                         std::shared_ptr<RoomManager> room_mgr,
                         std::shared_ptr<TokenManager> token_mgr)
    : user_manager_(std::move(user_mgr))
    , room_manager_(std::move(room_mgr))
    , token_manager_(std::move(token_mgr)) {
}

//std::shared_ptr<User> ChatService::Register(const std::string& name, const std::string& password_hash) {
bool ChatService::Register(const std::string& name, const std::string& password_hash) {
    return user_manager_->RegisterUser(name, password_hash) != nullptr;
}

std::optional<std::string> ChatService::Login(const std::string& name, const std::string& password_hash) {
    auto user = user_manager_->GetUserByName(name);
    if (!user || user->GetPasswordHash() != password_hash) {
        return std::nullopt;
    }

    auto general_room = room_manager_->GetRoomByName("general");
    if (general_room) {
        general_room->AddUser(user);
        user->SetRoom(general_room);
    }

    std::string token = Token::GENERATOR.GenerateHEXToken();
    token_manager_->SaveToken(user->GetId(), token);
    return token;
}

bool ChatService::Logout(const std::string& token) {
    auto user_id_opt = token_manager_->GetUserIdByToken(token);
    if (!user_id_opt.has_value()) return false;

    token_manager_->RemoveTokenByToken(token);
    return room_manager_->RemoveUserFromRoom(user_id_opt.value());
}

std::vector<std::string> ChatService::GetOnlineUserNames() const {
    auto users = user_manager_->GetAllUsers();
    std::vector<std::string> names;
    names.reserve(users.size());
    for (const auto& user : users) {
        names.push_back(user->GetName());
    }
    return names;
}

std::shared_ptr<Room> ChatService::CreateRoom(const std::string& name) {
    return room_manager_->CreateRoom(name);
}

bool ChatService::JoinRoom(const std::string& token, const std::string& room_name) {
    auto user = GetUserByToken(token);
    if (!user) return false;
    return room_manager_->MoveUserToRoom(user->GetId(), room_name);
}

bool ChatService::LeaveRoom(const std::string& token) {
    auto user = GetUserByToken(token);
    if (!user) {
        return false;
    }

    auto general_room = room_manager_->GetRoomByName("general");
    if (!general_room) {
        return false;
    }

    auto current_room = user->GetRoom();
    if (current_room) {
        current_room->RemoveUser(user->GetId());
    }

    general_room->AddUser(user);
    user->SetRoom(general_room);

    return true;
}

bool ChatService::HasRoom(const std::string& name) const {
    return room_manager_->GetRoomByName(name) != nullptr;
}

std::vector<std::string> ChatService::GetRoomNames() const {
    return room_manager_->GetAllRoomNames();
}

std::string ChatService::GetCurrentRoomName(const std::string& token) const {
    auto user = GetUserByToken(token);
    if (!user) return "";
    auto room = user->GetRoom();
    return room ? room->GetName() : "";
}

std::vector<std::string> ChatService::GetUserNamesInCurrentRoom(const std::string& token) const {
    auto user = GetUserByToken(token);
    if (!user) {
        return {};
    }
    auto room = user->GetRoom();
    return room ? room->GetUserNames() : std::vector<std::string>{};
}

std::vector<std::string> ChatService::GetUserNamesInRoom(const std::string& room_name) const {
    auto room = room_manager_->GetRoomByName(room_name);
    if (!room) {
        return {};
    }
    return room->GetUserNames();
}

std::shared_ptr<User> ChatService::GetUserByToken(const std::string& token) const {
    auto user_id_opt = token_manager_->GetUserIdByToken(token);
    if (!user_id_opt.has_value()) {
        return nullptr;
    }
    return user_manager_->GetUserById(user_id_opt.value());
}

std::optional<std::string> ChatService::GetTokenByUserName(const std::string& name) const {
    auto user = user_manager_->GetUserByName(name);
    if (!user) {
        return std::nullopt;
    }
    return token_manager_->GetTokenByUserId(user->GetId());
}

}  // namespace chat
