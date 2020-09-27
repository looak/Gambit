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

# Options
option(BUILD_TESTS "Enable tests build" ON)
option(BUILD_CLI "Enable CLI build" ON)

# Exporting Compile commands to .json file that I can't find?
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Code Coverage
set(CODE_COVERAGE OFF CACHE BOOL "Code Coverage")
option(CODE_COVERAGE "Runs analysis on unit test, checking coverage" OFF)

# set(CMAKE_BUILD_TYPE Release)
if(CMAKE_BUILD_TYPE MATCHES Debug)
  message("Debug build.")
  add_definitions(-DDEBUG)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
  message("Release build.")
  add_definitions(-DRELEASE)
else()
  message("Some other build type.")
endif()

# compile options
if(MSVC)
  set(CMAKE_CXX_FLAGS_MSVC "/std:c++17 /W4 /w14640" )
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_MSVC} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_CXX_FLAGS_MSVC} /MTd")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wno-long-long -Wunused-variable -Wpedantic")
elseif (CODE_COVERAGE)
    message("Code Coverage Enabled")
    set(CMAKE_CXX_FLAGS "--coverage ${CMAKE_CXX_FLAGS}" )
endif()

set(CMAKE_CXX_STANDARD 17)

# output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# install directories
option(INSTALL_APP "Enable installation of app" OFF)
set(CMAKE_INSTALL_BINDIR "bin" CACHE STRING "User executables (bin)")
set(CMAKE_INSTALL_LIBDIR "lib${LIB_SUFFIX}" CACHE STRING "Object code libraries (lib)")
set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE STRING "C header files (include)")
mark_as_advanced(CMAKE_INSTALL_BINDIR CMAKE_INSTALL_LIBDIR CMAKE_INSTALL_INCLUDEDIR)  


add_subdirectory(engine)
if(BUILD_CLI)
    add_subdirectory(cli)
endif()
if(BUILD_TESTS)
    add_subdirectory(test)
endif()
