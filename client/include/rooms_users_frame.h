#ifndef ROOMS_USERS_FRAME_H
#define ROOMS_USERS_FRAME_H

#include <wx/wx.h>
#include <vector>

namespace gui {

class RoomsUsersFrame : public wxFrame {
public:
    RoomsUsersFrame(wxWindow* parent, const std::vector<std::string>& users);
    void OnOkButtonClicked(wxCommandEvent& event);

private:
    void OnClose(wxCloseEvent& event)
    {
        Destroy();
    }
};

}   //namespace gui
#endif // ROOMS_USERS_FRAME_H
