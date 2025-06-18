#pragma once

#include "chat_models.h"

#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>


namespace chat {

class RoomManager {
public:
    RoomManager();

    std::shared_ptr<Room> CreateRoom(const std::string& name);
    std::shared_ptr<Room> GetRoomById(ID id) const;
    std::shared_ptr<Room> GetRoomByName(const std::string& name) const;
    std::vector<std::string> GetAllRoomNames() const;

    bool MoveUserToRoom(ID user_id, const std::string& name);
    bool AddUserToRoom(const std::shared_ptr<User>& user, ID room_id);
    bool RemoveUserFromRoom(ID user_id);

private:
    mutable std::mutex mutex_;
    ID next_room_id_ = 1;
    std::unordered_map<ID, std::shared_ptr<Room>> id_to_room_;
    std::unordered_map<std::string, std::shared_ptr<Room>> name_to_room_;
};

}  // namespace chat