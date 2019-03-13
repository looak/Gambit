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

set(LIBRARY_NAME GambitCLI)
set(PROJECT_NAME GambitCLI)

project(${PROJECT_NAME} C CXX)

set(GambitCLI_SCR
        main.cpp
        commands.h
        commands.cpp
        printCommand.h
        printCommand.cpp
        commandsUtils.h)


include_directories(../engine/inc/)
add_executable(${PROJECT_NAME} ${GambitCLI_SCR})
target_link_libraries(${PROJECT_NAME} GambitEngine)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)