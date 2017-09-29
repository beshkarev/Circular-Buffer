cmake_minimum_required(VERSION 2.8)
project(CircularBuffer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/install")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/install")

# common compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pedantic-errors")
# only debug compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
# only release compiler flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src)

set(CORE_LIB_SOURCE
    src/circularBuffer.cpp
    )

set(EXE_SOURCE "src/main.cpp")

find_package(CxxTest REQUIRED)
if(CXXTEST_FOUND)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
  CXXTEST_ADD_TEST(tests
      runner.cpp  # THIS IS GENERATED BUT YOU NEED TO ADD IT HERE
      ${PROJECT_SOURCE_DIR}/src/circularBuffer.cpp  # YOUR PROJECT FILES THAT WILL BE USED FOR YOUR TEST
      ${PROJECT_SOURCE_DIR}/src/tests/circularBufferTest.h  # YOUR TEST DESCRIPTION FILE
  )
 endif()

add_executable(CircularBuffer ${EXE_SOURCE})

add_library(core SHARED ${CORE_LIB_SOURCE})

target_link_libraries(CircularBuffer
    core
    )
