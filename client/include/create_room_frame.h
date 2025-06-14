#ifndef CREATE_ROOM_FRAME_H
#define CREATE_ROOM_FRAME_H

#include <wx/wx.h>

#include "messages_handler.h"
namespace gui {

class CreateRoomFrame : public wxFrame {
public:
    CreateRoomFrame(wxWindow* parent, transfer::MessagesHandler* message_handler);
    void OnEnterButtonClicked(wxCommandEvent& event);

private:
    transfer::MessagesHandler* message_handler_;
    wxTextCtrl* room_name_input_;

    void OnClose(wxCloseEvent& event)
    {
        Destroy();
    }
};

}   //namespace gui

#endif  //CREATE_RROM_FRAME_H
