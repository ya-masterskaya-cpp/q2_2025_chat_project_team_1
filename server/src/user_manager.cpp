#include "user_manager.h"


namespace chat {

std::shared_ptr<User> UserManager::RegisterUser(const std::string& name, const std::string& password_hash) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (name_to_user_.count(name)) {
        return nullptr;  // пользователь с таким именем уже существует
    }

    auto user = std::make_shared<User>(next_user_id_++, name, password_hash);
    id_to_user_[user->GetId()] = user;
    name_to_user_[name] = user;
    return user;
}

std::shared_ptr<User> UserManager::GetUserById(ID id) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = id_to_user_.find(id);
    return (it != id_to_user_.end()) ? it->second : nullptr;
}

std::shared_ptr<User> UserManager::GetUserByName(const std::string& name) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = name_to_user_.find(name);
    return (it != name_to_user_.end()) ? it->second : nullptr;
}

std::vector<std::shared_ptr<User>> UserManager::GetAllUsers() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<std::shared_ptr<User>> result;
    result.reserve(id_to_user_.size());
    for (const auto& [_, user] : id_to_user_) {
        result.push_back(user);
    }
    return result;
}

void UserManager::RemoveUser(ID user_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = id_to_user_.find(user_id);
    if (it != id_to_user_.end()) {
        name_to_user_.erase(it->second->GetName());
        id_to_user_.erase(it);
    }
}

}  // namespace chat
