cmake_minimum_required(VERSION 3.27)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
add_executable(untitled src/main.cpp
        include/Entity.h
        include/Game.h
        include/Physics.h
        src/Entity.cpp
        src/Game.cpp
        src/Physics.cpp
        include/State.h)
include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(untitled sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Only set BOOST_ROOT if Boost is not installed in a standard location that CMake can find
set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/odeint")

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    # If you need to link Boost libraries, for example, Boost::filesystem
    # target_link_libraries(untitled ${Boost_LIBRARIES})
endif()

#add_subdirectory(tests/lib)