#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include "postgres.h"

class IRCDBWrapper {
public:
    IRCDBWrapper(const std::string& host,
                 int port,
                 const std::string& dbname,
                 const std::string& user,
                 const std::string& password,
                 size_t pool_size = 1);

    std::pair<bool, std::string> AddUserToDB(const std::string& name, const std::string& pass_hash);

    std::unordered_map<std::string, std::string> GetAllUsers();

private:
    std::shared_ptr<postgres::ConnectionPool> pool_;
};
