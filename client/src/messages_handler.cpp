#include "messages_handler.h"

#include <boost/json.hpp>
#include <const.h>

void MessagesHandler::DoOnReceive() {
    std::string message = tcp_client_.Receive();


    boost::json::value parsed = boost::json::parse(message);
    boost::json::object message_obj = parsed.as_object();

    const std::string action_str = message_obj[CONSTANTS::LF_ACTION].as_string().c_str();
    message_obj.erase(CONSTANTS::LF_ACTION);

    std::unordered_map<std::string, std::string> params;
    for (const auto& item : message_obj) {
        params[std::string(item.key())] = item.value().as_string().c_str();
    }
    actions_[action_str](params);
}

void MessagesHandler::AddAction(const std::string& action, action_function action_func) {
    actions_[action] = action_func;
}
