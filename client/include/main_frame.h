#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H


#include <wx/fileconf.h>
#include <wx/wx.h>

#include "domain.h"
#include "message_handler.h"
#include "rooms_frame.h"
#include "settings_frame.h"
#include "websocket_client.h"

namespace gui {

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
    void OnSendButtonClicked(wxCommandEvent& event);
    void OnRoomButtonClicked(wxCommandEvent& event);
    void OnSettingsMenu(wxCommandEvent& event);
    void OnConnectButtonClicked(wxCommandEvent& event);
    void Save();
    void Load();
    ~MainFrame();

private:
    domain::UserData user_;
    std::unique_ptr<domain::MessageHandler> message_handler_;
    std::unique_ptr<wxFileConfig> file_configs_;
    bool connected_{false};
    std::unique_ptr<transfer::WebSocketClient> ws_client_;

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
