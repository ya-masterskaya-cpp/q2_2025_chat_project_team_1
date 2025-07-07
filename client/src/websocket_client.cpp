#include "websocket_client.h"

#include <optional>

namespace transfer {

WebSocketClient::WebSocketClient(const std::string& ip, int port, const std::string& token)
    : token_{token} {
    url_ = "ws://" + ip + ":" + std::to_string(port) + "/ws/chat";
}

void WebSocketClient::Run() {
    ws_client_ = std::make_unique<ix::WebSocket>();
    std::string url = url_ + "?token=" + token_;
    ws_client_->setUrl(url);

    ws_client_->setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Message) {
            if(msg_handler_) {
                (*msg_handler_)(msg->str);
            }
        } else if (msg->type == ix::WebSocketMessageType::Open) {
            if(open_handler_) {
                (*open_handler_)("WebSocket is listining...");
            }
        } else if (msg->type == ix::WebSocketMessageType::Close) {
            if(close_handler_) {
                (*close_handler_)(msg->closeInfo.reason);
            }
        } else if (msg->type == ix::WebSocketMessageType::Error) {
            if(error_handler_) {
                (*error_handler_)(msg->errorInfo.reason);
            }
        }
    });

    ws_client_->start();
}

void WebSocketClient::Stop() {
    if (ws_client_) {
        ws_client_->stop();
        ws_client_.reset();
    }
}

void WebSocketClient::SetToken(const std::string& token){
    token_ = token;
}

void WebSocketClient::SetOnMessage(Callback callback) {
    msg_handler_ = callback;
}
void WebSocketClient::SetOnOpen(Callback callback) {
    open_handler_ = callback;
}
void WebSocketClient::SetOnClose(Callback callback) {
    close_handler_ = callback;
}
void WebSocketClient::SetOnError(Callback callback) {
    error_handler_ = callback;
}

void WebSocketClient::SetWebSocket(std::unique_ptr<ix::WebSocket>&& ws_client) {
    ws_client_ = std::move(ws_client);
}

void WebSocketClient::SetWebSocket(ix::WebSocket* ws_client) {
    ws_client_.reset(ws_client);
}

}   //namespace transfer
