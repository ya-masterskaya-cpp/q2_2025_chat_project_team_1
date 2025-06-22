#pragma once

#include "chat_models.h"

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>


namespace chat {

class TokenManager {
public:
    void SaveToken(ID user_id, const std::string& token);
    void RemoveTokenByUserId(ID user_id);
    void RemoveTokenByToken(const std::string& token);

    std::optional<std::string> GetTokenByUserId(ID user_id) const;
    std::optional<ID> GetUserIdByToken(const std::string& token) const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<ID, std::string> id_to_token_;
    std::unordered_map<std::string, ID> token_to_id_;
};

}  // namespace chat
