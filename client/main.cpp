#if defined(__linux__)
    #include "src/linux_tty.h"
    #include "src/fb_events.h"
#endif
#include <stdio.h>
#include <iostream>
#include "src/fb_parser.h"

int main(int argc, char** argv)
{
    FB fb;
    FBEvents fbE;
    FBParser fbParser;
    int lineNr = 0, columnNr = 0, result = 0;
    std::string errMsg;
    std::string test("keyup asdf\nexec 434\n");
    fbParser.LoadScript(test);
    result = fbParser.CheckErrors(lineNr, columnNr, &errMsg);
    if(result != 0)
    {
        printf("%s, line: %i, column: %i\n", errMsg.c_str(), lineNr, columnNr);
        return -1;
    }
    fb.OpenPort(argv[1]);

    printf("open port ok\n");
    char out = '0';
    while(1)
    {
        out = fb.Read();
        if(out == '1')
            fbE.FBKeyPress(fbParser.keymap["[SPACE]"]);
    }

    fb.ClosePort();
};