CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(h2_benchmark)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG .)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG .)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG .)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE .)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE .)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE .)

SET(CMAKE_CXX_STANDARD 11)

OPTION(H2UNIT1 "using h2unit.h" OFF)
OPTION(H2UNIT2 "using h2unit.hpp/cpp" OFF)

SET(SRC_TEST 1v2.cpp)

IF(NOT TESTCOUNT)
    SET(TESTCOUNT 8)
ENDIF()

FOREACH(i RANGE ${TESTCOUNT})
    EXECUTE_PROCESS(COMMAND cp ../benchmark/test.cpp test_${i}.cpp)
    list(APPEND SRC_TEST test_${i}.cpp)
ENDFOREACH(i)

IF(NOT H2UNIT1 AND NOT H2UNIT2)
    SET(H2UNIT1 ON)
ENDIF()

IF(H2UNIT1)
    ADD_EXECUTABLE(a.out ${SRC_TEST})
    TARGET_COMPILE_DEFINITIONS(a.out PUBLIC H2UNIT=1)
ENDIF(H2UNIT1)
IF(H2UNIT2)
    ADD_EXECUTABLE(a.out ${SRC_TEST} ../build/h2unit.cpp)
    TARGET_COMPILE_DEFINITIONS(a.out PUBLIC H2UNIT=2)
ENDIF(H2UNIT2)

IF(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    ADD_CUSTOM_TARGET(fixup ALL
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../h2fix.sh a.out
        DEPENDS a.out
    )
ENDIF()
