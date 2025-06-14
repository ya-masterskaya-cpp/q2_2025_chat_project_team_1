#ifndef ROOMS_FRAME_H
#define ROOMS_FRAME_H

#include <wx/wx.h>

#include "domain.h"
#include "login_frame.h"
#include "messages_handler.h"

namespace gui {

class RoomsFrame : public wxFrame
{
public:
    RoomsFrame(wxWindow* parent, const wxString& title,
               transfer::MessagesHandler* message_handler, domain::UserData& user);
    void OnLoginButtonClicked(wxCommandEvent& event);
    void UpdateRoomsList();

private:
    wxListBox* rooms_list_;
    transfer::MessagesHandler* message_handler_;
    domain::UserData& user_;
    LoginFrame* login_frame_{nullptr};

    std::vector<std::string> ParseRooms(std::string roomslist);
};

}   //namespace gui
#endif // ROOMS_FRAME_H
