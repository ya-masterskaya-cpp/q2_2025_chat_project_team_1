#include "main_frame.h"

#include "const.h"
#include "rooms_frame.h"
#include "service.h"

namespace gui {

MainFrame::MainFrame(const wxString& title, MessagesHandler& message_handler)
    : wxFrame(nullptr, wxID_ANY, title), message_handler_{message_handler} {
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* general_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    // история чата (read-only)
    chat_history_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(400, 300),
                                 wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);

    // поле ввода
    message_input_ = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);

    // поле ввода - событие нажатия Enter
    message_input_->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnSendButtonClicked, this);

    //кнопки
    wxButton* send_button = new wxButton(panel, wxID_ANY, "Send");
    send_button->Bind(wxEVT_BUTTON, &MainFrame::OnSendButtonClicked, this);
    wxButton* rooms_button = new wxButton(panel, wxID_ANY, "Rooms");
    rooms_button->Bind(wxEVT_BUTTON, &MainFrame::OnRoomButtonClicked, this);

    //компоновщики
    general_sizer->Add(chat_history_, 1, wxEXPAND | wxALL, 5);
    general_sizer->Add(message_input_, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 5);
    general_sizer->Add(buttons_sizer, 0 , wxEXPAND | wxLEFT | wxRIGHT, 5);

    buttons_sizer->Add(rooms_button, 0,  wxLEFT| wxBOTTOM, 5);
    buttons_sizer->AddStretchSpacer(1);
    buttons_sizer->Add(send_button, 0,  wxRIGHT | wxBOTTOM, 5);

    panel->SetSizer(general_sizer);

    message_handler_.AddAction(CONSTANTS::ACT_USER_MESSAGE,
                               [self = this](const std::unordered_map<std::string,std::string>& params) {
                                   // self->
    });
    message_handler_.AddAction(CONSTANTS::ACT_SEND_MESSAGE,
                               [self = this](const std::unordered_map<std::string,std::string>& params) {
                                   if(params.at(CONSTANTS::LF_RESULT) == CONSTANTS::RF_ERROR) {
                                       self->chat_history_->AppendText(params.at(CONSTANTS::LF_REASON));
                                   }
                               });
}

void MainFrame::OnSendButtonClicked(wxCommandEvent& event) {
    // wxString message = message_input_->GetValue();
    // if (!message.empty()) {
    //     chat_history_->AppendText("You: " + message + "\n");
    //     message_input_->Clear();
    // }

    message_handler_.Send(UserInterface::US_ChrMakeSendMessage(token_, message_input_->GetValue().utf8_string()));
}

void MainFrame::OnRoomButtonClicked(wxCommandEvent& event) {
    RoomsFrame* rooms_frame = new RoomsFrame("Select Room");
    rooms_frame->Show();
}


}   //namespace gui
