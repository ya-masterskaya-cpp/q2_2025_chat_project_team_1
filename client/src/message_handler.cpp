#include "message_handler.h"

#include <const_api.h>
#include <json/json.h>
#include <password_hasher.h>

#include <iostream>

namespace domain {

MessageHandler::MessageHandler(UserData& user, const std::string& url)
    : user_{user}, url_{url} {
}

ServerResponse MessageHandler::RegisterUser(const std::string& login, const std::string& password) {
    Json::Value body;
    body["login"] = login;
    body["password"] = PasswordHasher::HashPassword(password);
    auto res = SendPostRequest(url_ + std::string(api::AUTH_REGISTER), body);
    if(res.error) {
        return {res.status_code == 201, res.text, res.error.message};
    }
    return {res.status_code == 201, res.text};
}

ServerResponse MessageHandler::LoginUser(const std::string& login, const std::string& password) {
    Json::Value body;
    body["login"] = login;
    body["password"] = PasswordHasher::HashPassword(password);
    auto res = SendPostRequest(url_ + std::string(api::AUTH_LOGIN), body);
    if(res.error) {
        return {res.status_code == 200, res.text, res.error.message};
    }

    if (res.status_code == 200 && ParseTokenFromJson(res.text)) {
        user_.name = login;
        return {true,res.text};
    }

    user_.token.clear();
    return {false,res.text};
}

bool MessageHandler::ParseTokenFromJson(const std::string& jsonText) {
    Json::CharReaderBuilder reader;
    Json::Value root;
    std::string errs;
    std::istringstream s(jsonText);
    if (Json::parseFromStream(reader, s, &root, &errs)) {
        if (root.isMember("token")) {
            user_.token = root["token"].asString();
            return true;
        }
    }
    return false;
}

ServerResponse MessageHandler::LogoutUser() {
    auto res = SendPostRequest(url_ + std::string(api::AUTH_LOGOUT), Json::objectValue, user_.token);
    if(res.error) {
        return {res.status_code == 200, res.text, res.error.message};
    }
    if(res.status_code == 200) {
        user_.token.clear();
    }
    return{res.status_code == 200, res.text};
}

ServerResponse MessageHandler::GetOnlineUsers() {
    return ToRequest(SendGetRequest(std::string(api::USERS_ONLINE), user_.token));
}

ServerResponse MessageHandler::SendMessage(const std::string& text, const std::string& to) {
    Json::Value body;
    body["text"] = text;
    body["to"] = to;
    return ToRequest(SendPostRequest(url_ + std::string(api::MESSAGE_SEND), body,user_.token));
}

ServerResponse MessageHandler::GetRoomsRecentMessages(const std::string& room_name, size_t messages_count) {
    return ToRequest(SendGetRequest(url_ + std::string(api::MESSAGE_RECENT) + "?room=" + room_name +
                                        "&max_items=" + std::to_string(messages_count),user_.token));
}

ServerResponse MessageHandler::UploadMessageToDB(const std::string& message) {
    Json::Value body;
    body["text"] = message;
    return ToRequest(SendPostRequest(url_ + std::string(api::MESSAGE_UPLOAD), body,user_.token));
}

ServerResponse MessageHandler::CreateRoom(const std::string& name) {
    Json::Value body;
    body["name"] = name;
    auto res = SendPostRequest(url_ + std::string(api::ROOM_CREATE), body, user_.token);
    if(res.error) {
        return {res.status_code == 201, res.text, res.error.message};
    }
    return{res.status_code == 201, res.text};
}

ServerResponse MessageHandler::JoinRoom(const std::string& name) {
    Json::Value body;
    body["name"] = name;
    return ToRequest(SendPostRequest(url_ + std::string(api::ROOM_JOIN), body, user_.token));
}

ServerResponse MessageHandler::LeaveRoom() {
    return ToRequest(SendPostRequest(url_ + std::string(api::ROOM_LEAVE), Json::objectValue, user_.token));
}

ServerResponse MessageHandler::ListRooms() {
    return ToRequest(SendGetRequest(url_ + std::string(api::ROOM_LIST), user_.token));
}

ServerResponse MessageHandler::GetCurrentRoom() {
    return ToRequest(SendGetRequest(url_ + std::string(api::ROOM_CURRENT), user_.token));
}

ServerResponse MessageHandler::GetUsersInRoom(const std::string& roomName) {
    return ToRequest(SendGetRequest(url_ + std::string(api::ROOM_USERS) + "?name=" + roomName, user_.token));
}

void MessageHandler::SetUrl(const std::string& url) {
    url_ = url;
}



}//namespace domain
