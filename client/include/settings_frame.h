#ifndef SETTINGS_FRAME_H
#define SETTINGS_FRAME_H


#include <wx/fileconf.h>
#include <wx/wx.h>


namespace gui {

class SettingsFrame : public wxFrame {
public:
    SettingsFrame(wxWindow* parent, wxFileConfig* file_configs);
    void OnSaveButtonClicked(wxCommandEvent& event);
    void OnCancelButtonClicked(wxCommandEvent& event);
    void Save();
    void Load();

private:
    wxFileConfig* file_configs_;
    wxTextCtrl* ip_;
    wxTextCtrl* port_;


    void OnClose(wxCloseEvent& event) {
        Destroy();
    }
};


}   //namespace gui
#endif // SETTINGS_FRAME_H
