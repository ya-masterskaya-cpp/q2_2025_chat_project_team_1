#ifndef CREATE_ROOM_FRAME_H
#define CREATE_ROOM_FRAME_H

#include <wx/wx.h>

#include "message_handler.h"

namespace gui {

using ObserveFunc = std::function<void()>;

class CreateRoomFrame : public wxFrame {
public:
    CreateRoomFrame(wxWindow* parent, domain::MessageHandler* message_handler);
    void OnEnterButtonClicked(wxCommandEvent& event);
    void SetOnUpdate(ObserveFunc on_update);

private:
    domain::MessageHandler* message_handler_;
    wxTextCtrl* room_name_input_;
    ObserveFunc on_update_;

    void OnClose(wxCloseEvent& event);
};

}   //namespace gui

#endif  //CREATE_RROM_FRAME_H
