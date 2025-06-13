#ifndef APPLICATION_H
#define APPLICATION_H

#include <wx/wx.h>

#include "messages_handler.h"
#include "tcp_client.h"

class Application : public wxApp {
public:
    virtual bool OnInit();

private:
    std::unique_ptr<transfer::TcpCLient> tcp_client;
    std::unique_ptr<transfer::MessagesHandler> message_handler;
};

#endif // APPLICATION_H
