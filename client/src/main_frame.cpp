#include "main_frame.h"

#include <wx/stdpaths.h>
#include <json/json.h>

#include "login_frame.h"

namespace gui {

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* general_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    //settings
    wxString config_path = wxStandardPaths::Get().GetUserConfigDir() + "/" + "settings.ini";
    std::cout << config_path << std::endl;

    file_configs_ = std::make_unique<wxFileConfig>("IRC-chat", wxEmptyString, config_path,
                                                wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    // chat history
    chat_history_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(400, 300),
                                 wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);

    //input
    message_input_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    message_input_->SetHint("Write to Send");
    message_input_->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnSendButtonClicked, this);

    //buttons
    wxButton* send_button = new wxButton(panel, wxID_ANY, "Send");
    send_button->Bind(wxEVT_BUTTON, &MainFrame::OnSendButtonClicked, this);
    rooms_button_ = new wxButton(panel, wxID_ANY, "Rooms");
    // rooms_button_->Enable(false);
    rooms_button_->Bind(wxEVT_BUTTON, &MainFrame::OnRoomButtonClicked, this);
    conection_button_ = new wxButton(panel, wxID_ANY, "Connect");
    conection_button_->Bind(wxEVT_BUTTON, &MainFrame::OnConnectButtonClicked, this);

    //layouts
    general_sizer->Add(chat_history_, 1, wxEXPAND | wxALL, 5);
    general_sizer->Add(message_input_, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    general_sizer->Add(buttons_sizer, 0 , wxEXPAND | wxLEFT | wxRIGHT, 5);

    buttons_sizer->Add(rooms_button_, 0,  wxLEFT| wxBOTTOM, 5);
    buttons_sizer->Add(conection_button_, 0,  wxLEFT| wxBOTTOM, 5);
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

    //TO DELETE, FOR TESTING
    message_handler_ = std::make_unique<domain::MessageHandler>(user_,"127.0.0.1:3333");
    //Load settings
    Load();
}

void MainFrame::OnSendButtonClicked(wxCommandEvent& event) {
    if(!connected_) {
        chat_history_->AppendText("Server not connected\n");
        message_input_->Clear();
        return;
    }

    auto res = message_handler_->SendMessage(message_input_->GetValue().utf8_string());
    message_input_->Clear();

    if(!res.error_msg.empty()) {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_WARNING);
    }

    if(!res.status) {
        Json::CharReaderBuilder builder;
        Json::Value parsed_val;
        std::string err;
        std::istringstream iss(res.msg);
        if (Json::parseFromStream(builder, iss, &parsed_val,&err)) {
            wxMessageBox(parsed_val["error"].asString(), "MainFrame error", wxOK | wxICON_ERROR);
        } else {
            wxMessageBox("Parse JSON response failed", "MainFrame Error", wxOK | wxICON_ERROR);
        }
    }
}

void MainFrame::OnRoomButtonClicked(wxCommandEvent& event) {
    if(!rooms_frame_) {
            rooms_frame_ = new RoomsFrame(this, "Select Room", message_handler_.get(), user_);
    }
    rooms_frame_->Show();
}

void MainFrame::OnSettingsMenu(wxCommandEvent& event)
{
    // SettingsFrame* settings_frame = new SettingsFrame{this, file_configs_.get()};
    // settings_frame->Show();
}

void MainFrame::Save() {
    file_configs_->SetPath("/MainFrame");
    file_configs_->Write("Width", GetSize().GetWidth());
    file_configs_->Write("Height", GetSize().GetHeight());
}

void MainFrame::OnConnectButtonClicked(wxCommandEvent& event) {
    if(!connected_) {
        file_configs_->SetPath("/Transfer");

        wxString ip;
        file_configs_->Read("IP", &ip, "127.0.0.1");
        int port;
        file_configs_->Read("Port", &port, 3333);

        message_handler_ = std::make_unique<domain::MessageHandler>(user_,ip.ToStdString() +":" + std::to_string(port));

        LoginFrame* login_frame = new LoginFrame(this,message_handler_.get());
        login_frame->ShowModal();

        if(user_.token.empty()) {
            message_handler_.reset();
            return;
        }

        ws_client_ = std::make_unique<transfer::WebSocketClient>(ip.ToStdString(),port,user_.token);
        ws_client_->SetOnOpen([self = this](const std::string& msg) {
            self->conection_button_->SetLabel("Disconnect");
            self->rooms_button_->Enable(true);
            self->connected_ = true;
        });
        ws_client_->SetOnClose([self = this](const std::string& msg) {
            self->conection_button_->SetLabel("Connect");
            self->rooms_button_->Enable(false);
            self->connected_ = false;
        });
        ws_client_->SetOnError([self = this](const std::string& msg) {
            wxMessageBox(msg, "Error", wxOK | wxICON_WARNING);
        });
        ws_client_->SetOnMessage([self = this](const std::string& msg) {
            self->chat_history_->AppendText(msg);
        });


        ws_client_->Run();
    } else {
        message_handler_->LogoutUser();
        message_handler_.reset();
        ws_client_->Stop();
        rooms_frame_->Close();

    }

}

void MainFrame::Load() {
    file_configs_->SetPath("/MainFrame");

    int width = 300;
    int height = 300;

    if (file_configs_->HasEntry("Width")) {
        file_configs_->Read("Width", &width);
    }

    if (file_configs_->HasEntry("Height")) {
        file_configs_->Read("Height", &height);
    }

    SetSize(width,height);
}


MainFrame::~MainFrame() {
    Save();
}


}   //namespace gui
