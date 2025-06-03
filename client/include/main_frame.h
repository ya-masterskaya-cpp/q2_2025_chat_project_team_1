#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/frame.h>
#include <wx/wx.h>

namespace gui {


class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    wxTextCtrl* chat_history_;
    wxTextCtrl* message_input_;
    void OnSend(wxCommandEvent& event);
};

} //gui namespace

#endif // MAIN_FRAME_H
