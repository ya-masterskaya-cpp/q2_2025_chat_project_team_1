#include "settings_frame.h"

#include <wx/notebook.h>
#include <regex>

namespace gui {

SettingsFrame::SettingsFrame(wxWindow* parent, wxFileConfig* file_configs)
    : wxFrame(parent, wxID_ANY, "Settings"), file_configs_{file_configs} {
    Bind(wxEVT_CLOSE_WINDOW, &SettingsFrame::OnClose, this);

    wxBoxSizer* main_sizer = new wxBoxSizer(wxVERTICAL);

    //notebook
    wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

    //1 tab
    wxPanel* transfer_panel = new wxPanel(notebook);
    wxFlexGridSizer* transfer_sizer = new wxFlexGridSizer(2, 2, 5, 5);
    ip_ = new wxTextCtrl(transfer_panel, wxID_ANY);
    port_ = new wxTextCtrl(transfer_panel, wxID_ANY);
    transfer_sizer->Add(new wxStaticText(transfer_panel, wxID_ANY, "IP:"),1);
    transfer_sizer->Add(ip_,3,wxEXPAND,5);
    transfer_sizer->Add(new wxStaticText(transfer_panel, wxID_ANY, "Port:"),1);
    transfer_sizer->Add(port_,3,wxEXPAND,5);
    transfer_panel->SetSizer(transfer_sizer);


    //2 tab
    wxPanel* view_panel = new wxPanel(notebook);

    //
    notebook->AddPage(transfer_panel, "Transfer");
    notebook->AddPage(view_panel, "View");


    //buttons
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxButton* save_button = new wxButton(this, wxID_ANY, "Save");
    wxButton* cancel_button = new wxButton(this, wxID_ANY, "Cancel");
    save_button->Bind(wxEVT_BUTTON, &SettingsFrame::OnSaveButtonClicked,this);
    cancel_button->Bind(wxEVT_BUTTON, &SettingsFrame::OnCancelButtonClicked,this);
    buttons_sizer->Add(save_button, 0,  wxRIGHT, 5);
    buttons_sizer->Add(cancel_button, 0,  wxRIGHT, 5);


    //layout
    main_sizer->Add(notebook, 1, wxEXPAND,5);
    main_sizer->Add(buttons_sizer,0);
    SetSizer(main_sizer);

    Load();
}

void SettingsFrame::OnSaveButtonClicked(wxCommandEvent& event) {
    // std::regex ipRegex(R"(^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.)
    //         {3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])(\/(3[0-2]|[1-2]?\d))?$)");
    // if (std::regex_match(std::string(ip_->GetValue().ToStdString()), ipRegex)) {
    //     ip_->SetBackgroundColour(*wxWHITE);
    // } else {
    //     ip_->SetBackgroundColour(*wxRED);
    // }

    Save();
}

void SettingsFrame::Save() {
    //transfer
    file_configs_->SetPath("/Transfer");
    file_configs_->Write("IP", ip_->GetValue());
    file_configs_->Write("Port", port_->GetValue());
}

void SettingsFrame::Load() {
    //transfer
    file_configs_->SetPath("/Transfer");


    if (file_configs_->HasEntry("IP")) {
        wxString ip;
        file_configs_->Read("IP", &ip, "127.0.0.1");
        ip_->SetValue(ip);
    }

    if (file_configs_->HasEntry("Port")) {
        wxString port;
        file_configs_->Read("Port", &port, "3333");
        port_->SetValue(port);
    }

}

void SettingsFrame::OnCancelButtonClicked(wxCommandEvent& event) {
    Close();
}

}   //namespace gui
