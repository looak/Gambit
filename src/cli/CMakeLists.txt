cmake_minimum_required(VERSION 3.2)

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