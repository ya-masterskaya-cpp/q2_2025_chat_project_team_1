#pragma once

#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <vector>


namespace chat {

inline const std::string GENERAL_ROOM = "general"; // TODO вынести в константы

using ID = uint32_t;

class Room;

class User {
public:
    User(ID id,
         const std::string& name,
         const std::string& password_hash);

    ID GetId() const;
    const std::string& GetName() const;
    const std::string& GetPasswordHash() const;

    std::shared_ptr<Room> GetRoom() const;
    void SetRoom(std::shared_ptr<Room> room);

private:
    ID id_;
    std::string name_;
    std::string password_hash_;
    std::weak_ptr<Room> room_;
};

class Room {
public:
    Room(ID id, const std::string& name);

    ID GetId() const;
    const std::string& GetName() const;

    void AddUser(const std::shared_ptr<User>& user);
    void RemoveUser(ID user_id);

    std::vector<std::shared_ptr<User>> GetUsers() const;
    std::vector<std::string> GetUserNames() const;
    std::shared_ptr<User> GetUserByName(const std::string& name) const;

private:
    ID id_;
    std::string name_;
    mutable std::mutex mutex_;
    std::vector<std::shared_ptr<User>> users_;
};

}  // namespace chat
