CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(h2_testing)

SET(CMAKE_CXX_STANDARD 14)

FILE(GLOB SRC_TEST "test_*.cpp")

FOREACH(i IN LISTS SRC_TEST)
    GET_FILENAME_COMPONENT(j "${i}" NAME_WE)

    ADD_EXECUTABLE(${j}.out ${i})

    ADD_CUSTOM_TARGET(fixup_${j}.out ALL
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../h2fix.sh ${j}.out
        COMMAND echo './${j}.out > ${j}.exp' >> gen_expected.sh
        DEPENDS ${j}.out 
    )
ENDFOREACH(i)
