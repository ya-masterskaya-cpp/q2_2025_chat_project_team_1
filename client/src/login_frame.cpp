#include "login_frame.h"

namespace gui {

LoginFrame::LoginFrame(wxWindow* parent, domain::MessageHandler* message_handler)
    : wxDialog(parent, wxID_ANY, "Authorization", wxDefaultPosition),
    message_handler_{message_handler} {
    Bind(wxEVT_CLOSE_WINDOW, &LoginFrame::OnClose, this);

    wxPanel* panel = new wxPanel(this);

    username_ctrl_ = new wxTextCtrl(panel, wxID_ANY,"", wxDefaultPosition,
                                    wxDefaultSize,wxTE_PROCESS_ENTER);
    username_ctrl_->SetMinSize(wxSize(150, -1));
    username_ctrl_->Bind(wxEVT_TEXT_ENTER, &LoginFrame::OnUserNameEnter, this);
    password_ctrl_ = new wxTextCtrl(panel, wxID_ANY,"",wxDefaultPosition,
                                    wxDefaultSize,wxTE_PASSWORD);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* btn_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* login_sizer = new wxFlexGridSizer(2, 2, 5, 5);

    login_sizer->Add(new wxStaticText(panel, wxID_ANY, "Login:"),0,wxALL,2);
    login_sizer->Add(username_ctrl_,0,wxEXPAND | wxALL,2);
    login_sizer->Add(new wxStaticText(panel, wxID_ANY, "Password:"),0,wxALL,2);
    login_sizer->Add(password_ctrl_,0,wxEXPAND | wxALL,2);
    login_sizer->AddGrowableCol(1);


    //buttons
    wxButton* sign_up_button = new wxButton(panel, wxID_ANY, "Sign Up");
    wxButton* login_button = new wxButton(panel, wxID_ANY, "Login");
    sign_up_button->Bind(wxEVT_BUTTON, &LoginFrame::OnSignUpButtonClicked,this);
    login_button->Bind(wxEVT_BUTTON, &LoginFrame::OnLoginButtonClicked,this);

    btn_sizer->AddStretchSpacer(1);
    btn_sizer->Add(sign_up_button,0,wxALL,5);
    btn_sizer->Add(login_button,0,wxALL,5);

    main_sizer->Add(login_sizer,0,wxEXPAND | wxALL,5);
    main_sizer->Add(btn_sizer,0,wxEXPAND | wxALL,5);

    panel->SetSizer(main_sizer);
    main_sizer->SetSizeHints(this);

    Layout();
    Centre();
}

void LoginFrame::OnUserNameEnter(wxCommandEvent& event) {
    password_ctrl_->SetFocus();
}

void LoginFrame::OnSignUpButtonClicked(wxCommandEvent& event) {
    const auto user_name = std::string(username_ctrl_->GetValue().ToUTF8());
    const auto password = std::string(password_ctrl_->GetValue().ToUTF8());

    if(user_name.empty() || password.empty()) {
        wxMessageBox("Empty user_name or password", "Warning", wxOK | wxICON_WARNING);
        return;
    }

    auto res = message_handler_->RegisterUser(user_name, password);

    if (!res.error_msg.empty())  {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_WARNING);
        return;
    }

    Json::Value parsed_val = domain::Parse(res.msg);

    if(res.status) {
        wxMessageBox(wxString::FromUTF8(parsed_val["info"].asString()), "Info", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox(wxString::FromUTF8(parsed_val["error"].asString()), "Error", wxOK | wxICON_WARNING);
    }
}

void LoginFrame::OnLoginButtonClicked(wxCommandEvent& event) {
    const auto user_name = std::string(username_ctrl_->GetValue().ToUTF8());
    const auto password = std::string(password_ctrl_->GetValue().ToUTF8());

    if(user_name.empty() || password.empty()) {
        wxMessageBox("Empty user_name or password", "Warning", wxOK | wxICON_WARNING);
        return;
    }
    auto res = message_handler_->LoginUser(user_name, password);

    if (!res.error_msg.empty())  {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_WARNING);
        return;
    }

    Json::Value parsed_val = domain::Parse(res.msg);

    if(res.status) {
        wxMessageBox("Welcome, " + wxString::FromUTF8(parsed_val["user"].asString()),
                     "Info", wxOK | wxICON_INFORMATION);
        Close();
    } else {
        wxMessageBox(wxString::FromUTF8(parsed_val["error"].asString()), "Error", wxOK | wxICON_ERROR);
        return;
    }
}

void LoginFrame::OnClose(wxCloseEvent& event) {
    Destroy();
}

}   //namespace gui
