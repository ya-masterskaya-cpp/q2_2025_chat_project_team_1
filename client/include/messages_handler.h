#ifndef MESSAGESHANDLER_H
#define MESSAGESHANDLER_H


#include <functional>
#include <unordered_map>

#include "tcp_client.h"

namespace transfer {

using action_function = std::function<void(const
                            std::unordered_map<std::string,std::string>&)>;

class MessagesHandlerObserver {
public:
    virtual void Received() = 0;
};

class MessagesHandler
{
public:
    MessagesHandler(transfer::TcpCLient& tcp_client)
        : tcp_client_{tcp_client} {

    }

    void DoOnReceive();
    void AddAction(const std::string& action, action_function action_func);
    void Send(const std::string& message);
    const transfer::TcpCLient& GetTcpClient() const;
    transfer::TcpCLient& GetTcpClient();


private:
    transfer::TcpCLient& tcp_client_;
    std::unordered_map<std::string,action_function> actions_;
};

} //namespace transfer

#endif // MESSAGESHANDLER_H
