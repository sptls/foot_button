#include "fb_gui.h"

FBGui::FBGui()
{
    builder = Gtk::Builder::create_from_resource("/gui/gui.glade");

    builder->get_widget("windowMain", windowMain);
    builder->get_widget("textViewScript", textViewScript);

    GdkRGBA blackBG;
    blackBG.red = 0.0;
    blackBG.green = 0.0;
    blackBG.blue = 0.0;
    blackBG.alpha = 1.0;

    gtk_widget_override_background_color((GtkWidget*)(textViewScript->gobj()), GTK_STATE_FLAG_NORMAL, &blackBG);
};