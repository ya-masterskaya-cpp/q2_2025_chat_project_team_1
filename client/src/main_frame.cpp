#include "main_frame.h"

#include <wx/stdpaths.h>
#include <json/json.h>

#include "create_room_frame.h"
#include "login_frame.h"

namespace gui {

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title) {


    wxPanel* main_panel = new wxPanel(this);
    wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxPanel* central_panel = new wxPanel(main_panel);
    wxBoxSizer* central_sizer = new wxBoxSizer(wxVERTICAL);
    wxPanel* info_panel = new wxPanel(main_panel);
    wxBoxSizer* info_sizer = new wxBoxSizer(wxVERTICAL);
    wxPanel* control_panel = new wxPanel(main_panel);
    wxBoxSizer* control_sizer = new wxBoxSizer(wxVERTICAL);

    //CENTRAL PANEL
    // chat history
    chat_history_ = new wxRichTextCtrl(central_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,
                                 wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
    //input
    message_input_ = new wxTextCtrl(central_panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    message_input_->SetHint("Write to Send");
    message_input_->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnSendButtonClicked, this);
    //layouts
    central_sizer->Add(chat_history_, 1, wxEXPAND | wxALL, 5);
    central_sizer->Add(message_input_, 0, wxEXPAND | wxALL, 5);
    central_panel->SetSizer(central_sizer);

    //INFO PANEL
    //label
    info_label_txt_ = new wxStaticText(info_panel, wxID_ANY, "Info Panel");
    wxFont info_label_font = info_label_txt_->GetFont();
    info_label_font.SetWeight(wxFONTWEIGHT_BOLD);
    info_label_txt_->SetFont(info_label_font);
    //list
    info_list_ = new wxListBox(info_panel, wxID_ANY, wxDefaultPosition, wxSize(200, -1), wxArrayString{});
    //layouts
    info_sizer->Add(info_label_txt_,0, wxALL, 5);
    info_sizer->Add(info_list_,1,wxEXPAND | wxALL, 5);
    info_panel->SetSizer(info_sizer);

    //CONTROL PANEL
    //buttons
    wxButton* get_rooms_btn = new wxButton(control_panel, wxID_ANY, "Get rooms");
    get_rooms_btn->Bind(wxEVT_BUTTON, &MainFrame::OnGetRoomsButtonClicked,this);
    wxButton* create_room_btn = new wxButton(control_panel, wxID_ANY, "Create room");
    create_room_btn->Bind(wxEVT_BUTTON, &MainFrame::OnCreateRoomButtonClicked,this);
    wxButton* join_room_btn = new wxButton(control_panel, wxID_ANY, "Join room");
    join_room_btn->Bind(wxEVT_BUTTON, &MainFrame::OnJoinRoomButtonClicked,this);
    wxButton* leave_room_btn = new wxButton(control_panel, wxID_ANY, "Leave room");
    leave_room_btn->Bind(wxEVT_BUTTON, &MainFrame::OnLeaveRoomButtonClicked,this);
    wxButton* get_users_btn = new wxButton(control_panel, wxID_ANY, "Get users");
    get_users_btn->Bind(wxEVT_BUTTON, &MainFrame::OnGetUsersButtonClicked,this);
    wxButton* send_msg_btn = new wxButton(control_panel, wxID_ANY, "Send");
    send_msg_btn->Bind(wxEVT_BUTTON, &MainFrame::OnSendButtonClicked,this);
    //layout
    control_sizer->Add(get_rooms_btn,0,wxALL,5);
    control_sizer->Add(create_room_btn,0,wxALL,5);
    control_sizer->Add(join_room_btn,0,wxALL,5);
    control_sizer->Add(leave_room_btn,0,wxALL,5);
    control_sizer->Add(get_users_btn,0,wxALL,5);
    control_sizer->AddStretchSpacer(1);
    control_sizer->Add(send_msg_btn,0,wxALL,5);
    control_panel->SetSizer(control_sizer);

    //MAIN PANEL
    main_sizer->Add(info_panel,0, wxEXPAND);
    main_sizer->Add(central_panel,1, wxEXPAND);
    main_sizer->Add(control_panel,0, wxEXPAND);
    main_panel->SetSizer(main_sizer);
    //MENU
    wxString config_path = wxStandardPaths::Get().GetUserConfigDir() + "/" + "settings.ini";

    file_configs_ = std::make_unique<wxFileConfig>("IRC-chat", wxEmptyString, config_path,
                                                   wxEmptyString, wxCONFIG_USE_LOCAL_FILE);


    wxMenu* settings_menu = new wxMenu;
    settings_menu->Append(1001, "Settings");
    Bind(wxEVT_MENU, &MainFrame::OnSettingsMenu, this, 1001);

    wxMenu* server_menu = new wxMenu;
    server_menu->Append(1002, "Connect");
    server_menu->Append(1003, "Disconnect");
    Bind(wxEVT_MENU, &MainFrame::OnConnectButtonClicked, this, 1002);
    Bind(wxEVT_MENU, &MainFrame::OnDisconnectButtonClicked, this, 1003);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(settings_menu, "File");
    menuBar->Append(server_menu, "Server");
    SetMenuBar(menuBar);

    //STATUS BAR
    status_bar_ = CreateStatusBar(2);
    int widths[] = {150, 150};
    status_bar_->SetStatusWidths(2, widths);
    status_bar_->Show();
    status_bar_->SetStatusText(std::string("User: ") + std::string("Unknown"),0);
    status_bar_->SetStatusText(std::string("Room: ") + std::string("general"),1);

    SetStatusBar(status_bar_);

    this->SetMinSize({400,400});

    //Load settings
    LoadConfig();
}

void MainFrame::OnGetRoomsButtonClicked(wxCommandEvent& event) {
    if(is_connected_) {
        UpdateRoomsList();
        info_sts_ = info_panel_status::rooms;
    } else {
        chat_history_->AppendText("You are not connected.\n");
    }
}

void MainFrame::UpdateRoomsList() {
    info_list_->Clear();

    auto res = message_handler_->ListRooms();

    if (!res.error_msg.empty())  {
        wxMessageBox(wxString::FromUTF8(res.error_msg), "Error", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value parsed_val = domain::Parse(res.msg);

    if(res.status) {
        info_label_txt_->SetLabel("Info Panel - Rooms");
        for(auto& room : parsed_val) {
            info_list_->Append(wxString::FromUTF8(room.asString()));
        }
    } else {
        wxMessageBox(parsed_val["error"].asString(), "Error", wxOK | wxICON_ERROR);
        return;
    }
}

void MainFrame::OnCreateRoomButtonClicked(wxCommandEvent& event) {
    if(is_connected_) {
        CreateRoomFrame* create_room_frame = new CreateRoomFrame(this,message_handler_.get());
        create_room_frame->SetOnUpdate([self = this]() {
            self->UpdateRoomsList();
        });
        create_room_frame->Show();
    } else {
        chat_history_->AppendText("You are not connected.\n");
    }
}

void MainFrame::OnJoinRoomButtonClicked(wxCommandEvent& event) {
    if(info_sts_ != info_panel_status::rooms) {
        wxMessageBox("No rooms are selected in the info panel", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if(is_connected_) {
        int index = info_list_->GetSelection();
        if(index == -1) {
            wxMessageBox("Choose room", "Error", wxOK | wxICON_ERROR);
            return;
        }
        const std::string& room_name = info_list_->GetString(index).ToStdString();
        auto res = message_handler_->JoinRoom(room_name);

        if(!res.error_msg.empty()) {
            wxMessageBox(wxString::FromUTF8(res.error_msg), "Error", wxOK | wxICON_ERROR);
            return;
        }

        Json::Value parsed_val = domain::Parse(res.msg);

        if(res.status) {
            status_bar_->SetStatusText("Room: " + wxString::FromUTF8(parsed_val["info"].asString()),1);
            chat_history_->AppendText("Room " + wxString::FromUTF8(parsed_val["info"].asString()) +":\n");
        } else {
            wxMessageBox(parsed_val["error"].asString(), "Warning", wxOK | wxICON_WARNING);
            return;
        }

        auto recent_msg_res = message_handler_->GetRoomsRecentMessages(room_name);
        if(!recent_msg_res.error_msg.empty()) {
            wxMessageBox(recent_msg_res.error_msg, "Error", wxOK | wxICON_ERROR);
            return;
        }

        Json::Value parsed_msg_val = domain::Parse(recent_msg_res.msg);

        for(auto& msg : parsed_msg_val) {
            chat_history_->AppendText(wxString::FromUTF8(msg["from"].asString()) + ": " + wxString::FromUTF8(msg["text"].asString()) +"\n");
        }


    } else {
        chat_history_->AppendText("You are not connected.\n");
    }
}

void MainFrame::OnLeaveRoomButtonClicked(wxCommandEvent& event) {
    if(is_connected_) {
        auto res = message_handler_->LeaveRoom();

        if(!res.error_msg.empty()) {
            wxMessageBox(wxString::FromUTF8(res.error_msg), "Error", wxOK | wxICON_ERROR);
            return;
        }

        Json::Value parsed_val = domain::Parse(res.msg);

        if(res.status) {
            status_bar_->SetStatusText(std::string("Room: ") + std::string("general"),1);
            chat_history_->AppendText("Room General:\n");
        } else {
            wxMessageBox(parsed_val["error"].asString(), "Warning", wxOK | wxICON_WARNING);
            return;
        }

        auto recent_msg_res = message_handler_->GetRoomsRecentMessages("general");
        if(!recent_msg_res.error_msg.empty()) {
            wxMessageBox(recent_msg_res.error_msg, "Error", wxOK | wxICON_ERROR);
            return;
        }

        Json::Value parsed_msg_val = domain::Parse(recent_msg_res.msg);

        for(auto& msg : parsed_msg_val) {
            chat_history_->AppendText(wxString::FromUTF8(msg["from"].asString()) + ": " + wxString::FromUTF8(msg["text"].asString()) +"\n");
        }

    } else {
        chat_history_->AppendText("You are not connected.\n");
    }
}

void MainFrame::OnGetUsersButtonClicked(wxCommandEvent& event) {
    if(info_sts_ != info_panel_status::rooms) {
        wxMessageBox("No rooms are selected in the info panel", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if(is_connected_) {
        int index = info_list_->GetSelection();
        if(index == -1) {
            wxMessageBox("Choose room", "Error", wxOK | wxICON_ERROR);
            return;
        }

        auto res = message_handler_->GetUsersInRoom(info_list_->GetString(index).ToStdString());

        if(!res.error_msg.empty()) {
            wxMessageBox(wxString::FromUTF8(res.error_msg), "Error", wxOK | wxICON_ERROR);
            return;
        }

        Json::Value parsed_val = domain::Parse(res.msg);

        if(res.status) {
            info_label_txt_->SetLabel("Info Panel. Rooms users:");
            info_list_->Clear();
            for(const auto& user : parsed_val) {
                info_list_->Append(wxString::FromUTF8(user.asString()));
            }

        } else {
            wxMessageBox(parsed_val["error"].asString(), "Warning", wxOK | wxICON_WARNING);
            return;
        }
        info_sts_ = info_panel_status::users;
    } else {
        chat_history_->AppendText("You are not connected.\n");
    }
}

void MainFrame::OnSendButtonClicked(wxCommandEvent& event) {
    if(is_connected_) {
        const auto msg = std::string(message_input_->GetValue().ToUTF8());
        auto res = message_handler_->SendMessage(msg);
        message_input_->Clear();

        if(!res.error_msg.empty()) {
            chat_history_->AppendText(wxString::FromUTF8(res.error_msg) + '\n');
            return;
        }

        if(!res.status) {
            Json::Value parsed_val = domain::Parse(res.msg);
            wxMessageBox(wxString::FromUTF8(parsed_val["error"].asString()), "MainFrame error", wxOK | wxICON_ERROR);
        }

        auto upload_res = message_handler_->UploadMessageToDB(msg);

    } else {
        chat_history_->AppendText("You are not connected.\n");
    }
}

void MainFrame::OnSettingsMenu(wxCommandEvent& event) {
    SettingsFrame* settings_frame = new SettingsFrame{this, file_configs_.get()};
    settings_frame->Show();
}

void MainFrame::OnConnectButtonClicked(wxCommandEvent& event) {
    if(!is_connected_) {
        file_configs_->SetPath("/Transfer");

        wxString ip;
        file_configs_->Read("IP", &ip, "127.0.0.1");
        int port;
        file_configs_->Read("Port", &port, 8080);

        message_handler_ = std::make_unique<domain::MessageHandler>(user_,ip.ToStdString() +":" + std::to_string(port));

        LoginFrame* login_frame = new LoginFrame(this,message_handler_.get());
        login_frame->ShowModal();

        if(user_.token.empty()) {
            message_handler_.reset();
            return;
        }

        ws_client_ = std::make_unique<transfer::WebSocketClient>(ip.ToStdString(),port,user_.token);
        ws_client_->SetOnOpen([self = this](const std::string& msg) {
            self->status_bar_->SetStatusText(std::string("User: ") + self->user_.name,0);
        });
        ws_client_->SetOnClose([self = this](const std::string& msg) {
            self->status_bar_->SetStatusText(std::string("User: ") + std::string("Unknown"), 0);
            self->status_bar_->SetStatusText(std::string("Room: ") + std::string("None"),1);
        });
        ws_client_->SetOnError([self = this](const std::string& msg) {
            wxMessageBox(msg, "Error", wxOK | wxICON_WARNING);
        });
        ws_client_->SetOnMessage([self = this](const std::string& msg) {
            Json::Value parsed_msg = domain::Parse(msg);
            self->chat_history_->AppendText(wxString::FromUTF8(parsed_msg["from"].asString()) + ": "
                                                               + wxString::FromUTF8(parsed_msg["text"].asString()) + ".\n");
                                              
        });

        ws_client_->Run();
        is_connected_ = true;
    } else {
        chat_history_->AppendText("You are connected already.\n");
    }
}

void MainFrame::OnDisconnectButtonClicked(wxCommandEvent& event) {
    Disconnect();
}

void MainFrame::Disconnect() {
    if(is_connected_) {
        message_handler_->LogoutUser();
        message_handler_.reset();
        ws_client_->Stop();
        is_connected_ = false;
    } else {
        chat_history_->AppendText("You are not connected.\n");
    }
}

void MainFrame::SaveConfig() {
    file_configs_->SetPath("/MainFrame");
    file_configs_->Write("Width", GetSize().GetWidth());
    file_configs_->Write("Height", GetSize().GetHeight());
}

void MainFrame::LoadConfig() {
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
    SaveConfig();
    Disconnect();
}


}   //namespace gui
