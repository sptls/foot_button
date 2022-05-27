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

BOOL CALLBACK GetProcessHandle(HWND hwnd, LPARAM lParam);
std::string findWindowWithThisTitle;

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
                FBKeyPress(key_code, FB_PRESS, FB_INPUT_KB);
                FBKeyPress(key_code, FB_RELEASE, FB_INPUT_KB);
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

int FBEvents::MoveMouse(std::string x, std::string y)
{
    bool bMoveAbsolute;
    if(x[0] == '+' || x[0] == '-' || y[0] == '+' || y[0] == '-')
        bMoveAbsolute = false;
    else
        bMoveAbsolute = true;

    LONG x_pos = atol(x.c_str());
    LONG y_pos = atol(y.c_str());
    UINT result;
    INPUT input;
    ZeroMemory(&input, sizeof(input));
    input.type = INPUT_MOUSE;
    input.mi.dx = x_pos;
    input.mi.dy = y_pos;
    if(bMoveAbsolute)
        input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
    else
        input.mi.dwFlags = MOUSEEVENTF_MOVE;
    
    result = SendInput(1, &input, sizeof(INPUT));

    return result == 0 ? FB_FAILED : FB_OK;
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
        std::string cmdName, cmdArgs;
        bool bHasArgs = false;
        for(int i = 0; i < arg.length(); i++)
        {
            if(arg[i] == ' ')
                break;
            else
                cmdName.push_back(arg[i]);
        }
        if(cmdName.length() < arg.length())
        {
            bHasArgs = true;
            for(int i = cmdName.length()+1; i < arg.length(); i++)
                cmdArgs.push_back(arg[i]);
        }
        HINSTANCE result;
        if(bHasArgs)
            result = ShellExecuteA(NULL, NULL, cmdName.c_str(), cmdArgs.c_str(), NULL, SW_NORMAL);
        else
            result = ShellExecuteA(NULL, NULL, arg.c_str(), NULL, NULL, SW_NORMAL);
        
        switch((INT_PTR)(result))
        {
            case ERROR_FILE_NOT_FOUND:
                printf("EXEC failed: file not found\n");
                break;
            case ERROR_PATH_NOT_FOUND:
                printf("EXEC failed: path not found\n");
                break;
            case ERROR_BAD_FORMAT:
                printf("EXEC failed: The .exe file is invalid (non-Win32 .exe or error in .exe image).\n");
                break;
            case SE_ERR_ACCESSDENIED:
                printf("EXEC failed: The operating system denied access to the specified file.\n");
                break;
        }
        return (INT_PTR)(result) > 32 ? FB_OK : FB_FAILED;
        #endif
    }
    if(command == "movemouse")
    {
        return MoveMouse(argList[0], argList[1]);
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
    if(command ==  "focuswindow")
    {
        FocusWindow(argList[0]);
        return FB_OK;
    }

    return FB_OK;
};

int FBEvents::FocusWindow(std::string windowTitle)
{
    //printf("FocusWindow: %s\n", windowTitle.c_str());
    findWindowWithThisTitle = windowTitle;
    EnumWindows(&GetProcessHandle, 1);
    return FB_OK;
};


BOOL CALLBACK GetProcessHandle(HWND hwnd, LPARAM lParam)
{
    char buff[1024];
    GetWindowTextA(hwnd, buff, 1024);
    std::string tmp = buff;
    if(tmp.find(findWindowWithThisTitle, 0) != std::string::npos)
    {
        //printf("Window: %s found as %s\nSwitching to %s\n", findWindowWithThisTitle.c_str(), buff, buff);
        SwitchToThisWindow(hwnd, true);
        return false;
    }
    return true;
}