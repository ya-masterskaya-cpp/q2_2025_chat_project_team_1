#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H


#include <wx/wx.h>

namespace gui {


class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
    void OnSendButtonClicked(wxCommandEvent& event);
    void OnRoomButtonClicked(wxCommandEvent& event);

private:
    wxTextCtrl* chat_history_;
    wxTextCtrl* message_input_;
};

} //gui namespace

#endif // MAIN_FRAME_H
