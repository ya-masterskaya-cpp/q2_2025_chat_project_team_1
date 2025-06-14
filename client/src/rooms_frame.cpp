#include "rooms_frame.h"

#include <const.h>
#include <service.h>

#include "create_room_frame.h"

namespace gui {

RoomsFrame::RoomsFrame(wxWindow* parent,const wxString& title,
                       transfer::MessagesHandler* message_handler, domain::UserData& user)
    : wxFrame(parent, wxID_ANY, title), message_handler_{message_handler}, user_{user} {


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
    wxButton* login_button = new wxButton(panel, wxID_ANY, "Login");
    login_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnLoginButtonClicked,this);
    wxButton* create_room_button = new wxButton(panel, wxID_ANY, "Create room");
    create_room_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnCreateRoomButtonClicked,this);


    wxButton* ok_button = new wxButton(panel, wxID_ANY, "Ok");
    wxButton* cancel_button = new wxButton(panel, wxID_ANY, "Cancel");


    //layouts
    control_button_sizer->Add(reload_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);
    control_button_sizer->Add(login_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);
    control_button_sizer->Add(create_room_button, 0,  wxLEFT | wxRIGHT | wxTOP, 5);

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

    //transfer logic
    message_handler_->AddAction(CONSTANTS::ACT_ROOM_LIST, [self = this] (const std::unordered_map<std::string,std::string>& params) {
        if(params.at(CONSTANTS::LF_RESULT) == CONSTANTS::RF_ERROR) {
            self->rooms_list_->Clear();
            self->rooms_list_->Append(std::move(params.at(CONSTANTS::LF_REASON)));
        } else {
            self->rooms_list_->Clear();
            std::vector<std::string> rooms = self->ParseRooms(std::move(params.at(CONSTANTS::ACT_ROOM_LIST)));
            for (auto& room : rooms) {
                self->rooms_list_->Append(room + '\n');
            }
        }
    });

    message_handler_->AddAction(CONSTANTS::ACT_CREATE_ROOM, [self = this] (const std::unordered_map<std::string,std::string>& params) {
        if(params.at(CONSTANTS::LF_RESULT) == CONSTANTS::RF_ERROR) {
            wxMessageBox("Create room", params.at(CONSTANTS::LF_REASON), wxOK | wxICON_WARNING);
        } else {
            wxMessageBox("Create room", "Room created", wxOK | wxICON_INFORMATION);
            self->UpdateRoomsList();
        }

    });
    UpdateRoomsList();
}

void RoomsFrame::UpdateRoomsList() {
    try {
        message_handler_->Send(UserInterface::US_SrvMakeObjRoomList());
    } catch(const std::exception& e) {
        wxMessageBox(e.what(), "On Update RoomsList", wxOK | wxICON_WARNING);
        return;
    }
}

std::vector<std::string> RoomsFrame::ParseRooms(std::string roomslist) {
    boost::json::value parsed = boost::json::parse(roomslist);
    boost::json::array parsed_arr = parsed.as_array();
    std::vector<std::string> res;
    res.reserve(parsed_arr.size());

    for(auto& room : parsed_arr) {
        res.push_back(room.as_string().c_str());
    }
    return res;
}

void RoomsFrame::OnLoginButtonClicked(wxCommandEvent& event) {
    if(!login_frame_) {
        login_frame_ = new LoginFrame(this, message_handler_,user_);

    }
    login_frame_->Show();
}

void RoomsFrame::OnReloadButtonClicked(wxCommandEvent& event) {
    UpdateRoomsList();
}

void RoomsFrame::OnCreateRoomButtonClicked(wxCommandEvent& event) {
    CreateRoomFrame* create_room_frame = new CreateRoomFrame(this,message_handler_);
    create_room_frame->Show();
}

}   //namespace gui
