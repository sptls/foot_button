#include "fb_events.h"

#if defined(__linux__)

FBEvents::FBEvents()
{
    display = XOpenDisplay(NULL);
};

int FBEvents::FBKeyPress(int key_code)
{
    keycode = XKeysymToKeycode(display, key_code);
    XTestFakeKeyEvent(display, keycode, True, 0);
    XTestFakeKeyEvent(display, keycode, False, 0);
    XFlush(display);
    return 0;
};

#else

//Windows stuff

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
        for(int j = i; script[j] != '\n'; j++)
            line.push_back(script[j]);
        i += line.length();

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
    int execResult;

    printf("cmd: %s\n", command.c_str());
    printf("arg: %s\n", arg.c_str());

    if(command == "keypress")
    {
        printf("FBKeyPress(%i)", fbP.keymap[arg]);
        return FBKeyPress(fbP.keymap[arg]);
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
        return 1;
    }
    if(command == "exec")
    {
        return 1;
    }
    if(command == "movemouse")
    {
        return 1;
    }

    return 0;
};