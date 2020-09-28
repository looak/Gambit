// Gambit Chess Engine - a Chess AI
// Copyright (C) 2019  Alexander Loodin Ek

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once
#include <sstream>

enum VersionEnvironment
{
    dev,
    rc,
};

static std::string getVersion()
{
    std::ostringstream version;
    version << VER_MAJOR << '.' << VER_MINOR << '.' << VER_PATCH << '-';
    
    if constexpr(VER_ENV == dev)
    {
        version << "dev";
    }
    else
    {
        version << "rc";
    }

    version << " (" << CMAKE_INTDIR << ")";
    
    
    return version.str();
}