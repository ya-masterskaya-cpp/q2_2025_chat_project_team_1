#ifndef LOGIN_FRAME_H
#define LOGIN_FRAME_H

#include <wx/wx.h>

#include "domain.h"
#include "messages_handler.h"

class LoginFrame : public wxDialog {
public:
    LoginFrame(wxWindow* parent, transfer::MessagesHandler* message_handler,
                    domain::UserData& user);
    void OnSignUpButtonClicked(wxCommandEvent& event);
    void OnLoginButtonClicked(wxCommandEvent& event);

private:
    transfer::MessagesHandler* message_handler_;
    domain::UserData& user_;

    wxTextCtrl* username_ctrl_;
    wxTextCtrl* password_ctrl_;
};


#endif // LOGIN_FRAME_H
