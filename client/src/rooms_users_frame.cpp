#include "rooms_users_frame.h"


namespace  gui {

RoomsUsersFrame::RoomsUsersFrame(wxWindow* parent, const std::vector<std::string>& users)
    : wxFrame(parent, wxID_ANY, "Rooms Users") {
    Bind(wxEVT_CLOSE_WINDOW, &RoomsUsersFrame::OnClose, this);

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    //rooms list
    wxListBox* users_list = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxArrayString{});

    for(const auto& user : users) {
        users_list->Append(user);
    }

    //button
    wxButton* ok_button = new wxButton(panel, wxID_ANY, "Enter");
    ok_button->Bind(wxEVT_BUTTON, &RoomsUsersFrame::OnOkButtonClicked,this);



    main_sizer->Add(users_list,0,wxEXPAND);
    main_sizer->Add(ok_button);

    panel->SetSizer(main_sizer);
}

void RoomsUsersFrame::OnOkButtonClicked(wxCommandEvent& event) {
    Close();
}


}   //namespace gui
