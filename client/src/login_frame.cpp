#include "login_frame.h"

#include <service.h>
#include <password_hasher.h>

namespace gui {

LoginFrame::LoginFrame(wxWindow* parent, transfer::MessagesHandler* message_handler,
                       const std::string& room_name)
    : wxDialog(parent, wxID_ANY, "Authorization", wxDefaultPosition, wxSize(450, 150)),
    message_handler_{message_handler}, room_name_{room_name} {

    wxPanel* panel = new wxPanel(this);

    username_ctrl_ = new wxTextCtrl(panel, wxID_ANY);
    password_ctrl_ = new wxTextCtrl(panel, wxID_ANY);

    wxFlexGridSizer* main_sizer = new wxFlexGridSizer(3, 2, 5, 5);
    main_sizer->Add(new wxStaticText(panel, wxID_ANY, "Login:"),1);
    main_sizer->Add(username_ctrl_,3,wxEXPAND,5);
    main_sizer->Add(new wxStaticText(panel, wxID_ANY, "Password:"),1);
    main_sizer->Add(password_ctrl_,3,wxEXPAND,5);

    //кнопки
    wxButton* sign_up_button = new wxButton(panel, wxID_ANY, "Sign Up");
    wxButton* login_button = new wxButton(panel, wxID_ANY, "Login");
    sign_up_button->Bind(wxEVT_BUTTON, &LoginFrame::OnSignUpButtonClicked,this);
    login_button->Bind(wxEVT_BUTTON, &LoginFrame::OnLoginButtonClicked,this);

    main_sizer->Add(sign_up_button);
    main_sizer->Add(login_button);
    main_sizer->AddGrowableCol(0, 1);
    main_sizer->AddGrowableCol(1, 2);


    panel->SetSizer(main_sizer);

}

void LoginFrame::OnSignUpButtonClicked(wxCommandEvent& event) {
    try {
        message_handler_->Send(UserInterface::US_SrvMakeObjCreateUser(
            username_ctrl_->GetValue().ToStdString(),
                PasswordHasher::HashPassword(password_ctrl_->GetValue().ToStdString())));
    } catch(const std::exception& e) {
        wxMessageBox(e.what(), "On Sign up", wxOK | wxICON_WARNING);
        return;
    }
}

void LoginFrame::OnLoginButtonClicked(wxCommandEvent& event) {
    try {
        message_handler_->Send(UserInterface::US_SrvMakeObjLogin(
            username_ctrl_->GetValue().ToStdString(),
            PasswordHasher::HashPassword(password_ctrl_->GetValue().ToStdString()),room_name_));
    } catch(const std::exception& e) {
        wxMessageBox(e.what(), "On Login", wxOK | wxICON_WARNING);
        return;
    }
}

}   //namespace gui
