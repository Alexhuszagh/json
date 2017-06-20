#  :copyright: (c) 2015-2017 The Regents of the University of California.
#  :license: MIT, see LICENSE.md for more details.

cmake_minimum_required(VERSION 3.4)
project(json CXX)
enable_testing()

# FLAGS
# -----

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD TRUE)

option(BUILD_STATIC "Build static library" ON)
option(BUILD_TESTS "Build JSON library tests" OFF)
option(BUILD_FUZZ "Build fuzz testing" OFF)
option(BUILD_EXAMPLES "Build example files" OFF)

if(BUILD_STATIC)
    if(UNIX)
        set(CMAKE_POSITION_INDEPENDENT_CODE ON)
        add_definitions(-fPIC)
    elseif(MINGW)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
    elseif(MSVC)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /O0 /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /Ox /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT /Zi /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MT /Fd${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pdb")
    endif()
endif()

# Need to manually set compiler flags for Travis CI
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
endif()

# DEPENDENCIES
# ------------

if(NOT TARGET Lexi)
    add_subdirectory(lexi)
endif()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/lexi/include")

# LIBRARY
# -------

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

set(SOURCE_FILES
    src/except.cpp
    src/iterator.cpp
    src/reader.cpp
    src/view.cpp
    src/writer.cpp
)

if(BUILD_STATIC)
    add_library(json STATIC ${SOURCE_FILES})
else()
    add_library(json SHARED ${SOURCE_FILES})
endif()
set(JSON_LIBRARIES json)

# EXAMPLES
# --------

if(BUILD_EXAMPLES)
    add_executable(json_simplewriter example/simplewriter.cpp)
    target_link_libraries(json_simplewriter ${JSON_LIBRARIES} Lexi)

    add_executable(json_simplereader example/simplereader.cpp)
    target_link_libraries(json_simplereader ${JSON_LIBRARIES} Lexi)

    add_executable(json_range example/range.cpp)
    target_link_libraries(json_range ${JSON_LIBRARIES} Lexi)

    add_executable(json_stl example/stl.cpp)
    target_link_libraries(json_stl ${JSON_LIBRARIES} Lexi)
endif()

# TESTS
# -----

set(TEST_FILES
    test/main.cpp
    test/benchmark.cpp
    test/checker.cpp
    test/data.cpp
    test/iterator.cpp
    test/overrun.cpp
    test/rfc7419.cpp
    test/reader.cpp
    test/writer.cpp
)

if(BUILD_TESTS)
    if(NOT TARGET gtest)
        add_subdirectory(googletest)
    endif()
    add_executable(json_tests ${TEST_FILES})
    target_link_libraries(json_tests
        gtest
        gtest_main
        ${JSON_LIBRARIES}
        Lexi
    )
    set_target_properties(json_tests PROPERTIES OUTPUT_NAME test)

    # Need to specify proper exception handling for Windows.
    if(MSVC)
        set_target_properties(json_tests PROPERTIES
            COMPILE_OPTIONS "/EHsc"
        )
    endif()

     add_test(NAME json_tests
        COMMAND json_tests
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()


if(BUILD_FUZZ AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=bool -fsanitize-coverage=edge,8bit-counters,trace-cmp")
    if(NOT TARGET LLVMFuzzer)
        add_subdirectory(Fuzzer)
    endif()

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads)

    add_executable(json_fuzzreader test/fuzz/reader.cpp)
    target_link_libraries(json_fuzzreader LLVMFuzzer ${JSON_LIBRARIES} Lexi Threads::Threads)

    add_executable(json_fuzzwriter test/fuzz/writer.cpp)
    target_link_libraries(json_fuzzwriter LLVMFuzzer ${JSON_LIBRARIES} Lexi Threads::Threads)
endif()
