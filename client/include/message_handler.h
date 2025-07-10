#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include <string>

#include "domain.h"

namespace domain {

class MessageHandler {
public:
    MessageHandler(UserData& user, const std::string& url);

    // REST API for authentication
    ServerResponse RegisterUser(const std::string& login, const std::string& password);
    ServerResponse LoginUser(const std::string& login, const std::string& password);
    ServerResponse LogoutUser();

    // REST API for all users
    ServerResponse GetOnlineUsers();

    //REST API for working with messages
    ServerResponse SendMessage(const std::string& text,
                            const std::string& to = ""); // to all default (Broadcast)
    ServerResponse GetRoomsRecentMessages(const std::string& room_name,
                                          size_t messages_count = 10);
    ServerResponse UploadMessageToDB(const std::string& message);


    // REST API for working with rooms
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
    template<typename Function>
    ServerResponse ToRequest(Function function) {
        auto res = function;
        if(res.error) {
            return {res.status_code == 200, res.text, res.error.message};
        }
        return {res.status_code == 200, res.text};
    }

};

}   //namespace gui

#endif  //REQUEST_HANDLER_H
