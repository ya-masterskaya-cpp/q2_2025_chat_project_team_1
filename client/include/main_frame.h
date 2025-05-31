#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/frame.h>

namespace gui {

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);
};

} //gui namespace

#endif // MAIN_FRAME_H
