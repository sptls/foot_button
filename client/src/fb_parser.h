#ifndef FB_PARSER_H_

#include <string>
#include <map>

class FBParser
{
    public:
        FBParser();
        void LoadKeymap();

        std::map<std::string, int> keymap;

        std::string keywords[6] = {"keypress", "keydown", "keyup", "sleep", "exec", "movemouse"};
        std::string fullScript;

        int LoadScript(std::string script);
        int CheckErrors(int &lineNr, int &columnNr, std::string *errMsg);
};

#endif