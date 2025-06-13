#include "main_frame.h"

#include <wx/stdpaths.h>

#include "const.h"
#include "service.h"

namespace gui {

MainFrame::MainFrame(const wxString& title, transfer::MessagesHandler* message_handler)
    : wxFrame(nullptr, wxID_ANY, title), message_handler_{message_handler}, pausable_thread_{} {
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* general_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    //settings
    wxString config_path = wxStandardPaths::Get().GetUserConfigDir() + "/" + "settings.ini";
    std::cout << config_path << std::endl;

    configs_file_ = std::make_unique<wxFileConfig>("IRC-chat", wxEmptyString, config_path,
                                                wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    // chat history
    chat_history_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(400, 300),
                                 wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);

    //input
    message_input_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    // input - enter
    message_input_->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnSendButtonClicked, this);

    //buttons
    wxButton* send_button = new wxButton(panel, wxID_ANY, "Send");
    send_button->Bind(wxEVT_BUTTON, &MainFrame::OnSendButtonClicked, this);
    wxButton* rooms_button = new wxButton(panel, wxID_ANY, "Rooms");
    rooms_button->Bind(wxEVT_BUTTON, &MainFrame::OnRoomButtonClicked, this);

    //layouts
    general_sizer->Add(chat_history_, 1, wxEXPAND | wxALL, 5);
    general_sizer->Add(message_input_, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    general_sizer->Add(buttons_sizer, 0 , wxEXPAND | wxLEFT | wxRIGHT, 5);

    buttons_sizer->Add(rooms_button, 0,  wxLEFT| wxBOTTOM, 5);
    buttons_sizer->AddStretchSpacer(1);
    buttons_sizer->Add(send_button, 0,  wxRIGHT | wxBOTTOM, 5);

    panel->SetSizer(general_sizer);

    //menu
    wxMenu* settings_menu = new wxMenu;
    settings_menu->Append(1001, "Settings");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(settings_menu, "File");

    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &MainFrame::OnSettingsMenu, this, 1001);

    //transfer logic ---------------------------------------------------------------------------
    pausable_thread_.SetTask([self = this] () {
        self->message_handler_->DoOnReceive();
    });

    message_handler_->AddAction(CONSTANTS::ACT_USER_MESSAGE,
                               [self = this](const std::unordered_map<std::string,std::string>& params) {
                                   // self->
    });
    message_handler_->AddAction(CONSTANTS::ACT_SEND_MESSAGE,
                               [self = this](const std::unordered_map<std::string,std::string>& params) {
                                   if(params.at(CONSTANTS::LF_RESULT) == CONSTANTS::RF_ERROR) {
                                       self->chat_history_->AppendText(params.at(CONSTANTS::LF_REASON));
                                   }
                               });
}

void MainFrame::OnSendButtonClicked(wxCommandEvent& event) {
    try {
        message_handler_->Send(UserInterface::US_ChrMakeSendMessage(user_.token, message_input_->GetValue().utf8_string()));
    } catch(...) {

    }
}

void MainFrame::OnRoomButtonClicked(wxCommandEvent& event) {
    if(!rooms_frame_) {
            rooms_frame_ = new RoomsFrame(this, "Select Room", message_handler_, user_);
    }
    rooms_frame_->Show();
}

void MainFrame::OnSettingsMenu(wxCommandEvent& event)
{
//     if(!settings_frame_) {
//         settings_frame_ = new SettingsFrame{this};
//     }
//     settings_frame_->Show();
    SettingsFrame* settings_frame = new SettingsFrame{this, configs_file_.get()};
    settings_frame->Show();
}


}   //namespace gui
