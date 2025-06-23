#pragma once

#include <condition_variable>
#include <pqxx/pqxx>
#include <memory>
#include <vector>
#include <optional>
#include <string>
#include <mutex>

#include "tagged_uuid.h"

namespace postgres {

// ---- Идентификаторы через TaggedUUID ----
namespace detail {

struct UserIdTag {};
struct RoomIdTag {};
struct MessageIdTag {};
struct RoomMemberIdTag {};
}

using UserId = util::TaggedUUID<detail::UserIdTag>;
using RoomId = util::TaggedUUID<detail::RoomIdTag>;
using MessageId = util::TaggedUUID<detail::MessageIdTag>;
using RoomMemberId = util::TaggedUUID<detail::RoomMemberIdTag>;

// ---- Структуры записей ----
struct UserRecord {
    UserId id;
    std::string username;
    std::string password_hash;
    std::string registered_at;
};

struct RoomRecord {
    RoomId id;
    std::string name;
    std::string created_at;
};

struct MessageRecord {
    MessageId id;
    UserId user_id;
    RoomId room_id;
    std::string message;
    std::string sent_at;
};

struct RoomMemberRecord {
    RoomMemberId id;
    RoomId room_id;
    UserId user_id;
    std::string joined_at;
};

// ---- Пул соединений ----
class ConnectionPool {
    using PoolType = ConnectionPool;
    using ConnectionPtr = std::shared_ptr<pqxx::connection>;

public:
    class ConnectionWrapper {
    public:
        ConnectionWrapper(std::shared_ptr<pqxx::connection>&& conn, PoolType& pool) noexcept;

        ConnectionWrapper(const ConnectionWrapper&) = delete;
        ConnectionWrapper& operator=(const ConnectionWrapper&) = delete;

        ConnectionWrapper(ConnectionWrapper&&) = default;
        ConnectionWrapper& operator=(ConnectionWrapper&&) = default;

        pqxx::connection& operator*() const& noexcept;
        pqxx::connection& operator*() const&& = delete;
        pqxx::connection* operator->() const& noexcept;

        ~ConnectionWrapper();

    private:
        std::shared_ptr<pqxx::connection> conn_;
        PoolType* pool_;
    };

    template <typename ConnectionFactory>
    ConnectionPool(size_t capacity, ConnectionFactory&& connection_factory);

    ConnectionWrapper GetConnection();
    std::optional<ConnectionWrapper> GetConnection(std::chrono::milliseconds timeout);

private:
    void ReturnConnection(ConnectionPtr&& conn);

    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::vector<ConnectionPtr> pool_;
    size_t used_connections_ = 0;
};

template<typename ConnectionFactory>
ConnectionPool::ConnectionPool(size_t capacity, ConnectionFactory &&connection_factory) {
    pool_.reserve(capacity);
    for (size_t i = 0; i < capacity; ++i) {
        pool_.emplace_back(connection_factory());
    }
}

// ---- Репозиторий пользователей ----
class UsersRepository {
public:
    explicit UsersRepository(pqxx::work& transaction);

    void Save(const std::string& username, const std::string& password_hash) const;
    std::vector<UserRecord> LoadAll() const;
    std::optional<UserRecord> FindById(const UserId& id) const;
    std::optional<UserRecord> FindByUsername(const std::string& username) const;
    bool DeleteByUsername(const std::string& username) const;
    std::vector<UserRecord> LoadPage(int offset, int limit) const;
    std::unordered_map<std::string, std::string> LoadUserMap() const;

private:
    pqxx::work& transaction_;
};

// ---- Репозиторий комнат ----
class RoomsRepository {
public:
    explicit RoomsRepository(pqxx::work& transaction);

    void Save(const std::string& name) const;
    std::optional<RoomRecord> FindByName(const std::string& name) const;
    std::vector<RoomRecord> LoadAll() const;
    std::vector<RoomRecord> LoadPage(int offset, int limit) const;
    void DeleteById(const RoomId& id) const;

private:
    pqxx::work& transaction_;
};

// ---- Репозиторий сообщений ----
class MessagesRepository {
public:
    explicit MessagesRepository(pqxx::work& transaction);

    void Save(const UserId& user_id, const RoomId& room_id, const std::string& message) const;
    std::vector<MessageRecord> LoadRecent(const RoomId& room_id, int max_items) const;
    std::vector<MessageRecord> LoadPage(const RoomId& room_id, int offset, int limit) const;
    void DeleteById(const MessageId& id) const;


private:
    pqxx::work& transaction_;
};

// ---- Репозиторий участников комнат ----
class RoomMembersRepository {
public:
    explicit RoomMembersRepository(pqxx::work& transaction);

    void Save(const UserId& user_id, const RoomId& room_id) const;
    void Remove(const UserId& user_id, const RoomId& room_id) const;
    std::vector<RoomMemberRecord> LoadMembers(const RoomId& room_id) const;
    std::vector<RoomMemberRecord> LoadRooms(const UserId& user_id) const;

private:
    pqxx::work& transaction_;
};

// ---- Основная база данных ----
class Database {
public:
    explicit Database(std::shared_ptr<ConnectionPool> pool_ptr);

    [[nodiscard]] ConnectionPool::ConnectionWrapper GetTransaction() const;

private:
    std::shared_ptr<ConnectionPool> pool_ptr_;
};

} // namespace postgres
