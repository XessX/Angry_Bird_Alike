# Add Catch2 subdirectory
add_subdirectory(external/Catch2)

# Define test executable
add_executable(test_run
        test/gameTest.cpp
        src/Entity.cpp
        src/Game.cpp
        src/Physics.cpp
        src/main.cpp
        test/testing.cpp
)

# Include Catch2's header files
target_include_directories(test_run PRIVATE ${CATCH_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)

# Link SFML and Catch2 libraries to your test executable
target_link_libraries(test_run PRIVATE Catch2::Catch2 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Discover and register tests
include(CTest)
include(Catch)
catch_discover_tests(test_run)
find_package(Catch2 3 REQUIRED)
