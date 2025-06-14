#include "rooms_frame.h"

#include <const.h>
#include <service.h>

namespace gui {

RoomsFrame::RoomsFrame(wxWindow* parent,const wxString& title,
                       transfer::MessagesHandler* message_handler, domain::UserData& user)
    : wxFrame(parent, wxID_ANY, title), message_handler_{message_handler}, user_{user} {


    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* general_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    //список комнат
    rooms_list_ = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxArrayString{});

    //кнопки
    wxButton* reload_button = new wxButton(panel, wxID_ANY, "Reload");
    wxButton* login_button = new wxButton(panel, wxID_ANY, "Login");
    login_button->Bind(wxEVT_BUTTON, &RoomsFrame::OnLoginButtonClicked,this);
    wxButton* ok_button = new wxButton(panel, wxID_ANY, "Ok");
    wxButton* cancel_button = new wxButton(panel, wxID_ANY, "Cancel");


    //компоновщики
    general_sizer->Add(rooms_list_,1,wxEXPAND | wxALL, 5);
    general_sizer->Add(buttons_sizer, 0 , wxEXPAND | wxLEFT | wxRIGHT, 5);

    buttons_sizer->Add(reload_button, 0,  wxLEFT| wxBOTTOM, 5);
    buttons_sizer->Add(login_button, 0,  wxLEFT| wxBOTTOM, 5);
    buttons_sizer->AddStretchSpacer(1);
    buttons_sizer->Add(ok_button, 0,  wxRIGHT | wxBOTTOM, 5);
    buttons_sizer->Add(cancel_button, 0,  wxRIGHT | wxBOTTOM, 5);

    panel->SetSizer(general_sizer);

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
    UpdateRoomsList();
}

void RoomsFrame::UpdateRoomsList() {
    // try {
        message_handler_->Send(UserInterface::US_SrvMakeObjRoomList());
    // } catch(...) {

    // }
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

}
