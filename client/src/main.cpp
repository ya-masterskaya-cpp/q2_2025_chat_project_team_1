// #include "server_loader.h"
// #include "command_handler.h"
// #include "chat_client.h"
// #include <iostream>

// <<<<<<< HEAD
#include "application.h"
#include "main_frame.h"

#ifdef __unix__ || __unix || unix
    #include <X11/Xlib.h>
#endif


wxIMPLEMENT_APP(Application);

bool Application::OnInit() {
    #ifdef __unix__ || __unix || unix
        XInitThreads();
    #endif
    gui::MainFrame* frame = new gui::MainFrame("IRC-chat");
    frame->Show();
    return true;
}
