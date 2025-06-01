#ifndef CLIENT_TRANSFER_I_H
#define CLIENT_TRANSFER_I_H

#include <string>

class ClientTransferInterface {
public:
    virtual ~ClientTransferInterface() = default;
    virtual void Connect(const std::string& host, unsigned short port) = 0;
    virtual void Send(const std::string& message) = 0;
    virtual std::string Receive() = 0;
    virtual void Close() = 0;
    virtual bool IsConnected() const = 0;
};

#endif // CLIENT_TRANSFER_I_H
