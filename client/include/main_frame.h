#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H


#include <wx/wx.h>

#include "messages_handler.h"

namespace gui {

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, MessagesHandler& message_handler);
    void OnSendButtonClicked(wxCommandEvent& event);
    void OnRoomButtonClicked(wxCommandEvent& event);

private:
    MessagesHandler& message_handler_;
    std::string token_{};

    wxTextCtrl* chat_history_;
    wxTextCtrl* message_input_;
};

} //gui namespace

#endif // MAIN_FRAME_H
