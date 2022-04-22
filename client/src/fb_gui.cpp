#include "fb_gui.h"

FBGui::FBGui()
{
    builder = Gtk::Builder::create_from_resource("/gui/gui.glade");

    builder->get_widget("windowMain", windowMain);
};