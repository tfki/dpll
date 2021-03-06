cmake_minimum_required(VERSION 3.1)

# Application executable

set(TARGET_NAME Application)
set(TARGET_LINK_LIBRARIES Common Cnf Solver)

file(GLOB_RECURSE TARGET_HEADER app/*.h)
file(GLOB_RECURSE TARGET_SOURCE app/*.c)

add_executable("${TARGET_NAME}")
target_sources("${TARGET_NAME}" PRIVATE ${TARGET_HEADER} ${TARGET_SOURCE})
target_link_libraries("${TARGET_NAME}" PRIVATE ${TARGET_LINK_LIBRARIES})
set_target_properties("${TARGET_NAME}" PROPERTIES
        LINKER_LANGUAGE C
        C_STANDARD 99
        C_STANDARD_REQUIRED ON)

message("${PROJECT_NAME}: Executable '${TARGET_NAME}' added")

# Scratch executable

set(TARGET_NAME Scratch)
set(TARGET_LINK_LIBRARIES Common Cnf Solver)

file(GLOB_RECURSE TARGET_HEADER scratch/*.h)
file(GLOB_RECURSE TARGET_SOURCE scratch/*.c)

add_executable("${TARGET_NAME}")
target_sources("${TARGET_NAME}" PRIVATE ${TARGET_HEADER} ${TARGET_SOURCE})
target_link_libraries("${TARGET_NAME}" PRIVATE ${TARGET_LINK_LIBRARIES})
set_target_properties("${TARGET_NAME}" PROPERTIES
        LINKER_LANGUAGE C
        C_STANDARD 99
        C_STANDARD_REQUIRED ON)

message("${PROJECT_NAME}: Executable '${TARGET_NAME}' added")

# Benchmark executable

set(TARGET_NAME Benchmark)
set(TARGET_LINK_LIBRARIES Common Cnf Solver)

file(GLOB_RECURSE TARGET_HEADER benchmark/*.h)
file(GLOB_RECURSE TARGET_SOURCE benchmark/*.c)

add_executable("${TARGET_NAME}")
target_sources("${TARGET_NAME}" PRIVATE ${TARGET_HEADER} ${TARGET_SOURCE})
target_link_libraries("${TARGET_NAME}" PRIVATE ${TARGET_LINK_LIBRARIES})
set_target_properties("${TARGET_NAME}" PROPERTIES
        LINKER_LANGUAGE C
        C_STANDARD 99
        C_STANDARD_REQUIRED ON)

message("${PROJECT_NAME}: Executable '${TARGET_NAME}' added")
