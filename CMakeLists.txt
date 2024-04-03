cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 17)

# SFML
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(/usr/local/include)

# Boost - Only if you're using features from Boost
find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

# Main executable
add_executable(untitled
        src/main.cpp
        src/Entity.cpp
        src/Game.cpp
        src/Physics.cpp
)
target_link_libraries(untitled sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Test
add_subdirectory(external/Catch2)

# For Catch2 version 3.x or above
find_package(Catch2 REQUIRED)

add_executable(test_run
        test/gameTest.cpp
        src/Entity.cpp
        src/Game.cpp
        src/Physics.cpp
        test/gameTest.cpp
        test/testing.cpp
)
target_link_libraries(test_run PRIVATE Catch2::Catch2WithMain sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
target_include_directories(test_run PRIVATE ${PROJECT_SOURCE_DIR}/include)

include(CTest)
include(Catch)
catch_discover_tests(test_run)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
    add_compile_options(--coverage)
    link_libraries(--coverage)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

