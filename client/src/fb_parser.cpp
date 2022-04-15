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
            columnNr = tmp.find(tmpCommand);
            *errMsg = "Unknow command \"";
            *errMsg += tmpCommand;
            *errMsg += "\"";
            return FB_FAILED;
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
        if(tmpCommand == "sleep" || tmpCommand == "typedelay")
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
            std::string mmUsage = "mousemove +/-x +/-y or mousemove 32561 10203 ";
            if(argList.size() != 2)
            {
                columnNr = tmp.find(argList[0]);
                *errMsg = "Incorrect number of arguments for \"";
                *errMsg += tmpCommand;
                *errMsg += "\", expected 2, provided ";
                *errMsg += std::to_string(argList.size());
                return FB_FAILED;
            }

            const int ALLOWED_CHARS_SIZE = 13;
            char allowedChars[ALLOWED_CHARS_SIZE] = "+-1234567890";
            bool argGood = false;
            bool mmRelative = false;
            int argNr = 0;
            for(int loops = 0; loops < 2; loops++)
            {
                for(int a = 0; a < argList[loops].length(); a++)
                {
                    argGood = false;
                    for(int c = 0; c < ALLOWED_CHARS_SIZE; c++)
                    {
                        if(argList[loops][a] == allowedChars[c])
                        {
                            if(c < 2)
                                argGood = a == 0 ? true : false;
                            else
                                argGood = true;
                            break;
                        }
                    }
                    if(!argGood)
                        break;
                }
                if(!argGood)
                {
                    argNr = loops;
                    break;
                }
            }
            if(!argGood)
            {
                columnNr = tmp.find(argList[argNr]);
                *errMsg = "Value for \"";
                *errMsg += tmpCommand;
                *errMsg += "\" is not correct: ";
                *errMsg += argList[argNr];
                return FB_FAILED;
            }
            bool isRelative = false;
            if(argList[0][0] == '+' || argList[0][0] == '-')
            {
                if(argList[1][0] == '+' || argList[1][0] == '-')
                    isRelative = true;
            }

            printf("%i, %i\n", atoi(argList[0].c_str()), atoi(argList[1].c_str()));
            int x, y;
            x = atoi(argList[0].c_str());
            if(x < 0)
                x *= -1;
            y = atoi(argList[1].c_str());
            if(y < 0)
                y *= -1;
            if(x > 65535)
            {
                columnNr = tmp.find(argList[0]);
                *errMsg = "Value for X mouse position is too high(max 65535): ";
                *errMsg += argList[0];
                return FB_FAILED;
            }
            if(y > 65535)
            {
                columnNr = tmp.find(argList[1]);
                *errMsg = "Value for Y mouse position is too high(max 65535): ";
                *errMsg += argList[1];
                return FB_FAILED;
            }
        }
    }

    return FB_OK;
};