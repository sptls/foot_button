#include "fb_events.h"
#include <cstdlib>

#if defined(__linux__)
#include <unistd.h>

#define FB_SLEEP(x) usleep(x*1000)

FBEvents::FBEvents()
{
    display = XOpenDisplay(NULL);
};

int FBEvents::FBKeyPress(int key_code)
{
    unsigned int keycode = XKeysymToKeycode(display, key_code);
    XTestFakeKeyEvent(display, keycode, True, 0);
    XTestFakeKeyEvent(display, keycode, False, 0);
    XFlush(display);
    return 0;
};

#else

//Windows stuff
#define FB_SLEEP(x) Sleep(x)

FBEvents::FBEvents()
{

};

int FBEvents::FBKeyPress(int key_code)
{
    INPUT inputs[2] = {};
    ZeroMemory(inputs, sizeof(inputs));
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = key_code;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_RETURN;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT result = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

    if(result != ARRAYSIZE(inputs))
    {
        printf("Keypress failed\n");
        return -1;
    }

    return 0;
};


#endif

int FBEvents::RunScript(std::string script)
{
    for(int i = 0; i < script.length(); i++)
    {
        std::string line;
        int k = i;
        for(; script[k] != '\n' && k < script.length(); k++)
        {
            if(script[k] == ' ' && line.length() == 0)
                continue;
            else
                line.push_back(script[k]);
        }
        i = k;

        std::string cmd, arg;
        for(int j = 0; line[j] != ' '; j++)
            cmd.push_back(line[j]);
        for(int j = cmd.length()+1; j < line.length(); j++)
            arg.push_back(line[j]);

        if(ExecCommand(cmd, arg) != 0)
            return 1;
    }
    return 0;
};

int FBEvents::ExecCommand(std::string command, std::string arg)
{
    if(command == "keypress")
    {
        if(fbP.keymap.find(arg) != fbP.keymap.end())
            return FBKeyPress(fbP.keymap[arg]);
        else
        {
            printf("Failed to press %s\n", arg.c_str());
            return -1;
        }
    }
    if(command == "keydown")
    {
        return 1;
    }
    if(command == "keyup")
    {
        return 1;
    }
    if(command == "sleep")
    {
        FB_SLEEP(atoi(arg.c_str()));
    }
    if(command == "exec")
    {
        arg += " &";
        std::system(arg.c_str());
        /*
        CreateProcess()
        or
        execl()
        for Windows
        */
    }
    if(command == "movemouse")
    {
        return 1;
    }

    return 0;
};