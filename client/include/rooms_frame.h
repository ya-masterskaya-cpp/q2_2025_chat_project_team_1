#ifndef ROOMS_FRAME_H
#define ROOMS_FRAME_H

#include <wx/wx.h>

#include "domain.h"
#include "message_handler.h"

namespace gui {

class RoomsFrame : public wxFrame
{
public:
    RoomsFrame(wxWindow* parent, const wxString& title,
               domain::MessageHandler* message_handler, domain::UserData& user);
    void OnReloadButtonClicked(wxCommandEvent& event);
    void OnCreateRoomButtonClicked(wxCommandEvent& event);
    void OnGetUsersButtonClicked(wxCommandEvent& event);
    void UpdateRoomsList();

private:
    wxListBox* rooms_list_;
    domain::MessageHandler* message_handler_;
    domain::UserData& user_;

    std::vector<std::string> ParseRooms(std::string roomslist);
    void OnClose(wxCloseEvent& event){
        Hide();
    }
};

}   //namespace gui
#endif // ROOMS_FRAME_H
