#ifndef LOGIN_FRAME_H
#define LOGIN_FRAME_H

#include <wx/wx.h>

#include "message_handler.h"

namespace gui {

class LoginFrame : public wxDialog {
public:
    LoginFrame(wxWindow* parent, domain::MessageHandler* message_handler);
    void OnUserNameEnter(wxCommandEvent& event);
    void OnSignUpButtonClicked(wxCommandEvent& event);
    void OnLoginButtonClicked(wxCommandEvent& event);

private:
    domain::MessageHandler* message_handler_;
    bool is_login_;

    wxTextCtrl* username_ctrl_;
    wxTextCtrl* password_ctrl_;

    void OnClose(wxCloseEvent& event);
};

}   //namespace gui
#endif // LOGIN_FRAME_H
