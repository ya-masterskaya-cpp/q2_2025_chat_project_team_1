#include "rooms_frame.h"

// #include "create_room_frame.h"
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

    //rooms list
    rooms_list_ = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxArrayString{});

    //buttons
    wxButton* reload_button = new wxButton(panel, wxID_ANY, "Reload");
    reload_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnReloadButtonClicked,this);
    wxButton* create_room_button = new wxButton(panel, wxID_ANY, "Create room");
    create_room_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnCreateRoomButtonClicked,this);
    wxButton* get_users_button = new wxButton(panel, wxID_ANY, "Get users");
    get_users_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnGetUsersButtonClicked,this);


    wxButton* ok_button = new wxButton(panel, wxID_ANY, "Ok");
    wxButton* cancel_button = new wxButton(panel, wxID_ANY, "Cancel");


    //layouts
    control_button_sizer->Add(reload_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);
    control_button_sizer->Add(create_room_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);
    control_button_sizer->Add(get_users_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);

    main_buttons_sizer->Add(ok_button, 0,  wxLEFT | wxBOTTOM, 5);
    main_buttons_sizer->Add(cancel_button, 0,  wxLEFT | wxBOTTOM, 5);


    functional_sizer->Add(rooms_list_,1,wxEXPAND | wxALL, 5);
    functional_sizer->Add(control_button_sizer);
    main_sizer->Add(functional_sizer,1,wxEXPAND);
    main_sizer->Add(main_buttons_sizer);

    panel->SetSizer(main_sizer);

    //-----------------------------------------------------------
    rooms_list_->Append("test 1");
    rooms_list_->Append("test 2");
    rooms_list_->Append("test 3");
    //-----------------------------------------------------------
    UpdateRoomsList();
}

void RoomsFrame::UpdateRoomsList() {
    // try {
    //     message_handler_->Send(UserInterface::US_SrvMakeObjRoomList());
    // } catch(const std::exception& e) {
    //     wxMessageBox(e.what(), "On Update RoomsList", wxOK | wxICON_WARNING);
    //     return;
    // }
    rooms_list_->Clear();

    auto res = message_handler_->ListRooms();

    if (!res.error_msg.empty())  {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_ERROR);
        return;
    }

    std::optional<Json::Value> parsed_val = domain::Parse(res.msg);
    if(!parsed_val) {
        wxMessageBox("Parse JSON Error", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if(res.status) {
        std::vector<std::string> parsed_rooms = ParseRooms((*parsed_val)["error"].asString());
        for(auto& room : parsed_rooms) {
            rooms_list_->Append(room + '\n');
        }
    } else {
        wxMessageBox((*parsed_val)["error"].asString(), "Error", wxOK | wxICON_ERROR);
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

void RoomsFrame::OnReloadButtonClicked(wxCommandEvent& event) {
    UpdateRoomsList();
}

void RoomsFrame::OnCreateRoomButtonClicked(wxCommandEvent& event) {
    // CreateRoomFrame* create_room_frame = new CreateRoomFrame(this,message_handler_);
    // create_room_frame->Show();
}

void RoomsFrame::OnGetUsersButtonClicked(wxCommandEvent& event){
    // int index = rooms_list_->GetSelection();
    // try {
    //     message_handler_->Send(UserInterface::US_SrvMakeObjGetUsers(rooms_list_->GetString(index).ToStdString()));
    // } catch(const std::exception& e) {
    //     wxMessageBox(e.what(), "On Get Users", wxOK | wxICON_WARNING);
    //     return;
    // }
}

}   //namespace gui
