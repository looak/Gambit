#pragma once
#include <iostream>
#include <sstream>

static const std::string whitespace = " > ............................. ";
static const int lineLength = 28;
static std::string AddLineDivider(std::string command, std::string helpText)
{
    int lengthLeft = lineLength - command.length();
    _ASSERT(lengthLeft >= 0);

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