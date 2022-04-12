#include "fb_parser.h"

#if defined(__linux__)

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

    char c = '!';
    for(int i = 0x0021; i <= 0x007c; i++, c++)
    {
        std::string tmp = "";
        tmp += c;
        keymap[tmp] = i;
    }
};

#else

void FBParser::LoadKeymap()
{
    keymap["[ENTER]"] = 0x0d;
    keymap["[BACKSPACE]"] = 0x08;
    keymap["[ESCAPE]"] = 0x1b;
    keymap["[TAB]"] = 0x09;
    keymap["[DELETE]"] = 0x2e;
    keymap["[UP]"] = 0x26;
    keymap["[DOWN]"] = 0x28;
    keymap["[LEFT]"] = 0x25;
    keymap["[RIGHT]"] = 0x27;
    keymap["[SPACE]"] = 0x20;
    for(char i = 'A'; i <= 'Z'; i++)
        keymap[std::to_string(i)] = i;
    for(char i = '0'; i <= '9'; i++)
        keymap[std::to_string(i)] = i;
};

#endif