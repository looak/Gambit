cmake_minimum_required(VERSION 3.2)
project(Gambit VERSION 0.0.0)

if(MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -Wunused-variable -pedantic")
endif()

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

#set(Gambit_INC ${PROJECT_SOURCE_DIR}/engine/src)

add_subdirectory(engine)
add_subdirectory(test)
