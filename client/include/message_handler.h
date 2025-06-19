#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

#include "domain.h"

namespace domain {

class MessageHandler {
public:
    MessageHandler(UserData& user, const std::string& url);

    // REST API для аутентификации: регистрация, логина, выхода
    ServerResponse RegisterUser(const std::string& login, const std::string& password);
    ServerResponse LoginUser(const std::string& login, const std::string& password);
    ServerResponse LogoutUser();

    // REST API список всех клиентов и рассылка
    ServerResponse GetOnlineUsers();
    ServerResponse SendMessage(const std::string& text,
                            const std::string& to = ""); // по умолчанию рассылка всем (Broadcast)

    // REST API для работы с комнатами
    ServerResponse CreateRoom(const std::string& name);
    ServerResponse JoinRoom(const std::string& name);
    ServerResponse LeaveRoom();
    ServerResponse ListRooms();
    ServerResponse GetCurrentRoom();
    ServerResponse GetUsersInRoom(const std::string& roomName);

    //
    void SetUrl(const std::string& url);

private:
    UserData& user_;
    std::string url_;

    bool ParseTokenFromJson(const std::string& jsonText);

};

}   //namespace gui

#endif  //REQUEST_HANDLER_H
