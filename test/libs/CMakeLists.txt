CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(a_dynamic_library)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ..)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ..)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ..)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ..)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ..)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ..)

IF(MSVC)
    ADD_COMPILE_OPTIONS(/wd4244)
    ADD_COMPILE_OPTIONS(/wd4838)
ENDIF()

ADD_LIBRARY(${PROJECT_NAME} SHARED 
    lib_dynamic.cpp
    ia32_modrm_map.cpp
    ia32_opcode_map.cpp
    mini_disassembler.cpp
)
