#pragma once

#include "postgres.h"

#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


namespace chat {

class TokenManager {
public:
    void SaveToken(postgres::UserId user_id, const std::string& token);
    void RemoveTokenByUserId(const postgres::UserId& user_id);
    void RemoveTokenByToken(const std::string& token);

    std::optional<std::string> GetTokenByUserId(const postgres::UserId& user_id) const;
    std::optional<postgres::UserId> GetUserIdByToken(const std::string& token) const;

    std::vector<postgres::UserId> GetOnlineUserIds() const;

private:
    mutable std::mutex mutex_;
    std::unordered_map<postgres::UserId, std::string, util::TaggedHasher<postgres::UserId>> id_to_token_;
    std::unordered_map<std::string, postgres::UserId> token_to_id_;
};

}  // namespace chat
