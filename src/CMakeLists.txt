cmake_minimum_required(VERSION 3.2)
project(Gambit VERSION 0.0.0)

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

#set(Gambit_INC ${PROJECT_SOURCE_DIR}/engine/src)

add_subdirectory(engine)
add_subdirectory(test)
