#if defined(__linux__)

#ifndef FB_EVNETS_H_
#define FB_EVNETS_H_

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

class FBEvents
{
    public:
        Display*        display;
        unsigned int    keycode;

        FBEvents();
        int FBKeyPress(int key_code);

};


#endif
#endif