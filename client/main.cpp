#if defined(__linux__)
    #define FB_SLEEP(x) usleep(x*1000)
#else
    #define FB_SLEEP(x) Sleep(x)
#endif

#ifdef FB_CLIENT_GUI

#include "src/fb_events_g.h"
#include "src/fb_gui.h"

int main(int argc, char **argv)
{
    FBGui g;
    Glib::RefPtr<Gtk::Application> app = Gtk::Application::create("sptls.footbutton.gui");

    return app->run(*g.windowMain);
}

#endif


#ifdef FB_CLIENT_CLI

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
    std::string errMsg;

    if(!(strcmp(argv[1], "-mpos")))
    {
        size_t outLen = 0;
        std::string x, y, prevX, prevY;
        while(1)
        {
            fbE.GetMousePosition(x, y);
            if(x != prevX || y != prevY)
            {
                std::string out = "x: ";
                out += x;
                out += ", y: ";
                out += y;
                for(size_t i = 0; i < outLen; i++)
                    printf("\b");
                printf("%s", out.c_str());
                outLen = out.length();
                prevX = x;
                prevY = y;
                fflush(stdout);
            }
        }
    }

    if(!(strcmp(argv[1], "-runonce")))
    {
        script = GetScriptFromFile(argv[2]);
        fbP.LoadScript(script);
        if(fbP.CheckErrors(lineNr, columnNr, errMsg) != 0)
        {
            printf("%i, %i: %s\n", lineNr, columnNr, errMsg.c_str());
            return 0;
        }
        fbE.RunScript(fbP.subScripts[0]);
        return 0;
    }
    else
    {
        script = GetScriptFromFile(argv[1]);
        fbP.LoadScript(script);
        if(fbP.CheckErrors(lineNr, columnNr, errMsg) != 0)
        {
            printf("%i, %i: %s\n", lineNr, columnNr, errMsg.c_str());
            return 0;
        }
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
        printf("Opening port %s FAILED, exitting...\n", port);
        return -1;
    }

    printf("Opening port %s SUCCESS\n\n", port.c_str());
    printf("---> Loaded script: %s\n\n\n/-----------------------Contents:-------------------------\\\n\n%s\n\\---------------------------------------------------------/\n\n", argv[1], fbP.fullScript.c_str());
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
                scriptRunString = "               ---[ Script run " + std::to_string(counter) + " times ]---";
                printf("%s", scriptRunString.c_str());
                fflush(stdout);
            }
        }
    }

    fb.ClosePort();

    return 0;
};

#endif