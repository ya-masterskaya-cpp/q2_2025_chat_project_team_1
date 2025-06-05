#include "rooms_frame.h"

namespace gui {

RoomsFrame::RoomsFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title) {

    Bind(wxEVT_CLOSE_WINDOW, &RoomsFrame::OnClose, this);


    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* general_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    //список комнат
    rooms_list_ = new wxListBox(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize,
                                wxArrayString{});

    //кнопки
    wxButton* reload_button = new wxButton(panel, wxID_ANY, "Reload");
    wxButton* ok_button = new wxButton(panel, wxID_ANY, "Ok");
    wxButton* cancel_button = new wxButton(panel, wxID_ANY, "Cancel");


    //компоновщики
    general_sizer->Add(rooms_list_,1,wxEXPAND | wxALL, 5);
    general_sizer->Add(buttons_sizer, 0 , wxEXPAND | wxLEFT | wxRIGHT, 5);

    buttons_sizer->Add(reload_button, 0,  wxLEFT| wxBOTTOM, 5);
    buttons_sizer->AddStretchSpacer(1);
    buttons_sizer->Add(ok_button, 0,  wxRIGHT | wxBOTTOM, 5);
    buttons_sizer->Add(cancel_button, 0,  wxRIGHT | wxBOTTOM, 5);

    panel->SetSizer(general_sizer);
}

}
