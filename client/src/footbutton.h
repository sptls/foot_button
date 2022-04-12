#ifndef FOOTBUTTON_H_
#define FOOTBUTTON_H_

#include <stdio.h>
#include <string>
#include <cstring>

#if defined(__linux__)
    #include <fcntl.h>
    #include <errno.h>
    #include <termios.h>
    #include <unistd.h>
#else
    #include <windows.h>
    //windows headers

class FB
{
    public:
        FB();
        int                 port;
        char                lastChar = '0';
        #if defined(__linux__)
            struct termios tty;
        #else
            HANDLE          serialHandle;
            DCB             dcbSerialParams; 
            COMMTIMEOUTS    comTimeout;
        #endif
        //linux: /dev/ttyUSB*
        //windows \\\\.\\COM*
        int OpenPort(std::string port_path);
        char Read();
        void ClosePort();
};

#endif
#endif