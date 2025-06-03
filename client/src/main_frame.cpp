#include "main_frame.h"

#include <wx/wx.h>

namespace gui {

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title) {
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // История чата (read-only)
    chat_history_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(400, 300),
                                 wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);

    // Поле ввода
    message_input_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    wxButton* sendButton = new wxButton(panel, wxID_ANY, "Send");

    sizer->Add(chat_history_, 1, wxEXPAND | wxALL, 5);
    sizer->Add(message_input_, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    sizer->Add(sendButton, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 5);

    panel->SetSizer(sizer);
}


void MainFrame::OnSend(wxCommandEvent& event) {
    wxString message = message_input_->GetValue();
    if (!message.empty()) {
        chat_history_->AppendText("You: " + message + "\n");
        message_input_->Clear();
    }
}
}   //namespace gui
