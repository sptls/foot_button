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
    std::string         script;
    FB                  fb;
    FBParser            fbP;
    FBEvents            fbE;

    int lineNr = 0, columnNr = 0, result = 0;
    script = GetScriptFromFile(argv[1]);
    fbP.LoadScript(script);
    std::string errMsg;
    if(fbP.CheckErrors(lineNr, columnNr, errMsg) != 0)
    {
        printf("%i, %i: %s\n", lineNr, columnNr, errMsg.c_str());
        return 0;
    }

    int openResult;
    std::ifstream fPort;
    fPort.open("port");
    if(!fPort.is_open())
    {
        printf("Failed opening \"port\" file!\n");
        return -1;
    }
    std::string port;
    getline(fPort, port);
    fPort.close();
    openResult = fb.OpenPort(port);
    if(openResult != 0)
    {
        printf("Failed opening port %s\n", port);
        return -1;
    }

    printf("Opening port %s SUCCESS\n", port.c_str());
    printf("Loaded script: %s\nContents:\n/---------------------------------------------------------\\\n%s\n\\---------------------------------------------------------/\n", argv[1], fbP.fullScript.c_str());
    fflush(stdout);
    char out = '0';

    std::ifstream fLoop;


    int currentSubScript = 0;
    char prevOut = '0';
    std::string scriptRunString;
    int counter = 0;
    while(1)
    {
        fLoop.open("end");
        if(fLoop.is_open())
        {
            fLoop.close();
            break;
        }
        FB_SLEEP(1);
        out = fb.Read();
        if(out != prevOut)
        {
            prevOut = out;
            if(out == '1')
            {
                fbE.RunScript(fbP.subScripts[currentSubScript]);
                currentSubScript++;
                fflush(stdout);
                if(currentSubScript >= lineNr)
                    currentSubScript = 0;
                counter++;
                for(int i = 0; i < scriptRunString.length(); i++)
                {
                    printf("\b");
                    fflush(stdout);
                }
                scriptRunString = "Script run " + std::to_string(counter) + " times";
                printf("%s", scriptRunString.c_str());
                fflush(stdout);
            }
        }
    }

    fb.ClosePort();

    return 0;
};