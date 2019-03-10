#pragma once
#include <sstream>

static std::string getVersion()
{
    std::ostringstream version;
    version << VER_MAJOR << '.' << VER_MINOR << '.' << VER_PATCH << '-' << VER_ENV;
    #ifdef DEBUG
    version << " (debug)";
    #elif  RELEASE
    version << " (release)";
    #else
    version << " (unkown)";
    #endif
    
    return version.str();
}