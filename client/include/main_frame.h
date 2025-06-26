#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H


#include <wx/fileconf.h>
#include <wx/wx.h>

#include "domain.h"
#include "message_handler.h"
// #include "rooms_frame.h"
#include "settings_frame.h"
#include "websocket_client.h"

namespace gui {

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
    void OnSendButtonClicked(wxCommandEvent& event);
    // void OnRoomButtonClicked(wxCommandEvent& event);
    void OnGetRoomsButtonClicked(wxCommandEvent& event);
    void OnCreateRoomButtonClicked(wxCommandEvent& event);
    void OnJoinRoomButtonClicked(wxCommandEvent& event);
    void OnLeaveRoomButtonClicked(wxCommandEvent& event);
    void OnGetUsersButtonClicked(wxCommandEvent& event);
    void OnSettingsMenu(wxCommandEvent& event);
    void OnConnectButtonClicked(wxCommandEvent& event);
    void OnDisconnectButtonClicked(wxCommandEvent& event);
    void Disconnect();
    void Save();
    void Load();
    ~MainFrame();

private:
    domain::UserData user_;
    std::unique_ptr<domain::MessageHandler> message_handler_;
    std::unique_ptr<wxFileConfig> file_configs_;
    std::unique_ptr<transfer::WebSocketClient> ws_client_;
    bool is_connected_{false};

    //gui
    wxTextCtrl* chat_history_;
    wxTextCtrl* message_input_;
    wxStatusBar* status_bar_;
    wxListBox* info_list_;
    SettingsFrame* settings_frame_;

    //methods
    void UpdateRoomsList();
};

} //gui namespace

#endif // MAIN_FRAME_H
