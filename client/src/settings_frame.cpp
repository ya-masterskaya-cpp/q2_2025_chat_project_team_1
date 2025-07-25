#include "settings_frame.h"

#include <wx/notebook.h>

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
    ip_->SetMinSize({150,-1});
    port_ = new wxTextCtrl(transfer_panel, wxID_ANY);
    port_->SetMinSize({150,-1});
    transfer_sizer->Add(new wxStaticText(transfer_panel, wxID_ANY, "IP:"),0, wxALL, 5);
    transfer_sizer->Add(ip_,0,wxEXPAND | wxALL, 5);
    transfer_sizer->Add(new wxStaticText(transfer_panel, wxID_ANY, "Port:"),0, wxALL, 5);
    transfer_sizer->Add(port_,0,wxEXPAND | wxALL, 5);
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
    buttons_sizer->AddStretchSpacer(1);
    buttons_sizer->Add(save_button, 0,  wxALL, 5);
    buttons_sizer->Add(cancel_button, 0,  wxALL, 5);


    //layout
    main_sizer->Add(notebook, 1, wxEXPAND,5);
    main_sizer->Add(buttons_sizer,0,wxEXPAND,5);
    SetSizer(main_sizer);
    main_sizer->SetSizeHints(this);

    Layout();
    Centre();


    //settings load
    Load();
}

void SettingsFrame::OnSaveButtonClicked(wxCommandEvent& event) {
    Save();
    Close();
}

void SettingsFrame::Save() {
    //transfer
    file_configs_->SetPath("/Transfer");
    file_configs_->Write("IP", ip_->GetValue());
    file_configs_->Write("Port", port_->GetValue());


    //to save on disk
    file_configs_->Flush();
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
