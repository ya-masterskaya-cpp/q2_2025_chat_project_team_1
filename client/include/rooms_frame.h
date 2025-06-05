#ifndef ROOMS_FRAME_H
#define ROOMS_FRAME_H

#include <wx/wx.h>

namespace gui {

class RoomsFrame : public wxFrame
{
public:
    RoomsFrame(const wxString& title);

private:
    wxListBox* rooms_list_;
    void OnClose(wxCloseEvent& event)
    {
        // Освобождаем ресурсы (если нужно) и уничтожаем окно
        Destroy();
    }

};

}   //namespace gui
#endif // ROOMS_FRAME_H
