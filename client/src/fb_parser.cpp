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
        std::string tmp;
        lineNr++;
        for(int j = i; fullScript[j] != '\n'; j++)
            tmp.push_back(fullScript[j]);
        i += tmp.length();
        std::string tmpCommand = "";
        for(int j = 0; j < tmp.length(); j++)
        {
            if(tmp[j] != ' ')
                tmpCommand.push_back(tmp[j]);
            else
                break;
        }
        bool bValidCommand = false;
        for(int j = 0; j < 6; j++)
        {
            if(tmpCommand == keywords[j])
                bValidCommand = true;
        }
        if(!bValidCommand)
        {
            columnNr = 0;
            *errMsg = "Unknow command \"";
            *errMsg += tmpCommand.c_str();
            *errMsg += "\"";
            return -1;
        }
    }

    return 0;
};