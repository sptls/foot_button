#ifndef FB_PARSER_H_

#include <string>
#include <map>

#define FB_NUMBER_OF_KEYWORDS 7

class FBParser
{
    public:
        FBParser();
        void LoadKeymap();

        std::map<std::string, int> keymap;

        std::string keywords[FB_NUMBER_OF_KEYWORDS] = {"keypress", "keydown", "keyup", "sleep", "exec", "movemouse", "type"};
        std::string fullScript;

        int LoadScript(std::string script);
        int CheckErrors(int &lineNr, int &columnNr, std::string *errMsg);
};

#endif