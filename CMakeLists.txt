cmake_minimum_required(VERSION 2.6)

project(Fiend)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/release)

FIND_PACKAGE(Fmod REQUIRED)

IF(FMOD_FOUND)
   INCLUDE_DIRECTORIES(${FMOD_INCLUDE_DIR})
ENDIF(FMOD_FOUND)

add_subdirectory(src/fiend)
