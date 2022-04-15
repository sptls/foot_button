#include "fb_parser.h"
#include <stdio.h>
#include <string>
#include <vector>

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
                return FB_FAILED;
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
        std::vector<std::string> argList;
        std::string tmpArg;
        for(int i = tmpCommand.length(); i <= tmp.length(); i++)
        {
            if(tmp[i] == ' ' || tmp[i] == '\n' || i == tmp.length())
            {
                if(tmpArg.length() > 0)
                {
                    argList.push_back(tmpArg);
                    tmpArg.clear();
                }
                continue;
            }
            else
                tmpArg.push_back(tmp[i]);
        }


        //check if commands have correct amount of arguments and check if arguments are correct
        if(tmpCommand == "keypress" || tmpCommand == "keyup" || tmpCommand == "keydown")
        {
            if(argList.size() > 1)
            {
                columnNr = 0;
                *errMsg = "Too many arguments for \"";
                *errMsg += tmpCommand;
                *errMsg += "\", expected 1, provided ";
                *errMsg += std::to_string(argList.size());
                return FB_FAILED;
            }
            else
            {
                if(keymap.find(argList[0]) == keymap.end())
                {
                    columnNr = tmp.find(argList[0]) + 1;
                    *errMsg = "Key \"";
                    *errMsg += argList[0];
                    *errMsg += "\" for function \"";
                    *errMsg += tmpCommand;
                    *errMsg += "\" not found!";
                    return FB_FAILED;
                }

            }
        }
        if(tmpCommand == "sleep")
        {
            if(argList.size() > 1)
            {
                columnNr = tmp.find(argList[1]);
                *errMsg = "Too many arguments for \"";
                *errMsg += tmpCommand;
                *errMsg += "\", expected 1, provided ";
                *errMsg += std::to_string(argList.size());
                return FB_FAILED;
            }
            for(int i = 0; i < argList[0].length(); i++)
            {
                if(!(isdigit(argList[0][i])))
                {
                    columnNr = tmp.find(argList[0]);
                    *errMsg = "Argument for \"";
                    *errMsg += tmpCommand;
                    *errMsg += "\", is not digit: \"";
                    *errMsg += argList[0];
                    *errMsg += "\"";
                    return FB_FAILED;
                }
            }
            if(argList[0].length() > 9)
            {
                columnNr = tmp.find(argList[0]);
                *errMsg = "Value for \"";
                *errMsg += tmpCommand;
                *errMsg += "\" is too big. Max value is 999999999, provided: ";
                *errMsg += argList[0];
                return FB_FAILED;
            }
        }
        if(tmpCommand == "movemouse")
        {
            if(argList.size() != 2)
            {
                columnNr = tmp.find(argList[0]);
                *errMsg = "Incorrect number of arguments for \"";
                *errMsg += tmpCommand;
                *errMsg += "\", expected 2, provided ";
                *errMsg += atoi(argList.size().c_str());
                return FB_FAILED;
            }
        }
    }

    return FB_OK;
};