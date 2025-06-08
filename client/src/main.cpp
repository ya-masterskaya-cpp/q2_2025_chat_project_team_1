#include <wx/wx.h>

#include "application.h"
#include "main_frame.h"
#include "messages_handler.h"
#include "tcp_client.h"


wxIMPLEMENT_APP(Application);

bool Application::OnInit() {
    transfer::TcpCLient tcp_client;
    MessagesHandler message_handler{tcp_client};

    gui::MainFrame* frame = new gui::MainFrame("IRC-chat", message_handler);
    frame->Show(true);
    return true;
}
