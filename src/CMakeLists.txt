# Gambit Chess Engine - a Chess AI
# Copyright (C) 2019  Alexander Loodin Ek

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Versioning
set(ver_major 0)
set(ver_minor 1)
set(ver_patch 0)
set(ver_env "\"dev\"")

project(Gambit VERSION ${ver_major}.${ver_minor}.${ver_patch})
message("version: ${ver_major}.${ver_minor}.${ver_patch}-${ver_env}")
add_definitions(-DVER_MAJOR=${ver_major})
add_definitions(-DVER_MINOR=${ver_minor})
add_definitions(-DVER_PATCH=${ver_patch})
add_definitions(-DVER_ENV=${ver_env})

# Exporting Compile commands to .json file that I can't find?
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Code Coverage
set(CODE_COVERAGE OFF CACHE BOOL "Code Coverage")
option(CODE_COVERAGE "Runs analysis on unit test, checking coverage" OFF)

# set(CMAKE_BUILD_TYPE Release)
IF(CMAKE_BUILD_TYPE MATCHES Debug)
  message("Debug build.")
  add_definitions(-DDEBUG)
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
  message("Release build.")
  add_definitions(-DRELEASE)
ELSE()
  message("Some other build type.")
ENDIF()

if(MSVC)
  set(CMAKE_CXX_FLAGS_MSVC "/std:c++14 /permissive /W4 /w14640")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_MSVC} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_MSVC} /MTd")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  message("GCC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wno-long-long -Wunused-variable -Wpedantic")
endif()

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(engine)
add_subdirectory(cli)
add_subdirectory(test)
