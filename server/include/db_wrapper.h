#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "postgres.h"

class IRCDBWrapper {
public:
    IRCDBWrapper(const std::string& conn_str, size_t pool_size = 1);

    std::pair<bool, std::string> AddUserToDB(const std::string& name, const std::string& pass_hash);

    std::optional<postgres::UserRecord> FindUserById(const postgres::UserId& id);

    std::optional<postgres::UserRecord> FindUserByName(const std::string& username);

    bool DeleteUserByName(const std::string& username);

    std::unordered_map<std::string, std::string> GetAllUsers();

    std::optional<postgres::RoomRecord> FindRoomByName(const std::string& room_name);

    std::pair<bool, std::string> AddRoomToDB(const std::string& room_name);

    std::vector<postgres::RoomRecord> GetAllRooms();

    std::pair<bool, std::string> AddUserToRoomByName(const std::string& username, const std::string& roomname);

    std::pair<bool, std::string> RemoveUserFromRoomByName(const std::string& username, const std::string& roomname);

    std::vector<postgres::RoomRecord> GetRoomsPage(int offset, int limit);

    std::pair<bool, std::string> DeleteRoomByName(const std::string& roomname);

    std::pair<bool, std::string> AddMessage(const std::string& username, const std::string& roomname, const std::string& text);

    std::vector<postgres::MessageRecord> GetRecentMessages(const std::string& roomname, int max_items);

    std::vector<postgres::MessageRecord> GetRoomMessagesPage(const std::string& roomname, int offset, int limit);

    bool DeleteMessageById(const postgres::MessageId& id);

private:
    std::shared_ptr<postgres::ConnectionPool> pool_;
};
