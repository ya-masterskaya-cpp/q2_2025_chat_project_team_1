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

#ifdef __unix__ || __unix || unix
struct XlibInitializer {
    XlibInitializer() {
        XInitThreads();
    }
} xlib_initializer;
#endif

bool Application::OnInit() {
    gui::MainFrame* frame = new gui::MainFrame("IRC-chat");
    frame->Show();
    return true;
}
