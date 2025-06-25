#pragma once

#include "db_wrapper.h"

#include <drogon/plugins/Plugin.h>


// Плагин для подключения к базе данных
class DatabasePlugin : public drogon::Plugin<DatabasePlugin> {
public:
    void initAndStart(const Json::Value &config) override;
    void shutdown() override;

    static std::shared_ptr<IRCDBWrapper> GetDB();

private:
    static std::shared_ptr<IRCDBWrapper> db_wrapper_;
};
