#include <wx/wx.h>

#include "application.h"
#include "main_frame.h"


wxIMPLEMENT_APP(Application);

bool Application::OnInit() {
    gui::MainFrame* frame = new gui::MainFrame("Hello wxWidgets");
    frame->Show(true);
    return true;
}
