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
    //mouse
    keymap["[LMB]"] = 0x0002;
    keymap["[RMB]"] = 0x0008;
    //keymap["[MMB]"] = 0x04;
    //kb
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
    keymap[" "] = 0x20;
    //if no shift specified press left one
    keymap["[SHIFT]"] = 0xA0;
    keymap["[LSHIFT]"] = 0xA0;
    keymap["[RSHIFT]"] = 0xA1;
    //if no Ctrl specified press left one
    keymap["[CTRL]"] = 0xA2;
    keymap["[LCTRL]"] = 0xA2;
    keymap["[RCTRL]"] = 0xA3;
    keymap["[WIN]"] = 0x5b;
    keymap["[ALT]"] = 0x12;
    
    std::string tmp = "";
    for(char i = 'A', s = 'a'; i <= 'Z'; s++, i++)
    {
        tmp.clear();
        tmp += i;
        keymap[tmp] = i;
        tmp.clear();
        tmp += s;
        keymap[tmp] = i;
    }
    //    keymap[std::to_string(i)] = i;
    for(char i = '0'; i <= '9'; i++)
    {
        tmp.clear();
        tmp += i;
        keymap[tmp] = i;
    }

    //F1-F12
    for(int i = 0x70, f = 1; i < 0x7c; i++, f++)
    {
        tmp.clear();
        tmp += 'F' + std::to_string(f);
        keymap[tmp] = i;
    }

    //other keys
    std::string otherKeys = ";=,-./`";
    for(int i = 0xba, x = 0; i < 0xc0; x++, i++)
    {
        tmp.clear();
        tmp += otherKeys[x];
        keymap[tmp] = i;
    }
    otherKeys = "[\\]'";
    for(int i = 0xdb, x = 0; i < 0xdf; x++, i++)
    {
        tmp.clear();
        tmp += otherKeys[x];
        keymap[tmp] = i;
    }
};

#endif