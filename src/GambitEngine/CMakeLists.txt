cmake_minimum_required(VERSION 3.1)
set(LIBRARY_NAME	GambitEngineLib)

project(GambitEngine)

file(GLOB 
GambitEngine_SRC 
src/* )

#file(GLOB GambitEngine_INCLUDE include/* )

include_directories(${GambitEngine_INCLUDE})

add_library(${GambitEngineLib} SHARED ${GambitEngine_SRC})


#target_link_libraries(${GambitEngineLib} ${GambitEngine_INCLUDE})