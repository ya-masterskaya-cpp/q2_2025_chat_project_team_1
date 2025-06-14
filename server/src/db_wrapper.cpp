#include "db_wrapper.h"
#include <pqxx/pqxx>
#include <sstream>

IRCDBWrapper::IRCDBWrapper(const std::string& host,
                           int port,
                           const std::string& dbname,
                           const std::string& user,
                           const std::string& password,
                           size_t pool_size)
{
    std::ostringstream conn_stream;
    conn_stream << "host=" << host
                << " port=" << port
                << " dbname=" << dbname
                << " user=" << user
                << " password=" << password;

    std::string conn_str = conn_stream.str();

    pool_ = std::make_shared<postgres::ConnectionPool>(pool_size, [conn_str] {
        return std::make_shared<pqxx::connection>(conn_str);
    });
    postgres::Database db(pool_);
}

std::pair<bool, std::string> IRCDBWrapper::AddUserToDB(const std::string& name, const std::string& pass_hash)
{
    try {
        auto conn = pool_->GetConnection();
        pqxx::work txn(*conn);
        postgres::UsersRepository repo(txn);

        if (repo.FindByUsername(name)) {
            return {false, "User already exists"};
        }

        repo.Save(name, pass_hash);
        txn.commit();
        return {true, ""};
    } catch (const std::exception& ex) {
        return {false, ex.what()};
    }
}

std::unordered_map<std::string, std::string> IRCDBWrapper::GetAllUsers()
{
    try {
        auto conn = pool_->GetConnection();
        pqxx::work txn(*conn);
        postgres::UsersRepository repo(txn);
        return repo.LoadUserMap();
    } catch (...) {
        // Можно добавить лог
        return {};
    }
}
