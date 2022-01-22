cmake_minimum_required(VERSION 3.1)

# cnf lib

set(TARGET_NAME Cnf)
set(TARGET_INCLUDE_DIRECTORIES cnf)
set(TARGET_LINK_LIBRARIES )

file(GLOB_RECURSE TARGET_HEADER cnf/*.h)
file(GLOB_RECURSE TARGET_SOURCE cnf/*.c)

add_library("${TARGET_NAME}" STATIC)
target_sources("${TARGET_NAME}" PUBLIC ${TARGET_HEADER})
target_sources("${TARGET_NAME}" PRIVATE ${TARGET_SOURCE})
target_link_libraries("${TARGET_NAME}" PUBLIC ${TARGET_LINK_LIBRARIES})
target_include_directories("${TARGET_NAME}" PUBLIC ${TARGET_INCLUDE_DIRECTORIES})
set_target_properties("${TARGET_NAME}" PROPERTIES
        LINKER_LANGUAGE C
        C_STANDARD 99
        C_STANDARD_REQUIRED ON)

message("${PROJECT_NAME}: Static Library '${TARGET_NAME}' added")

# solver lib

set(TARGET_NAME Solver)
set(TARGET_INCLUDE_DIRECTORIES solver)
set(TARGET_LINK_LIBRARIES Cnf)

file(GLOB_RECURSE TARGET_HEADER solver/*.h)
file(GLOB_RECURSE TARGET_SOURCE solver/*.c)

add_library("${TARGET_NAME}" STATIC)
target_sources("${TARGET_NAME}" PUBLIC ${TARGET_HEADER})
target_sources("${TARGET_NAME}" PRIVATE ${TARGET_SOURCE})
target_link_libraries("${TARGET_NAME}" PUBLIC ${TARGET_LINK_LIBRARIES})
target_include_directories("${TARGET_NAME}" PUBLIC ${TARGET_INCLUDE_DIRECTORIES})
set_target_properties("${TARGET_NAME}" PROPERTIES
        LINKER_LANGUAGE C
        C_STANDARD 99
        C_STANDARD_REQUIRED ON)

message("${PROJECT_NAME}: Static Library '${TARGET_NAME}' added")
