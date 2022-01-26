cmake_minimum_required(VERSION 3.1)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(PROJECT_NAME "DpllSolver")
project("${PROJECT_NAME}" LANGUAGES C)


#   ____                             ____
#  / ___|   ___    _ __    _   _    |  _ \    ___   ___    ___    _   _   _ __    ___    ___   ___
# | |      / _ \  | '_ \  | | | |   | |_) |  / _ \ / __|  / _ \  | | | | | '__|  / __|  / _ \ / __|
# | |___  | (_) | | |_) | | |_| |   |  _ <  |  __/ \__ \ | (_) | | |_| | | |    | (__  |  __/ \__ \
#  \____|  \___/  | .__/   \__, |   |_| \_\  \___| |___/  \___/   \__,_| |_|     \___|  \___| |___/
#                 |_|      |___/

# Create a target that can be a dependency to all tests.
# If there is no test this target is never build.

set(TEST_RESOURCES_SRC "${CMAKE_CURRENT_LIST_DIR}/res")
set(TEST_RESOURCES_DST "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res")
add_custom_target(Test_Resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${TEST_RESOURCES_SRC}" "${TEST_RESOURCES_DST}")
message("${PROJECT_NAME}: 'Test_Resources' added")

#     _          _       _     ____            _           _   _
#    / \      __| |   __| |   / ___|   _   _  | |__     __| | (_)  _ __   ___
#   / _ \    / _` |  / _` |   \___ \  | | | | | '_ \   / _` | | | | '__| / __|
#  / ___ \  | (_| | | (_| |    ___) | | |_| | | |_) | | (_| | | | | |    \__ \
# /_/   \_\  \__,_|  \__,_|   |____/   \__,_| |_.__/   \__,_| |_| |_|    |___/
#

add_subdirectory(core)
add_subdirectory(tool)

option(BUILD_TESTING "Build Tests" ON)
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory(test)
endif ()
