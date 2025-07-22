#include "token_manager.h"

#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>
#include "postgres.h" // для UserId

using chat::TokenManager;
using postgres::UserId;

class TokenManagerTest : public ::testing::Test {
protected:
    TokenManager tm;
};

TEST_F(TokenManagerTest, SaveAndGetUserIdAndToken) {
    UserId user_id = UserId::FromString("00000000-0000-0000-0000-000000000001");
    const std::string token = "user_token";

    tm.SaveToken(user_id, token);

    auto opt_token = tm.GetTokenByUserId(user_id);
    EXPECT_TRUE(opt_token.has_value());
    EXPECT_EQ(opt_token.value(), token);

    auto opt_user = tm.GetUserIdByToken(token);
    EXPECT_TRUE(opt_user.has_value());
    EXPECT_EQ(opt_user.value(), user_id);
}

TEST_F(TokenManagerTest, OverwriteTokenForUserId) {
    UserId user_id = UserId::FromString("00000000-0000-0000-0000-000000000001");
    const std::string token1 = "token1";
    const std::string token2 = "token2";

    tm.SaveToken(user_id, token1);
    tm.SaveToken(user_id, token2);

    auto opt_token = tm.GetTokenByUserId(user_id);
    EXPECT_TRUE(opt_token.has_value());
    EXPECT_EQ(opt_token.value(), token2);

    EXPECT_FALSE(tm.GetUserIdByToken(token1).has_value());
    auto opt_user2 = tm.GetUserIdByToken(token2);
    EXPECT_TRUE(opt_user2.has_value());
    EXPECT_EQ(opt_user2.value(), user_id);
}

TEST_F(TokenManagerTest, RemoveTokenByUserId) {
    UserId user_id = UserId::FromString("00000000-0000-0000-0000-000000000001");
    const std::string token = "user_token";

    tm.SaveToken(user_id, token);
    tm.RemoveTokenByUserId(user_id);

    EXPECT_FALSE(tm.GetTokenByUserId(user_id).has_value());
    EXPECT_FALSE(tm.GetUserIdByToken(token).has_value());
}

TEST_F(TokenManagerTest, RemoveTokenByToken) {
    UserId user_id = UserId::FromString("00000000-0000-0000-0000-000000000001");
    const std::string token = "user_token";

    tm.SaveToken(user_id, token);
    tm.RemoveTokenByToken(token);

    EXPECT_FALSE(tm.GetTokenByUserId(user_id).has_value());
    EXPECT_FALSE(tm.GetUserIdByToken(token).has_value());
}

TEST_F(TokenManagerTest, GetEmptyUserIdByNonExistingToken) {
    UserId user_id = UserId::FromString("00000000-0000-0000-0000-000000000001");
    EXPECT_FALSE(tm.GetTokenByUserId(user_id).has_value());
    EXPECT_FALSE(tm.GetUserIdByToken("non_existing_token").has_value());
}

TEST_F(TokenManagerTest, GetUserIDs) {
    UserId id1 = UserId::FromString("00000000-0000-0000-0000-000000000001");
    UserId id2 = UserId::FromString("00000000-0000-0000-0000-000000000002");
    const std::string token1 = "token1";
    const std::string token2 = "token2";
    tm.SaveToken(id1, token1);
    tm.SaveToken(id2, token2);

    auto users = tm.GetUserIds();
    std::sort(users.begin(), users.end());

    EXPECT_EQ(users.size(), 2);
    EXPECT_EQ(users[0], id1);
    EXPECT_EQ(users[1], id2);
}

TEST_F(TokenManagerTest, ExpirationByTimeout) {
    UserId id1 = UserId::FromString("00000000-0000-0000-0000-000000000001");
    UserId id2 = UserId::FromString("00000000-0000-0000-0000-000000000002");
    const std::string token1 = "token1";
    const std::string token2 = "token2";

    tm.SaveToken(id1, token1);
    tm.SaveToken(id2, token2);

    // Ждём 20ms
    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    // Оба токена просрочены при таймауте 10 мс (TTL)
    {
        auto expired = tm.GetExpiredTokens(std::chrono::milliseconds(10));
        std::sort(expired.begin(), expired.end());
        std::vector<std::string> want = {token1, token2};
        EXPECT_EQ(expired, want);
    }

    // Оба токена активны при таймауте 30 мс (TTL)
    {
        auto expired = tm.GetExpiredTokens(std::chrono::milliseconds(30));
        EXPECT_TRUE(expired.empty());
    }
}

TEST_F(TokenManagerTest, ExpirationAfterUpdateActivity) {
    UserId id = UserId::FromString("00000000-0000-0000-0000-000000000001");
    const std::string token = "token1";

    tm.SaveToken(id, token);
    std::this_thread::sleep_for(std::chrono::milliseconds(8));

    // Обновляем время активности токена
    tm.UpdateActivityByToken(token);

    // Токен будет активен, так как с момента обновления активности пройдет 8 мс, а его TTL 10 мс
    std::this_thread::sleep_for(std::chrono::milliseconds(8));
    {
        auto expired = tm.GetExpiredTokens(std::chrono::milliseconds(10));
        EXPECT_TRUE(expired.empty());
    }

    // Токен будет просрочен спутся 8 + 8 = 16 мс, при его TTL = 10 мс
    std::this_thread::sleep_for(std::chrono::milliseconds(8));
    {
        auto expired = tm.GetExpiredTokens(std::chrono::milliseconds(10));
        EXPECT_EQ(expired, std::vector<std::string>({token}));
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
