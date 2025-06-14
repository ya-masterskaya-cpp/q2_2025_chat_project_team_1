#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H


#include <wx/fileconf.h>
#include <wx/wx.h>

#include "domain.h"
#include "messages_handler.h"
#include "pausable_thread.h"
#include "rooms_frame.h"
#include "settings_frame.h"

namespace gui {

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, transfer::MessagesHandler* message_handler);
    void OnSendButtonClicked(wxCommandEvent& event);
    void OnRoomButtonClicked(wxCommandEvent& event);
    void OnSettingsMenu(wxCommandEvent& event);
    void OnConnectButtonClicked(wxCommandEvent& event);
    void Save();
    void Load();
    ~MainFrame();

private:
    transfer::MessagesHandler* message_handler_;
    domain::UserData user_;
    domain::PausableThread pausable_thread_;
    std::unique_ptr<wxFileConfig> file_configs_;
    bool connected_{false};
    //gui
    wxTextCtrl* chat_history_;
    wxTextCtrl* message_input_;
    wxButton* rooms_button_{nullptr};
    wxButton* conection_button_{nullptr};
    RoomsFrame* rooms_frame_{nullptr};
    SettingsFrame* settings_frame_{nullptr};
};

} //gui namespace

#endif // MAIN_FRAME_H
