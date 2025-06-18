#include "chat_models.h"


namespace chat {

// User

User::User(ID id, const std::string& name, const std::string& password_hash)
    : id_(id)
    , name_(name)
    , password_hash_(password_hash) {
}

ID User::GetId() const {
    return id_;
}

const std::string& User::GetName() const {
    return name_;
}

const std::string& User::GetPasswordHash() const {
    return password_hash_;
}

std::shared_ptr<Room> User::GetRoom() const {
    return room_.lock();
}

void User::SetRoom(std::shared_ptr<Room> room) {
    room_ = room;
}

// Room

Room::Room(ID id, const std::string& name)
    : id_(id), name_(name) {}

ID Room::GetId() const {
    return id_;
}

const std::string& Room::GetName() const {
    return name_;
}

void Room::AddUser(const std::shared_ptr<User>& user) {
    if (!user) return;
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find_if(users_.begin(), users_.end(),
                            [&](const std::shared_ptr<User>& u) {
                                return u->GetId() == user->GetId();
                            });
    if (it == users_.end()) {
        users_.push_back(user);
    }
}

void Room::RemoveUser(ID user_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    users_.erase(std::remove_if(users_.begin(), users_.end(),
                                [&](const std::shared_ptr<User>& u) {
                                    return u->GetId() == user_id;
                                }),
                 users_.end());
}

std::vector<std::shared_ptr<User>> Room::GetUsers() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return users_;
}

std::vector<std::string> Room::GetUserNames() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::string> names;
    names.reserve(users_.size());
    for (const auto& user : users_) {
        names.push_back(user->GetName());
    }
    return names;
}

std::shared_ptr<User> Room::GetUserByName(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = std::find_if(users_.begin(), users_.end(),
                            [&](const std::shared_ptr<User>& u) {
                                return u->GetName() == name;
                            });
    return (it != users_.end()) ? *it : nullptr;
}

}  // namespace chat