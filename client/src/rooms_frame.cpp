#include "rooms_frame.h"

#include <cassert>

#include "create_room_frame.h"
// #include "rooms_users_frame.h"

namespace gui {

RoomsFrame::RoomsFrame(wxWindow* parent,const wxString& title,
                       domain::MessageHandler* message_handler, domain::UserData& user)
    : wxFrame(parent, wxID_ANY, title), message_handler_{message_handler}, user_{user} {
    Bind(wxEVT_CLOSE_WINDOW, &RoomsFrame::OnClose, this);

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* main_buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* control_button_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* functional_sizer =  new wxBoxSizer(wxHORIZONTAL);

    //main list
    main_list_ = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxArrayString{});

    //buttons
    wxButton* get_rooms_button = new wxButton(panel, wxID_ANY, "Get rooms");
    get_rooms_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnGetRoomsButtonClicked,this);
    wxButton* create_room_button = new wxButton(panel, wxID_ANY, "Create room");
    create_room_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnCreateRoomButtonClicked,this);
    wxButton* join_room_button = new wxButton(panel, wxID_ANY, "Join room");
    create_room_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnJoinRoomButtonClicked,this);
    wxButton* leave_room_button = new wxButton(panel, wxID_ANY, "Leave room");
    create_room_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnLeaveRoomButtonClicked,this);
    wxButton* get_users_button = new wxButton(panel, wxID_ANY, "Get users");
    get_users_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnGetUsersButtonClicked,this);


    wxButton* ok_button = new wxButton(panel, wxID_ANY, "Ok");
    wxButton* cancel_button = new wxButton(panel, wxID_ANY, "Cancel");


    //layouts
    control_button_sizer->Add(get_rooms_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);
    control_button_sizer->Add(create_room_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);
    control_button_sizer->Add(join_room_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);
    control_button_sizer->Add(leave_room_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);
    control_button_sizer->Add(get_users_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);

    main_buttons_sizer->Add(ok_button, 0,  wxLEFT | wxBOTTOM, 5);
    main_buttons_sizer->Add(cancel_button, 0,  wxLEFT | wxBOTTOM, 5);


    functional_sizer->Add(main_list_,1,wxEXPAND | wxALL, 5);
    functional_sizer->Add(control_button_sizer);
    main_sizer->Add(functional_sizer,1,wxEXPAND);
    main_sizer->Add(main_buttons_sizer);

    panel->SetSizer(main_sizer);

    //-----------------------------------------------------------
    // main_list_->Append("test 1");
    // main_list_->Append("test 2");
    // main_list_->Append("test 3");
    //-----------------------------------------------------------
    UpdateRoomsList();
}

void RoomsFrame::UpdateRoomsList() {
    main_list_->Clear();

    auto res = message_handler_->ListRooms();

    if (!res.error_msg.empty())  {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value parsed_val = domain::Parse(res.msg);

    if(res.status) {
        std::vector<std::string> parsed_rooms = ParseRooms(parsed_val["error"].asString());
        main_list_->Append("Rooms:\n");
        for(auto& room : parsed_rooms) {
            main_list_->Append(room + '\n');
        }
    } else {
        wxMessageBox(parsed_val["error"].asString(), "Error", wxOK | wxICON_ERROR);
        return;
    }
}

std::vector<std::string> RoomsFrame::ParseRooms(std::string roomslist) {
    Json::CharReaderBuilder builder;
    Json::Value parsed_val;
    std::string err;
    std::istringstream iss(roomslist);

    std::vector<std::string> res;

    if (!Json::parseFromStream(builder, iss, &parsed_val,&err)) {
        res.push_back(err);
        return res;
    }

    for(auto& room: parsed_val) {
        res.push_back(room.asString());
    }
    return res;
}

void RoomsFrame::OnGetRoomsButtonClicked(wxCommandEvent& event) {
    UpdateRoomsList();
}

void RoomsFrame::OnCreateRoomButtonClicked(wxCommandEvent& event) {
    CreateRoomFrame* create_room_frame = new CreateRoomFrame(this,message_handler_);
    create_room_frame->SetOnUpdate([self = this]() {
        self->UpdateRoomsList();
    });
    create_room_frame->Show();
}

void RoomsFrame::OnJoinRoomButtonClicked(wxCommandEvent& event) {
    int index = main_list_->GetSelection();
    auto res = message_handler_->JoinRoom(main_list_->GetString(index).ToStdString());

    if(!res.error_msg.empty()) {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value parsed_val = domain::Parse(res.msg);

    if(res.status) {
        if(onjoinroom_callback_) {
            // onjoinroom_callback_(main_list_->GetString(index).ToStdString());
            onjoinroom_callback_(parsed_val["info"].asString());
        }
    } else {
        wxMessageBox(parsed_val["error"].asString(), "Warning", wxOK | wxICON_WARNING);
        return;
    }
}

void RoomsFrame::OnLeaveRoomButtonClicked(wxCommandEvent& event) {
    auto res = message_handler_->LeaveRoom();

    if(!res.error_msg.empty()) {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value parsed_val = domain::Parse(res.msg);

    if(res.status) {
        if(onleaveroom_callback_) {
            onleaveroom_callback_();
        }
    } else {
        wxMessageBox(parsed_val["error"].asString(), "Warning", wxOK | wxICON_WARNING);
        return;
    }
}

void RoomsFrame::OnGetUsersButtonClicked(wxCommandEvent& event){
    int index = main_list_->GetSelection();
    auto res = message_handler_->GetUsersInRoom(main_list_->GetString(index).ToStdString());

    if(!res.error_msg.empty()) {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value parsed_val = domain::Parse(res.msg);

    if(res.status) {
        Json::Value users_array = domain::Parse(parsed_val["info"].asString());

        main_list_->Clear();
        main_list_->Append("Room #" + main_list_->GetString(index) + " users:\n");
        for(const auto& user : users_array) {
            main_list_->Append(user.asString() + '\n');
        }

    } else {
        wxMessageBox(parsed_val["error"].asString(), "Warning", wxOK | wxICON_WARNING);
        return;
    }
}

}   //namespace gui
