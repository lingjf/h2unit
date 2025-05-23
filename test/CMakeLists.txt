CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(h2_test)

ADD_SUBDIRECTORY(libs)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG .)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG .)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG .)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE .)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE .)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE .)

FILE(GLOB SRC_TEST "test_types.cpp" "test_*.cpp")

IF(NOT CMAKE_CXX_STANDARD)
    SET(CMAKE_CXX_STANDARD 14)
ENDIF()
MESSAGE("C++ Standard is " ${CMAKE_CXX_STANDARD})

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF()
MESSAGE("Build Type is " ${CMAKE_BUILD_TYPE})

IF(CMAKE_BUILD_TYPE MATCHES "Release") 
    IF(MSVC)
        ADD_COMPILE_OPTIONS(/Od) # Disable Optimization
    ELSE()
        ADD_COMPILE_OPTIONS(-g -O0)
    ENDIF()
ENDIF()

IF(MSVC)
    ADD_COMPILE_OPTIONS(-bigobj)
    ADD_COMPILE_OPTIONS(/wd4101) #unreferenced local variable
    ADD_COMPILE_OPTIONS(/wd4804) #unsafe use of type 'bool' in operation ~ -
    ADD_COMPILE_OPTIONS(/wd4805) #comparison operations between bool and int
    ADD_COMPILE_OPTIONS(/wd4834) #discarding return value of function
ELSE()
    ADD_COMPILE_OPTIONS(-Wno-noexcept-type)
    ADD_COMPILE_OPTIONS(-Wno-conversion-null)
    ADD_COMPILE_OPTIONS(-Wall)
    IF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        ADD_COMPILE_OPTIONS(-Werror)
        ADD_COMPILE_OPTIONS(-fno-access-control)
    ENDIF()
ENDIF()


IF(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    ADD_COMPILE_OPTIONS(-pthread)
ENDIF()

LINK_DIRECTORIES(libs)
LINK_LIBRARIES(a_dynamic_library)

ADD_EXECUTABLE(a.out ${SRC_TEST})
TARGET_COMPILE_DEFINITIONS(a.out PUBLIC H2UNIT_SELF_UT=1)

IF(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    TARGET_LINK_LIBRARIES(a.out pthread)
ENDIF()

IF(CYGWIN OR MINGW)
    TARGET_LINK_LIBRARIES(a.out Ws2_32 Shlwapi Dbghelp)
ENDIF()

IF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    ADD_CUSTOM_TARGET(fixup ALL
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../h2fix.sh a.out
        DEPENDS a.out
    )
ENDIF()
