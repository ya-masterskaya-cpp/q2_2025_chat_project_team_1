#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <ixwebsocket/IXWebSocket.h>
#include <memory>
#include <optional>

using Callback = std::function<void(const std::string& msg)>;

#include "client_interface.h"

namespace transfer {

class WebSocketClient : public ClientInterface
{
public:
    WebSocketClient(const std::string& ip, int port, const std::string& token = "");
    virtual void Run() override;
    virtual void Stop() override;
    void SetToken(const std::string& token);
    void SetOnMessage(Callback callback);
    void SetOnOpen(Callback callback);
    void SetOnClose(Callback callback);
    void SetOnError(Callback callback);

private:
    std::unique_ptr<ix::WebSocket> ws_client_;
    std::string url_;
    std::string token_;

    std::optional<Callback> msg_handler_{std::nullopt};
    std::optional<Callback> open_handler_{std::nullopt};
    std::optional<Callback> close_handler_{std::nullopt};
    std::optional<Callback> error_handler_{std::nullopt};
};

}   //namespace transfer

#endif // WEBSOCKET_CLIENT_H
