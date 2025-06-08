#ifndef MESSAGESHANDLER_H
#define MESSAGESHANDLER_H

// #include <queue>
// #include <list>
#include <functional>
#include <unordered_map>

#include "tcp_client.h"

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


private:
    transfer::TcpCLient& tcp_client_;
    std::unordered_map<std::string,action_function> actions_;
};

#endif // MESSAGESHANDLER_H
