#include "login_frame.h"

namespace gui {

LoginFrame::LoginFrame(wxWindow* parent, domain::MessageHandler* message_handler)
    : wxDialog(parent, wxID_ANY, "Authorization", wxDefaultPosition, wxSize(450, 150)),
    message_handler_{message_handler} {
    Bind(wxEVT_CLOSE_WINDOW, &LoginFrame::OnClose, this);

    wxPanel* panel = new wxPanel(this);

    username_ctrl_ = new wxTextCtrl(panel, wxID_ANY);
    password_ctrl_ = new wxTextCtrl(panel, wxID_ANY,"",wxDefaultPosition,
                                    wxDefaultSize,wxTE_PASSWORD);

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
    auto res = message_handler_->RegisterUser(username_ctrl_->GetValue().ToStdString(),
                                   password_ctrl_->GetValue().ToStdString());

    if (!res.error_msg.empty())  {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_WARNING);
        return;
    }

    std::optional<Json::Value> parsed_val = Parse(res.msg);

    if(res.status) {
        wxMessageBox((*parsed_val)["info"].asString(), "Info", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox((*parsed_val)["error"].asString(), "Warning", wxOK | wxICON_WARNING);
    }
}

void LoginFrame::OnLoginButtonClicked(wxCommandEvent& event) {
    auto res = message_handler_->LoginUser(username_ctrl_->GetValue().ToStdString(),
                                           password_ctrl_->GetValue().ToStdString());

    if (!res.error_msg.empty())  {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_WARNING);
        return;
    }

    if(res.status) {
        std::optional<Json::Value> parsed_val = Parse(res.msg);
        if(parsed_val) {
            wxMessageBox((*parsed_val)["info"].asString(), "Info", wxOK | wxICON_INFORMATION);
            Close();
        } else {
            wxMessageBox("Parse JSON Error", "Error", wxOK | wxICON_WARNING);
        }
    } else {
        std::optional<Json::Value> parsed_val = Parse(res.msg);
        if(parsed_val) {
            wxMessageBox((*parsed_val)["error"].asString(), "Warning", wxOK | wxICON_WARNING);
        } else {
            wxMessageBox("Parse JSON Error", "Error", wxOK | wxICON_WARNING);
        }
    }
}

std::optional<Json::Value> LoginFrame::Parse(const std::string& msg) {
    Json::CharReaderBuilder builder;
    Json::Value parsed_val;
    std::string err;
    std::istringstream iss(msg);
    if (!Json::parseFromStream(builder, iss, &parsed_val,&err)) {
        return std::nullopt;
    }
    return parsed_val;
}

void LoginFrame::OnClose(wxCloseEvent& event) {
    Destroy();
}

}   //namespace gui
