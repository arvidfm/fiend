cmake_minimum_required(VERSION 2.6)

project(Fiend)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

FIND_PACKAGE(Fmod REQUIRED)

IF(FMOD_FOUND)
   INCLUDE_DIRECTORIES(${FMOD_INCLUDE_DIR})
ENDIF(FMOD_FOUND)

add_subdirectory(src/fiend)
