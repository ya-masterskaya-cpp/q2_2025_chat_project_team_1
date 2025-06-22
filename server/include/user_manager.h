#pragma once

#include "chat_models.h"

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>


namespace chat {

class UserManager {
public:
    std::shared_ptr<User> RegisterUser(const std::string& name, const std::string& password_hash);
    std::shared_ptr<User> GetUserById(ID id) const;
    std::shared_ptr<User> GetUserByName(const std::string& name) const;
    std::vector<std::shared_ptr<User>> GetAllUsers() const;
    void RemoveUser(ID user_id);

private:
    mutable std::mutex mutex_;
    ID next_user_id_ = 1;
    std::unordered_map<ID, std::shared_ptr<User>> id_to_user_;
    std::unordered_map<std::string, std::shared_ptr<User>> name_to_user_;
};

}  // namespace chat
