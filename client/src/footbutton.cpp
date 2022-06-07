#include "footbutton.h"

#if defined(__linux__)


//Linux specific
FB::FB()
{
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
};

int FB::OpenPort(std::string port_path)
{
    port = open(port_path.c_str(), O_RDWR);
    if(tcsetattr(port, TCSANOW, &tty) != 0)
    {
        printf("Error %i from tcsetattr:\n", errno);
        return 1;
    }
    return 0;
};

char FB::Read()
{
    char out = 'x';
    int bytes_read = read(port, &out, sizeof(out));
    if(bytes_read < 0)
    {
        printf("Error reading: %s");
        return 'x';
    }
    if(bytes_read == 0)
        out = lastChar;
    else
        lastChar = out;
    return out;
};

void FB::ClosePort()
{
    //do nothing
};

#else

//Windows specific

FB::FB()
{
    dcbSerialParams = {sizeof(DCB)};
    dcbSerialParams.DCBlength = sizeof(DCB);
    dcbSerialParams.BaudRate=CBR_9600;
    dcbSerialParams.ByteSize=8;
    dcbSerialParams.StopBits=ONESTOPBIT;
    dcbSerialParams.Parity=NOPARITY;

    comTimeout.ReadIntervalTimeout=2000;
    comTimeout.ReadTotalTimeoutConstant=2000;
    comTimeout.ReadTotalTimeoutMultiplier=2000;
    comTimeout.WriteTotalTimeoutConstant=2000;
    comTimeout.WriteTotalTimeoutMultiplier=2000;
}

int FB::OpenPort(std::string port_path)
{
    serialHandle = CreateFile(port_path.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

    if(serialHandle == INVALID_HANDLE_VALUE)
    {
        printf("Failed to open port - port %s doesn't exist\n", port_path.c_str());
        return 1;
    }

    if(!SetCommState(serialHandle, &dcbSerialParams))
    {
        printf("Failed to set params for handle\n");
        return 1;
    }

    if(!SetCommTimeouts(serialHandle, &comTimeout))
    {
        printf("Failed to set handle timeouts\n");
        return 1;
    }

    return 0;
};

char FB::Read()
{
    char out = 'x';
    DWORD bytes_read = 0;
    if(!ReadFile(serialHandle, &out, 1, &bytes_read, NULL))
    {
        printf("Failed reading from COM port\n");
        return 'x';
    }
    if(bytes_read == 0)
        out = lastChar;
    else
        lastChar = out;
    return out;
};

void FB::ClosePort()
{
    CloseHandle(serialHandle);
}

#endif