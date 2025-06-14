#include "create_room_frame.h"

#include <service.h>

namespace  gui {


CreateRoomFrame::CreateRoomFrame(wxWindow* parent, transfer::MessagesHandler* message_handler)
    : wxFrame(parent, wxID_ANY, "Create Room",{},{250,70},wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER),
                message_handler_{message_handler} {
    Bind(wxEVT_CLOSE_WINDOW, &CreateRoomFrame::OnClose, this);
    // ShowModal();

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
    std::string value = room_name_input_->GetValue().ToStdString();
    if(!value.empty()) {
        try {
            message_handler_->Send(UserInterface::US_SrvMakeObjCreateRoom(value));
        } catch(const std::exception& e) {
            wxMessageBox(e.what(), "On Room name send", wxOK | wxICON_WARNING);
        }
        Close();
    }
}

}   //namespace gui
