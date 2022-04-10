#if defined(__linux__)

#ifndef LINUX_TTY_H_
#define LINUX_TTY_H_

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <cstring>

class FB
{
    public:
        FB();
        int port;
        char lastChar = '0';
        struct termios tty;

        int OpenPort(std::string port_path);
        char Read();
        void ClosePort();
};

#endif
#endif