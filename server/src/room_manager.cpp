#include "room_manager.h"


namespace chat {

RoomManager::RoomManager() {
    auto general = std::make_shared<Room>(next_room_id_++, GENERAL_ROOM); // общая комната
    id_to_room_[general->GetId()] = general;
    name_to_room_[general->GetName()] = general;
}

std::shared_ptr<Room> RoomManager::CreateRoom(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto room = std::make_shared<Room>(next_room_id_++, name);
    id_to_room_[room->GetId()] = room;
    name_to_room_[room->GetName()] = room;
    return room;
}

std::shared_ptr<Room> RoomManager::GetRoomById(ID id) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = id_to_room_.find(id);
    return (it != id_to_room_.end()) ? it->second : nullptr;
}

std::shared_ptr<Room> RoomManager::GetRoomByName(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = name_to_room_.find(name);
    return (it != name_to_room_.end()) ? it->second : nullptr;
}

std::vector<std::string> RoomManager::GetAllRoomNames() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::string> names;
    names.reserve(name_to_room_.size());
    for (const auto& [name, room] : name_to_room_) {
        names.push_back(name);
    }
    return names;
}

bool RoomManager::MoveUserToRoom(ID user_id, const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = name_to_room_.find(name);
    if (it == name_to_room_.end()) {
        return true; // Уже в нужной комнате
    }
    std::shared_ptr<Room> to_room = it->second;

    // ищем текущую комнату пользователя
    for (auto& [_, room] : id_to_room_) {
        auto users = room->GetUsers();
        for (const auto& user : users) {
            if (user->GetId() == user_id) {
                if (room->GetId() == to_room->GetId()) {
                    return true;
                }
                room->RemoveUser(user_id);
                to_room->AddUser(user);
                user->SetRoom(to_room);
                return true;
            }
        }
    }

    return false;
}

bool RoomManager::AddUserToRoom(const std::shared_ptr<User>& user, ID room_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = id_to_room_.find(room_id);
    if (it == id_to_room_.end() || !user) return false;

    it->second->AddUser(user);
    user->SetRoom(it->second);
    return true;
}

bool RoomManager::RemoveUserFromRoom(ID user_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    for (auto& [_, room] : id_to_room_) {
        auto users = room->GetUsers();
        auto it = std::find_if(users.begin(), users.end(),
                               [&](const std::shared_ptr<User>& u) {
                                   return u->GetId() == user_id;
                               });
        if (it != users.end()) {
            room->RemoveUser(user_id);
            return true;
        }
    }
    return false;
}

}  // namespace chat
