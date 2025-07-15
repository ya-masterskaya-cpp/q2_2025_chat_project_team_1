#include "message_handler.h"

#include <const_api.h>
#include <json/json.h>
#include <password_hasher.h>

namespace domain {

MessageHandler::MessageHandler(UserData& user, const std::string& url)
    : user_{user} {
    query_handler_.SetUrl(url);

    query_handler_.AddEndpoint(api::AUTH_REGISTER)->SetHeader({{"Content-Type", "application/json"}});
    query_handler_.AddEndpoint(api::AUTH_LOGIN)->SetHeader({{"Content-Type", "application/json"}});
    query_handler_.AddEndpoint(api::AUTH_LOGOUT)->SetHeader({{"Content-Type", "application/json"}});
    query_handler_.AddEndpoint(api::USERS_ONLINE);
    query_handler_.AddEndpoint(api::MESSAGE_SEND)->SetHeader({{"Content-Type", "application/json"}});
    query_handler_.AddEndpoint(api::MESSAGE_RECENT);
    query_handler_.AddEndpoint(api::MESSAGE_UPLOAD)->SetHeader({{"Content-Type", "application/json"}});
    query_handler_.AddEndpoint(api::ROOM_CREATE)->SetHeader({{"Content-Type", "application/json"}});
    query_handler_.AddEndpoint(api::ROOM_JOIN)->SetHeader({{"Content-Type", "application/json"}});
    query_handler_.AddEndpoint(api::ROOM_LEAVE)->SetHeader({{"Content-Type", "application/json"}});
    query_handler_.AddEndpoint(api::ROOM_LIST);
    query_handler_.AddEndpoint(api::ROOM_CURRENT);
    query_handler_.AddEndpoint(api::ROOM_USERS);
}

ServerResponse MessageHandler::RegisterUser(const std::string& login, const std::string& password) {
    Json::Value body;
    body["login"] = login;
    body["password"] = PasswordHasher::HashPassword(password);
    auto& query = query_handler_.At(api::AUTH_REGISTER);
    query.SetBody(Json::writeString(Json::StreamWriterBuilder{}, body));
    return ToRequest(SendPostRequest(query_handler_.GetUrl(), query),201);
}

ServerResponse MessageHandler::LoginUser(const std::string& login, const std::string& password) {
    Json::Value body;
    body["login"] = login;
    body["password"] = PasswordHasher::HashPassword(password);
    auto& query = query_handler_.At(api::AUTH_LOGIN);
    query.SetBody(Json::writeString(Json::StreamWriterBuilder{}, body));
    auto res = SendPostRequest(query_handler_.GetUrl(), query);
    if(res.error) {
        return {res.status_code == 200, res.text, res.error.message};
    }

    if (res.status_code == 200 && ParseTokenFromJson(res.text)) {
        user_.name = login;
        return {true,res.text};
    } else {
        return {false,res.text};
    }
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
    auto& query = query_handler_.At(api::AUTH_LOGOUT);
    query.SetToken(user_.token);
    auto res = SendPostRequest(query_handler_.GetUrl(), query);
    if(res.error) {
        return {res.status_code == 200, res.text, res.error.message};
    }
    if(res.status_code == 200) {
        user_.token.clear();
    }
    return{res.status_code == 200, res.text};
}

ServerResponse MessageHandler::GetOnlineUsers() {
    auto& query = query_handler_.At(api::USERS_ONLINE);
    query.SetToken(user_.token);
    return ToRequest(SendGetRequest(query_handler_.GetUrl(),query));
}

ServerResponse MessageHandler::SendMessage(const std::string& text, const std::string& to) {
    Json::Value body;
    body["text"] = text;
    body["to"] = to;
    auto& query = query_handler_.At(api::MESSAGE_SEND);
    query.SetToken(user_.token);
    query.SetBody(Json::writeString(Json::StreamWriterBuilder{}, body));
    return ToRequest(SendPostRequest(query_handler_.GetUrl(),query));
}

ServerResponse MessageHandler::GetRoomsRecentMessages(const std::string& room_name, size_t messages_count) {
    auto& query = query_handler_.At(api::MESSAGE_RECENT);
    query.SetParameters({{"room",room_name},{"max_items",std::to_string(messages_count)}});
    query.SetToken(user_.token);
    return ToRequest(SendGetRequest(query_handler_.GetUrl(),query));
}

ServerResponse MessageHandler::UploadMessageToDB(const std::string& message) {
    Json::Value body;
    body["text"] = message;
    auto& query = query_handler_.At(api::MESSAGE_UPLOAD);
    query.SetBody(Json::writeString(Json::StreamWriterBuilder{}, body));
    query.SetToken(user_.token);
    return ToRequest(SendPostRequest(query_handler_.GetUrl(), query));
}

ServerResponse MessageHandler::CreateRoom(const std::string& name) {
    Json::Value body;
    body["name"] = name;
    auto& query = query_handler_.At(api::ROOM_CREATE);
    query.SetBody(Json::writeString(Json::StreamWriterBuilder{}, body));
    query.SetToken(user_.token);
    auto res = SendPostRequest(query_handler_.GetUrl(), query);
    if(res.error) {
        return {res.status_code == 201, res.text, res.error.message};
    }
    return{res.status_code == 201, res.text};
}

ServerResponse MessageHandler::JoinRoom(const std::string& name) {
    Json::Value body;
    body["name"] = name;
    auto& query = query_handler_.At(api::ROOM_JOIN);
    query.SetBody(Json::writeString(Json::StreamWriterBuilder{}, body));
    query.SetToken(user_.token);
    return ToRequest(SendPostRequest(query_handler_.GetUrl(), query));
}

ServerResponse MessageHandler::LeaveRoom() {
    return ToRequest(SendPostRequest(query_handler_.GetUrl(),
                query_handler_.At(api::ROOM_LEAVE).SetToken(user_.token)));
}

ServerResponse MessageHandler::ListRooms() {
    return ToRequest(SendGetRequest(query_handler_.GetUrl(),
                query_handler_.At(api::ROOM_LIST).SetToken(user_.token)));
}

ServerResponse MessageHandler::GetCurrentRoom() {
    return ToRequest(SendGetRequest(query_handler_.GetUrl(),
                query_handler_.At(api::ROOM_CURRENT).SetToken(user_.token)));
}

ServerResponse MessageHandler::GetUsersInRoom(const std::string& roomName) {
    auto& query = query_handler_.At(api::ROOM_USERS);
    query.SetParameters({{"name", roomName}});
    query.SetToken(user_.token);
    return ToRequest(SendGetRequest(query_handler_.GetUrl(),query));
}

void MessageHandler::SetUrl(const std::string& url) {
    query_handler_.SetUrl(url);
}

}//namespace domain
