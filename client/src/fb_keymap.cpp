#include "fb_parser.h"

void FBParser::LoadKeymap()
{
    keymap["[ENTER]"] = 0xff0d;
    keymap["[BACKSPACE]"] = 0xff08;
    keymap["[ESCAPE]"] = 0xff1b;
    keymap["[TAB]"] = 0xff09;
    keymap["[DELETE]"] = 0xffff;
    keymap["[UP]"] = 0xff52;
    keymap["[DOWN]"] = 0xff54;
    keymap["[LEFT]"] = 0xff51;
    keymap["[RIGHT]"] = 0xff53;
    keymap["[SPACE]"] = 0x0020;

    std::string numbers = "0123456789";
    for(int i = 0, keyValue = 0x0030; i < numbers.length(); i++, keyValue++)
    {
        std::string tmp = "" + numbers[i];
        keymap[tmp] = keyValue;
    }

};