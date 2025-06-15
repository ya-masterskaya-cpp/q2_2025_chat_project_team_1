#ifndef LOGIN_FRAME_H
#define LOGIN_FRAME_H

#include <wx/wx.h>

#include "messages_handler.h"

namespace gui {

class LoginFrame : public wxDialog {
public:
    LoginFrame(wxWindow* parent, transfer::MessagesHandler* message_handler,
               const std::string& room_name);
    void OnSignUpButtonClicked(wxCommandEvent& event);
    void OnLoginButtonClicked(wxCommandEvent& event);

private:
    transfer::MessagesHandler* message_handler_;
    const std::string room_name_;

    wxTextCtrl* username_ctrl_;
    wxTextCtrl* password_ctrl_;
};

}   //namespace gui
#endif // LOGIN_FRAME_H
