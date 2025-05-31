#include <wx/wx.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        wxFrame* frame = new wxFrame(NULL, wxID_ANY, "Hello wxWidgets");
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
