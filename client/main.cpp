#include <stdio.h>
#include <string>
#include "src/footbutton.h"
#include "src/fb_events.h"

int main(int argc, char** argv)
{
    FB fb;
    FBEvents fbE;
    int lineNr = 0, columnNr = 0, result = 0;
    std::string testScript = "keypress [SPACE]\nkeypress D";
    Sleep(3000);
    fbE.RunScript(testScript);

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
            fbE.FBKeyPress(1);
    }

    fb.ClosePort();
};