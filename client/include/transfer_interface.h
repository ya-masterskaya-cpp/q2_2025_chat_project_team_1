#ifndef CLIENT_TRANSFER_I_H
#define CLIENT_TRANSFER_I_H

#include <string>

namespace transfer {

class ClientTransferInterface {
public:
    virtual ~ClientTransferInterface() = default;
    virtual void Connect(const std::string& host, unsigned short port) = 0;
    virtual void Send(const std::string& message) = 0;
    virtual std::string Receive(char delimeter = '\0') = 0;
    virtual void Close() = 0;
    virtual bool IsConnected() const = 0;
};

}//namespace transfer
#endif // CLIENT_TRANSFER_I_H
