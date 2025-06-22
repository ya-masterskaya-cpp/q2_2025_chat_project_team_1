#include "chat_websocket.h"


// описание ?
std::mutex ChatWebSocket::conn_mutex_;
std::unordered_map<std::string, drogon::WebSocketConnectionPtr> ChatWebSocket::connections_;

void ChatWebSocket::handleNewConnection(const drogon::HttpRequestPtr &req, const drogon::WebSocketConnectionPtr &conn) {
    const auto &query = req->getParameters();
    auto it = query.find("token");
    if (it == query.end()) {
        conn->shutdown();
        std::cout << "[ChatWS] missing token\n";
        return;
    }

    const std::string &token = it->second;
    std::string user;

    auto chat_service = ChatServicePlugin::GetService();
    if (!chat_service->GetUserByToken(token)) {
        conn->shutdown();
        std::cout << "[ChatWS] invalid token\n";
        return;
    }

    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        connections_[user] = conn;
    }

    // TODO!!! проверить появление пользователя в general есть ли в методе LOGIN? (если первое появление при входе и появление при входе после выхода)
    //chat_service->JoinRoom(token, "general"); // при логине пользователь появляется в общей комнате 

    std::cout << "[ChatWS] connected: " << user << "\n"; // TODO отладка, нужны логи
    std::cout << "[ChatWS] client in room: " << chat_service->GetCurrentRoomName(token) << "\n"; // TODO отладка, нужны логи
}

void ChatWebSocket::handleNewMessage(const drogon::WebSocketConnectionPtr &ws_conn, std::string &&message, const drogon::WebSocketMessageType &) {
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(message, root)) {
        return;
    }

    const std::string text = root.get("text", "").asString();
    if (text.empty()) {
        return;
    }

    std::string token;
    
    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        for (const auto &[user, conn] : connections_) {
            if (conn == ws_conn) {
                // найти токен по имени пользователя
                auto chat_service = ChatServicePlugin::GetService();
                auto token_opt = chat_service->GetTokenByUserName(user);
                if (token_opt) {
                    token = token_opt.value();
                }
                break;
            }
        }
    }

    if (token.empty()) {
        return;
    }

    // сериализуем ответ
    auto chat_service = ChatServicePlugin::GetService();
    auto user_ptr = chat_service->GetUserByToken(token);
    if (!user_ptr) return;

    Json::Value response;
    response["from"] = user_ptr->GetName();
    response["text"] = text;

    std::string serialized = Json::FastWriter().write(response);

    std::cout << "[ChatWS] message received: " << text << "\n"; // TODO отладка, нужны логи
    Broadcast(token, serialized);
}

void ChatWebSocket::handleConnectionClosed(const drogon::WebSocketConnectionPtr &conn) {
    std::lock_guard<std::mutex> lock(conn_mutex_);
    for (auto it = connections_.begin(); it != connections_.end(); ++it) {
        if (it->second == conn) {
            std::cout << "[ChatWS] client disconnected: " << it->first << "\n";
            //RoomManager::instance().LeaveRoom(it->first); // удаление по выходу в Logout
            connections_.erase(it);
            break;
        }
    }
}

std::vector<std::string> ChatWebSocket::GetConnectedUsers() {
    std::lock_guard<std::mutex> lock(conn_mutex_);
    std::vector<std::string> users;
    for (const auto& [login, _] : connections_) {
        users.push_back(login);
    }
    return users;
}

//void ChatWebSocket::Broadcast(const std::string &from, const std::string &to, const std::string &message) {
void ChatWebSocket::Broadcast(const std::string& token, const std::string& message) {
    std::lock_guard<std::mutex> lock(conn_mutex_);

    auto chat_service = ChatServicePlugin::GetService();
    auto room_name = chat_service->GetCurrentRoomName(token);

    std::cout << "[ChatWS] broadcast in room: " << room_name << "\n"; // TODO отладка, нужны логи

    auto room_user_names = chat_service->GetUserNamesInCurrentRoom(token);
    for (const auto &[user, conn] : connections_) {
        if (std::find(room_user_names.begin(), room_user_names.end(), user) != room_user_names.end()) {
            conn->send(message);
        }
    }
}