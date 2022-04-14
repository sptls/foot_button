#include "fb_parser.h"
#include <stdio.h>
#include <iostream>

FBParser::FBParser()
{
    LoadKeymap();
};

int FBParser::LoadScript(std::string script)
{
    fullScript = script;
    return 0;
};

int FBParser::CheckErrors(int &lineNr, int &columnNr, std::string *errMsg)
{
    for(int i = 0; i < fullScript.length(); i++)
    {
        std::string tmp = "";
        lineNr++;
        int k = i;
        //copy script text to new buffer until new line or eof reached
        for(; fullScript[k] != '\n' && k < fullScript.length(); k++)
        {
            if(fullScript[k] == ' ' && tmp.length() == 0)
                continue;
            else
                tmp.push_back(fullScript[k]);
        }
        i = k;

        //copy command to new buffer, check if it's valid and does it have any arguments
        std::string tmpCommand = "";
        for(int j = 0; j < tmp.length(); j++)
        {
            if(j == tmp.length() - 1)
            {
                columnNr = j + 1;
                *errMsg = "No arguments provided";
                return -1;
            }
            if(tmp[j] != ' ')
                tmpCommand.push_back(tmp[j]);
            else
                break;
        }
        bool bValidCommand = false;
        for(int j = 0; j < FB_NUMBER_OF_KEYWORDS; j++)
        {
            if(tmpCommand == keywords[j])
                bValidCommand = true;
        }
        if(!bValidCommand)
        {
            columnNr = 1;
            *errMsg = "Unknow command \"";
            *errMsg += tmpCommand.c_str();
            *errMsg += "\"";
            return -1;
        }

        //check if commands have correct amount of arguments and check if arguments are correct
        if(tmpCommand == "keypress")
        {
            int countSpaces = 0;
            for(int s = 0; s < tmp.length(); s++)
            {
                if(tmp[s] == ' ')
                    countSpaces++;
                if(countSpaces > 1)
                {
                    columnNr = s + 1;
                    *errMsg = "Too many arguments or spaces for this command";
                    return -1;
                }
            }

            std::string checkArg = "";
            for(int s = tmpCommand.length(); s < tmp.length() && tmp[s] != '\n'; s++)
            {
                if(tmp[s] == ' ')
                    continue;
                else
                    checkArg.push_back(tmp[s]);
            }
            if(keymap.find(checkArg) == keymap.end())
            {
                columnNr = tmpCommand.length() + 1;
                *errMsg = "Key value: \"";
                *errMsg += checkArg;
                *errMsg += "\" is invalid";
                printf("checkArg.length() = %i\n", checkArg.length());
                return -1;
            }
        }
    }

    return 0;
};