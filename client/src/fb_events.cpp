#include "fb_events.h"
#include <cstdlib>

#if defined(__linux__)
#include <unistd.h>

#define FB_SLEEP(x) usleep(x*1000)

FBEvents::FBEvents()
{
    display = XOpenDisplay(NULL);
};

int FBEvents::FBKeyPress(int key_code, int input_type, int input_source)
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

int FBEvents::FBKeyPress(int key_code, int input_type, int input_source)
{
    //key_code lest than 4 is mostly mouse related
    //acutyall not right!!!
    if(input_source == FB_INPUT_KB)
    {
        UINT result;
        INPUT input;
        switch(input_type)
        {
            case FB_PRESS:
                ZeroMemory(&input, sizeof(input));
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = key_code;
                result = SendInput(1, &input, sizeof(INPUT));
                break;
            case FB_RELEASE:
                ZeroMemory(&input, sizeof(input));
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = key_code;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                result = SendInput(1, &input, sizeof(INPUT));
                break;
            case FB_TAP:
                INPUT inputs[2] = {};
                ZeroMemory(inputs, sizeof(inputs));
                inputs[0].type = INPUT_KEYBOARD;
                inputs[0].ki.wVk = key_code;
                inputs[1].type = INPUT_KEYBOARD;
                inputs[1].ki.wVk = key_code;
                inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
                result = SendInput(1, inputs, sizeof(INPUT));
                break;
        }
    }
    else
    {
        INPUT inputs[2] = {};
        ZeroMemory(inputs, sizeof(inputs));
        inputs[0].type = INPUT_MOUSE;
        inputs[0].mi.dwFlags = key_code;
        inputs[1].type = INPUT_MOUSE;
        //inputs[1].mi.wVk = key_code;
        inputs[1].mi.dwFlags = key_code + 2;

        UINT result = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));

        if(result != ARRAYSIZE(inputs))
        {
            printf("Keypress failed\n");
            return -1;
        }
    }

    return 0;
};


#endif

int FBEvents::RunScript(std::string script)
{
    typeDelay = 0;
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
        {
            printf("ExecCommand failed, cmd: %s, arg: %s\n", cmd.c_str(), arg.c_str());
            return 1;
        }
    }
    return 0;
};

int FBEvents::ExecCommand(std::string command, std::string arg)
{
    std::vector<std::string> argList;
    std::string tmpArg;
    for(int i = 0; i <= arg.length(); i++)
    {
        if(arg[i] == ' ' || arg[i] == '\n' || i == arg.length())
        {
            if(tmpArg.length() > 0)
            {
                argList.push_back(tmpArg);
                tmpArg.clear();
            }
            continue;
        }
        else
            tmpArg.push_back(arg[i]);
    }
    if(command == "keypress")
    {
        if(fbP.keymap.find(argList[0]) != fbP.keymap.end())
        {
            const int FB_MOUSE_BUTTONS = 3;
            std::string mouseButtons[] = {"[LMB]", "[RMB]", "[MMB]"};
            for(int i = 0; i < FB_MOUSE_BUTTONS; i++)
                if(arg == mouseButtons[i])
                    return FBKeyPress(fbP.keymap[argList[0]], FB_TAP, FB_INPUT_MOUSE);
            return FBKeyPress(fbP.keymap[argList[0]], FB_TAP, FB_INPUT_KB);
        }
        else
        {
            printf("Failed to press %s\n", arg.c_str());
            return -1;
        }
    }
    if(command == "keydown")
    {
        if(fbP.keymap.find(argList[0]) != fbP.keymap.end())
            return FBKeyPress(fbP.keymap[argList[0]], FB_PRESS, FB_INPUT_KB);
        else
        {
            printf("Failed to press %s\n", arg.c_str());
            return -1;
        }
    }
    if(command == "keyup")
    {
        if(fbP.keymap.find(argList[0]) != fbP.keymap.end())
            return FBKeyPress(fbP.keymap[argList[0]], FB_RELEASE, FB_INPUT_KB);
        else
        {
            printf("Failed to press %s\n", arg.c_str());
            return -1;
        }
    }
    if(command == "sleep")
    {
        FB_SLEEP(atoi(argList[0].c_str()));
    }
    if(command == "exec")
    {
        #if defined(__linux__)
            arg += " &";
            std::system(arg.c_str());
        #else
        ShellExecuteA(NULL, NULL, arg.c_str(), NULL, NULL, SW_NORMAL);
        /*k
        CreateProcess()
        or
        execl()
        for Windows
        */
        #endif
    }
    if(command == "movemouse")
    {
        return FB_OK;
    }
    if(command == "type")
    {
        int result = 0;
        std::string tmp;
        for(int i = 0; i < arg.length(); i++)
        {
            tmp.clear();
            tmp += arg[i];
            if(fbP.keymap.find(tmp) == fbP.keymap.end())
            {
                printf("FBKP failed for %s\n", tmp.c_str());
                return -1;
            }
            if(tmp[0] >= 'A' && tmp[0] <= 'Z')
            {
                result = FBKeyPress(fbP.keymap["[SHIFT]"], FB_PRESS, FB_INPUT_KB);
                result = FBKeyPress(fbP.keymap[tmp], FB_TAP, FB_INPUT_KB);
                result = FBKeyPress(fbP.keymap["[SHIFT]"], FB_RELEASE, FB_INPUT_KB);
            }
            else
                result = FBKeyPress(fbP.keymap[tmp], FB_TAP, FB_INPUT_KB);
            if(typeDelay)
                FB_SLEEP(typeDelay);
            if(result)
            {
                printf("FBKP in \"type\" failed at %i char %s", i, tmp.c_str());
                return -1;
            }
        }
        return FB_OK;
    }
    if(command == "typedelay")
    {
        typeDelay = atoi(argList[0].c_str());
        return FB_OK;
    }

    return FB_OK;
};