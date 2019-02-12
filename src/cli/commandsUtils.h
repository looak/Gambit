#pragma once
#include <iostream>
#include <sstream>
#include <exception>
#include <stdexcept>

static const std::string whitespace = " > ............................. ";
static const int lineLength = 28;
static std::string AddLineDivider(std::string command, std::string helpText)
{
    int lengthLeft = lineLength - command.length();
    if(lengthLeft < 0)
        throw new std::range_error("AddLineDevider is too short!");
    
    std::stringstream output;
    output << " > " << command << ' ';
    while (lengthLeft > 0)
    {
        output << '.';
        --lengthLeft;
    }
    output << " " << helpText;
    return output.str();
}