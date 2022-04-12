#ifndef FB_EVNETS_H_
#define FB_EVNETS_H_

#if defined(__linux__)
    #include <X11/Xlib.h>
    #include <X11/keysym.h>
    #include <X11/extensions/XTest.h>
#else
    #include <windows.h>
#endif

#include <stdio.h>
#include "fb_parser.h"

class FBEvents
{
    public:
    #if defined(__linux__)
        Display*        display;
    #endif
        FBParser        fbP;
        FBEvents();
        int FBKeyPress(int key_code);
        int RunScript(std::string script);
        int ExecCommand(std::string command, std::string arg);
};

#endif