cmake_minimum_required(VERSION 3.21)

project(tree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

IF(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
ENDIF()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

IF(NOT WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
ENDIF()


set(LLVM_CONFIG "F:/projects/llvm-project/build_deb/bin/llvm-config")
set(GTEST_ROOT "F:/projects/googletest")
set(LLVM_CONFIG_CXXFLAGS_ARGS "--cppflags")
set(LLVM_CONFIG_LIBS_ARGS "--libs" "engine" "all-targets")

execute_process(COMMAND ${LLVM_CONFIG} ${LLVM_CONFIG_CXXFLAGS_ARGS} OUTPUT_VARIABLE LLVM_CXXFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
set(CMAKE_CXX_FLAGS "${LLVM_CXXFLAGS} -Wno-deprecated-declarations")
execute_process(COMMAND ${LLVM_CONFIG} ${LLVM_CONFIG_LIBS_ARGS} OUTPUT_VARIABLE LLVM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)
separate_arguments(LLVM_LIBS)

add_subdirectory(src)
add_subdirectory(test)


