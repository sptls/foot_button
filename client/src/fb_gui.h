#ifndef FB_GUI_H_
#define FB_GUI_H_

#include <gtkmm.h>

class FBGui
{
    public:
        Glib::RefPtr<Gtk::Builder>      builder;
        Gtk::Window                     *windowMain;

        FBGui();
};

#endif