#include <gtest/gtest.h>
#include "postgres.h"
#include <pqxx/pqxx>

// Для каждого теста подготавливается чистая база
class PostgresRepoTest : public ::testing::Test {
protected:
    void SetUp() override {
        pool = std::make_shared<postgres::ConnectionPool>(1, []{
            return std::make_shared<pqxx::connection>(
                "host=localhost dbname=test_db user=test_user password=test_password"
            );
        });
        db = std::make_unique<postgres::Database>(pool);

        auto conn = db->GetTransaction();
        pqxx::work txn(*conn);
        txn.exec("DELETE FROM room_members;");
        txn.exec("DELETE FROM messages;");
        txn.exec("DELETE FROM rooms;");
        txn.exec("DELETE FROM users;");
        txn.commit();
    }

    std::shared_ptr<postgres::ConnectionPool> pool;
    std::unique_ptr<postgres::Database> db;
};

// --- UsersRepository ---
TEST_F(PostgresRepoTest, UsersRepository_SaveAndLoadAndFind) {
    auto conn = db->GetTransaction();
    pqxx::work txn(*conn);
    postgres::UsersRepository repo(txn);

    repo.Save("testuser", "hash123");
    repo.Save("second", "hash456");

    auto users = repo.LoadAll();
    EXPECT_EQ(users.size(), 2);

    auto found = repo.FindByUsername("second");
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->username, "second");
    EXPECT_EQ(found->password_hash, "hash456");

    // Несуществующий
    auto not_found = repo.FindByUsername("nope");
    EXPECT_FALSE(not_found.has_value());
}

// --- RoomsRepository ---
TEST_F(PostgresRepoTest, RoomsRepository_SaveAndLoad) {
    {
        auto conn = db->GetTransaction();
        pqxx::work txn(*conn);
        postgres::RoomsRepository repo(txn);
        repo.Save("room1");
        txn.commit();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    {
        auto conn = db->GetTransaction();
        pqxx::work txn(*conn);
        postgres::RoomsRepository repo(txn);
        repo.Save("room2");
        auto rooms = repo.LoadAll();
        EXPECT_EQ(rooms.size(), 2);
        EXPECT_EQ(rooms[0].name, "room2"); // loadAll: ORDER BY created_at DESC
        EXPECT_EQ(rooms[1].name, "room1");
        txn.commit();
    }
}

// --- MessagesRepository ---
TEST_F(PostgresRepoTest, MessagesRepository_SaveAndLoadRecent) {
    auto conn = db->GetTransaction();
    pqxx::work txn(*conn);

    // Для сообщения нужен пользователь и комната
    postgres::UsersRepository users(txn);
    users.Save("msguser", "phash");
    auto user = users.FindByUsername("msguser");
    ASSERT_TRUE(user.has_value());

    postgres::RoomsRepository rooms(txn);
    rooms.Save("msgroom");
    auto room = rooms.LoadAll().front();

    postgres::MessagesRepository msgs(txn);
    msgs.Save(user->id, room.id, "Hello!");
    msgs.Save(user->id, room.id, "Second message");
    auto loaded = msgs.LoadRecent(room.id, 5);
    ASSERT_EQ(loaded.size(), 2);
    EXPECT_EQ(loaded[0].message, "Second message");
    EXPECT_EQ(loaded[1].message, "Hello!");

    // Ограничение выборки
    auto one = msgs.LoadRecent(room.id, 1);
    ASSERT_EQ(one.size(), 1);
    EXPECT_EQ(one[0].message, "Second message");
}

// --- RoomMembersRepository ---
TEST_F(PostgresRepoTest, RoomMembersRepository_SaveLoadRemove) {
    auto conn = db->GetTransaction();
    pqxx::work txn(*conn);

    postgres::UsersRepository users(txn);
    users.Save("memberuser", "phash");
    auto user = users.FindByUsername("memberuser");
    ASSERT_TRUE(user.has_value());

    postgres::RoomsRepository rooms(txn);
    rooms.Save("roomA");
    auto room = rooms.LoadAll().front();

    postgres::RoomMembersRepository members(txn);

    // Добавить в комнату
    members.Save(user->id, room.id);
    auto in_room = members.LoadMembers(room.id);
    EXPECT_EQ(in_room.size(), 1);
    EXPECT_EQ(in_room[0].user_id.ToString(), user->id.ToString());

    // Повторное добавление (ON CONFLICT) — не будет дублей
    members.Save(user->id, room.id);
    auto after_conflict = members.LoadMembers(room.id);
    EXPECT_EQ(after_conflict.size(), 1);

    // Удалить из комнаты
    members.Remove(user->id, room.id);
    auto after_remove = members.LoadMembers(room.id);
    EXPECT_EQ(after_remove.size(), 0);
}

TEST_F(PostgresRepoTest, RoomMembersRepository_LoadRoomsByUser) {
    std::optional<postgres::UserRecord> alice;
    postgres::RoomRecord r1;
    postgres::RoomRecord r2;
    {
        auto conn = db->GetTransaction();
        pqxx::work txn(*conn);

        postgres::UsersRepository users(txn);
        users.Save("alice", "hash1");
        alice = users.FindByUsername("alice");
        txn.commit();
        ASSERT_TRUE(alice.has_value());
    }
    {
        auto conn = db->GetTransaction();
        pqxx::work txn(*conn);

        postgres::RoomsRepository rooms(txn);
        rooms.Save("r1");
        txn.commit();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    {
        auto conn = db->GetTransaction();
        pqxx::work txn(*conn);

        postgres::RoomsRepository rooms(txn);
        rooms.Save("r2");
        r1 = rooms.LoadAll()[1]; // r1 - старее, r2 - новее
        r2 = rooms.LoadAll()[0];
        txn.commit();
    }
    {
        auto conn = db->GetTransaction();
        pqxx::work txn(*conn);

        postgres::RoomMembersRepository members(txn);
        members.Save(alice->id, r1.id);
        txn.commit();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    {
        auto conn = db->GetTransaction();
        pqxx::work txn(*conn);

        postgres::RoomMembersRepository members(txn);
        members.Save(alice->id, r2.id);
        auto rooms_by_user = members.LoadRooms(alice->id);
        txn.commit();
        EXPECT_EQ(rooms_by_user.size(), 2);
        EXPECT_EQ(rooms_by_user[0].room_id.ToString(), r2.id.ToString());
        EXPECT_EQ(rooms_by_user[1].room_id.ToString(), r1.id.ToString());
    }
}

// --- TaggedUUID ---
TEST(TaggedUUIDTest, UUIDGenerateAndStringRoundtrip) {
    auto uuid = util::TaggedUUID<struct TestTag>::New();
    std::string str = uuid.ToString();
    auto from_str = util::TaggedUUID<struct TestTag>::FromString(str);
    EXPECT_EQ(uuid.ToString(), from_str.ToString());
}

TEST(TaggedUUIDTest, ZeroUUID) {
    util::TaggedUUID<struct TestTag> zero;
    EXPECT_EQ(zero.ToString(), util::detail::UUIDToString(util::detail::ZeroUUID));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
