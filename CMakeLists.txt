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

cmake_minimum_required(VERSION 3.10)
message("CMake Version: ${CMAKE_VERSION}")

# Versioning
set(ver_major 0)
set(ver_minor 1)
set(ver_patch 0)
set(ver_env "dev")

project(Gambit VERSION ${ver_major}.${ver_minor}.${ver_patch})
message("Gambit Project Version: ${ver_major}.${ver_minor}.${ver_patch}-${ver_env}")
add_definitions(-DVER_MAJOR=${ver_major})
add_definitions(-DVER_MINOR=${ver_minor})
add_definitions(-DVER_PATCH=${ver_patch})
add_definitions(-DVER_ENV=${ver_env})

set(CMAKE_CONFIGURATION_TYPES Release Debug)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(src)