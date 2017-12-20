cmake_minimum_required(VERSION 3.2)
project(Gambit VERSION 0.0.0)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14")

#set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

#set(Gambit_INC ${PROJECT_SOURCE_DIR}/engine/src)

add_subdirectory(engine)
add_subdirectory(test)
