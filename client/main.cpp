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
    if(argc < 2)
    {
        printf("Too few arguments\n");
        return 1;
    }
    FB fb;
    FBParser fbP;
    FBEvents fbE;
    std::string script = GetScriptFromFile(argv[2]);
    int lineNr = 0, columnNr = 0, result = 0;
    fbP.LoadScript(script);
    int line = 0, row = 0;
    std::string errMsg;
    if(fbP.CheckErrors(line, row, &errMsg) != 0)
    {
        printf("%i, %i: %s\n", line, row, errMsg.c_str());
        return 0;
    }
    //FB_SLEEP(3000);
    //fbE.RunScript(testScript);

    int openResult;
    openResult = fb.OpenPort(argv[1]);
    if(openResult != 0)
    {
        printf("Failed opening port %s", argv[1]);
        return -1;
    }
    

    printf("open port ok\n");
    char out = '0';
    while(1)
    {
        out = fb.Read();
        if(out == '1')
            fbE.RunScript(script);
    }

    fb.ClosePort();

    return 0;
};