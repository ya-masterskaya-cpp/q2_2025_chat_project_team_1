#include <wx/wx.h>

#include "application.h"
#include "main_frame.h"


wxIMPLEMENT_APP(Application);

bool Application::OnInit() {
    tcp_client = std::make_unique<transfer::TcpCLient>();
    message_handler = std::make_unique<transfer::MessagesHandler>(*tcp_client);

    gui::MainFrame* frame = new gui::MainFrame("IRC-chat", message_handler.get());
    frame->Show(true);
    return true;
}
