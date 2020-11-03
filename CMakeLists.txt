cmake_minimum_required(VERSION 3.8)

#oof
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fpermissive -Wall -Wformat-security")

project(RefurnisherCore)

#our include dir
include_directories(include)
file(GLOB_RECURSE LIBRARY_SOURCE
    "bstream/*.h"
    "bstream/*.cpp"
    "include/*.h"
    "src/*.cpp"
)

add_library(RefurnisherCore STATIC ${LIBRARY_SOURCE})

# Quick testing stuff

add_executable(refurnisher_test tests/main.cpp)
target_link_libraries(refurnisher_test RefurnisherCore stdc++fs)

add_custom_command(
  OUTPUT refurnisher_run_tests
  COMMAND refurnisher_test
  COMMENT "RefurnisherCore Library Tests..."
  VERBATIM
)

add_custom_target(
  RunTests ALL
  DEPENDS refurnisher_run_tests
)