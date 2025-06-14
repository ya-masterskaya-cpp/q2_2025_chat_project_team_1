#include "login_frame.h"

namespace gui {

LoginFrame::LoginFrame(wxWindow* parent, transfer::MessagesHandler* message_handler,
                       domain::UserData& user)
    : wxDialog(parent, wxID_ANY, "Authorization", wxDefaultPosition, wxSize(450, 150)),
    message_handler_{message_handler} , user_{user} {

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

    //transfer logic
    // message_handler.AddAction();
}

void LoginFrame::OnSignUpButtonClicked(wxCommandEvent& event) {

}

void LoginFrame::OnLoginButtonClicked(wxCommandEvent& event) {
    wxString username = username_ctrl_->GetValue();
    wxString password = password_ctrl_->GetValue();

    // Пример простой проверки (заглушка)
    if (username == "admin" && password == "1234") {
        wxMessageBox("Добро пожаловать!", "Успех", wxOK | wxICON_INFORMATION);
    } else {
        wxMessageBox("Неверный логин или пароль", "Ошибка", wxOK | wxICON_ERROR);
    }
}

}   //namespace gui
