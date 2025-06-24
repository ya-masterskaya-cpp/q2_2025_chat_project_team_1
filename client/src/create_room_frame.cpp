#include "create_room_frame.h"

namespace  gui {


CreateRoomFrame::CreateRoomFrame(wxWindow* parent, domain::MessageHandler* message_handler)
    : wxFrame(parent, wxID_ANY, "Create Room",{},{250,70},wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER),
                message_handler_{message_handler} {
    Bind(wxEVT_CLOSE_WINDOW, &CreateRoomFrame::OnClose, this);

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    //button
    wxButton* enter_button = new wxButton(panel, wxID_ANY, "Enter");
    enter_button->Bind(wxEVT_BUTTON, &CreateRoomFrame::OnEnterButtonClicked,this);

    //input
    room_name_input_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    room_name_input_->SetHint("Write room name");

    main_sizer->Add(room_name_input_,0,wxEXPAND | wxRIGHT,5);
    main_sizer->Add(enter_button,0,wxEXPAND | wxRIGHT,5);
    panel->SetSizer(main_sizer);
}

void CreateRoomFrame::OnEnterButtonClicked(wxCommandEvent& event) {

    std::string room_name = room_name_input_->GetValue().ToStdString();

    if(room_name.empty()) {
        wxMessageBox("Empty room name" , "Error", wxOK | wxICON_ERROR);
        return;
    }

    auto res = message_handler_->CreateRoom(room_name);

    if (!res.error_msg.empty())  {
        wxMessageBox(res.error_msg, "Error", wxOK | wxICON_ERROR);
        return;
    }

    Json::Value parsed_val = domain::Parse(res.msg);


    if(res.status) {
        if(on_update_) {
            on_update_();
        }
        Close();
    } else {
        wxMessageBox(parsed_val["error"].asString(), "Error", wxOK | wxICON_ERROR);
        return;
    }
}

}   //namespace gui
