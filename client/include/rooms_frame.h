#ifndef ROOMS_FRAME_H
#define ROOMS_FRAME_H

#include <wx/wx.h>

#include "domain.h"
#include "message_handler.h"

namespace gui {

using VoidFuncType = std::function<void()>;

class RoomsFrame : public wxFrame
{
public:
    RoomsFrame(wxWindow* parent, const wxString& title,
               domain::MessageHandler* message_handler, domain::UserData& user);
    void OnReloadButtonClicked(wxCommandEvent& event);
    void OnCreateRoomButtonClicked(wxCommandEvent& event);
    void OnJoinRoomButtonClicked(wxCommandEvent& event);
    void OnLeaveRoomButtonClicked(wxCommandEvent& event);
    void OnGetUsersButtonClicked(wxCommandEvent& event);
    void UpdateRoomsList();

private:
    wxListBox* main_list_;
    domain::MessageHandler* message_handler_;
    domain::UserData& user_;
    VoidFuncType onjoinroom_callback_;
    VoidFuncType onleaveroom_callback_;

    std::vector<std::string> ParseRooms(std::string roomslist);
    void OnClose(wxCloseEvent& event){
        Hide();
    }

    void OnJoinRoomUpdate(VoidFuncType callback) {
        onjoinroom_callback_ = callback;
    }

    void OnLeaveRoomUpdate(VoidFuncType callback) {
        onleaveroom_callback_ = callback;
    }
};

}   //namespace gui
#endif // ROOMS_FRAME_H
