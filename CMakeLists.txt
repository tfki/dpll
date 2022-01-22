cmake_minimum_required(VERSION 3.1)

set(PROJECT_NAME "DpllSolver")
project("${PROJECT_NAME}" LANGUAGES C)

add_subdirectory(core)
add_subdirectory(tool)

option(BUILD_TESTING "Build Tests" ON)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()
