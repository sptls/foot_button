#if defined(__linux__)
    #define FB_SLEEP(x) usleep(x*1000)
#else
    #define FB_SLEEP(x) Sleep(x)
#endif


#include <stdio.h>
#include <string>
#include "src/footbutton.h"
#include "src/fb_events.h"

#include <fstream>
std::string GetScriptFromFile(char* filepath)
{
    std::string out, tmp;
    std::ifstream f;
    f.open(filepath);

    while(getline(f, tmp))
    {
        out += tmp;
        out += '\n';
    }
    
    return out;
};

int main(int argc, char** argv)
{
    std::string port;
    std::ifstream fPort;
    std::string script;

    fPort.open("last_working_port");
    if(fPort.is_open())
    {
        getline(fPort, port);
        fPort.close();
    }
    if(port == "")
    {
        if(argc < 2)
        {
            printf("Too few arguments\n");
FB_SLEEP(5000);
            return 1;
        }
        port = argv[1];
        script = GetScriptFromFile(argv[2]);
    }
    else
        script = GetScriptFromFile(argv[1]);
    FB fb;
    FBParser fbP;
    FBEvents fbE;

    if(script == "")
    {
        printf("Script file %s is empty!\n", argv[2]);
FB_SLEEP(5000);
        return -1;
    }
    int lineNr = 0, columnNr = 0, result = 0;
    fbP.LoadScript(script);
    int line = 0, row = 0;
    std::string errMsg;
    if(fbP.CheckErrors(line, row, &errMsg) != 0)
    {
        printf("%i, %i: %s\n", line, row, errMsg.c_str());
FB_SLEEP(5000);
        return 0;
    }
    //testrun
    //fbE.RunScript(script);
    //test

    int openResult;
    openResult = fb.OpenPort(port);
    if(openResult != 0)
    {
        printf("Failed opening port %s\n", port.c_str());
FB_SLEEP(5000);
        return -1;
    }
    else
    {
        std::ofstream f;
        f.open("last_working_port");
        f << port;
        f.close();
    }

    printf("Opening port %s SUCCESS\n", port.c_str());
    char out = '0';
    fflush(stdout);
    while(1)
    {
        FB_SLEEP(1);
        out = fb.Read();
        if(out == '1')
            fbE.RunScript(script);
    }

    fb.ClosePort();

    return 0;
};