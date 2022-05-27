/*
* This class contains error checking
*/
#ifndef FB_PARSER_H_

#include <string>
#include <map>
#include <vector>

#ifndef FB_FAILED
#define FB_FAILED 1
#define FB_OK 0
#endif

#define FB_NUMBER_OF_KEYWORDS 9

class FBParser
{
        void LoadKeymap();
        std::string keywords[FB_NUMBER_OF_KEYWORDS] = {"keypress", "keydown", "keyup", "sleep", "exec", "movemouse", "type", "typedelay", "focuswindow"};
        int CheckSubScript(std::string subString, int &lineNr, int &columnNr, std::string *errMsg);
    public:
        FBParser();
        std::map<std::string, int> keymap;
        std::string fullScript;
        std::vector<std::string> subScripts;
        int LoadScript(std::string script);
        int CheckErrors(int &lineNr, int &columnNr, std::string &errMsg);
};

#endif