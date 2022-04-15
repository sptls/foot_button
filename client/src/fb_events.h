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
#include <string>
#include <vector>
#include "fb_parser.h"

enum FB_INPUT_TYPE
{
    FB_TAP,
    FB_PRESS,
    FB_RELEASE
};

enum FB_INPUT_SOURCE
{
    FB_INPUT_MOUSE,
    FB_INPUT_KB
};

class FBEvents
{
    public:
    #if defined(__linux__)
        Display*        display;
    #endif
        FBParser        fbP;
        FBEvents();
        int             typeDelay = 0;
        int FBKeyPress(int key_code, int input_type, int input_source);
        int RunScript(std::string script);
        int ExecCommand(std::string command, std::string arg);
};

#endif