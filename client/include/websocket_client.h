#ifndef WEBSOCKET_CLIENT_H
#define WEBSOCKET_CLIENT_H

#include <ixwebsocket/IXWebSocket.h>
#include <memory>

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
    void SetWebSocket(std::unique_ptr<ix::WebSocket>&& ws_client);
    void SetWebSocket(ix::WebSocket* ws_client);
    const ix::WebSocket* const GetWebSocket() const;
    virtual ~WebSocketClient() = default;

private:
    std::unique_ptr<ix::WebSocket> ws_client_;
    std::string url_;
    std::string token_;

    Callback msg_handler_;
    Callback open_handler_;
    Callback close_handler_;
    Callback error_handler_;
};

}   //namespace transfer

#endif // WEBSOCKET_CLIENT_H
