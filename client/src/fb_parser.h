#ifndef FB_PARSER_H_

#include <string>
#include <map>

#ifndef FB_FAILED
#define FB_FAILED 1
#define FB_OK 0
#endif

#define FB_NUMBER_OF_KEYWORDS 8

class FBParser
{
    public:
        FBParser();
        void LoadKeymap();

        std::map<std::string, int> keymap;

        std::string keywords[FB_NUMBER_OF_KEYWORDS] = {"keypress", "keydown", "keyup", "sleep", "exec", "movemouse", "type", "typedelay"};
        std::string fullScript;

        int LoadScript(std::string script);
        int CheckErrors(int &lineNr, int &columnNr, std::string *errMsg);
};

#endif