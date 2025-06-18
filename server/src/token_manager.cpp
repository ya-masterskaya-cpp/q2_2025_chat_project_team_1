#include "token_manager.h"


namespace chat {

void TokenManager::SaveToken(ID user_id, const std::string& token) {
    std::lock_guard<std::mutex> lock(mutex_);
    id_to_token_[user_id] = token;
    token_to_id_[token] = user_id;
}

void TokenManager::RemoveTokenByUserId(ID user_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = id_to_token_.find(user_id);
    if (it != id_to_token_.end()) {
        token_to_id_.erase(it->second);
        id_to_token_.erase(it);
    }
}

void TokenManager::RemoveTokenByToken(const std::string& token) {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = token_to_id_.find(token);
    if (it != token_to_id_.end()) {
        id_to_token_.erase(it->second);
        token_to_id_.erase(it);
    }
}

std::optional<std::string> TokenManager::GetTokenByUserId(ID user_id) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = id_to_token_.find(user_id);
    if (it != id_to_token_.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::optional<ID> TokenManager::GetUserIdByToken(const std::string& token) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = token_to_id_.find(token);
    if (it != token_to_id_.end()) {
        return it->second;
    }
    return std::nullopt;
}

}  // namespace chat
