cmake_minimum_required(VERSION 3.1)

#  _____                _
# |_   _|   ___   ___  | |_   ___
#   | |    / _ \ / __| | __| / __|
#   | |   |  __/ \__ \ | |_  \__ \
#   |_|    \___| |___/  \__| |___/
#

# macro for converting snake_case to Pascal_Snake_Case
# which is not trivial in cmake as cmake regex does not
# support case replacements
macro(snake_case_to_pascal_snake_case OUTPUT_VARIABLE INPUT)
    set(${OUTPUT_VARIABLE} ${INPUT})
    foreach (LOWER_LETTER a b c d e f g h i j k l m n o p q r s t u v w x y z)
        string(TOUPPER ${LOWER_LETTER} UPPER_LETTER)
        string(REGEX REPLACE "^${LOWER_LETTER}" "${UPPER_LETTER}" ${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}})
        string(REGEX REPLACE "_${LOWER_LETTER}" "_${UPPER_LETTER}" ${OUTPUT_VARIABLE} ${${OUTPUT_VARIABLE}})
    endforeach ()
endmacro()

set(TEST_COUNT 0)
set(TEST_LINK_LIBRARIES Cnf Solver)

file(GLOB TEST_SOURCE *.c)

message("${PROJECT_NAME}: Scanning Tests")

foreach (TEST_SOURCE ${TEST_SOURCE})

    # create the test name from test file and project name
    get_filename_component(TEST_NAME "${TEST_SOURCE}" NAME_WE)
    snake_case_to_pascal_snake_case(TEST_NAME ${TEST_NAME})

    # create executable for the test
    add_executable("${TEST_NAME}")
    target_sources("${TEST_NAME}" PRIVATE "${TEST_SOURCE}")
    target_link_libraries("${TEST_NAME}" PRIVATE "${TEST_LINK_LIBRARIES}")
    set_target_properties("${TEST_NAME}" PROPERTIES
            LINKER_LANGUAGE C
            C_STANDARD 99
            C_STANDARD_REQUIRED ON)

    # register executable as test
    add_test(NAME "${TEST_NAME}" COMMAND $<TARGET_FILE:${TEST_NAME}>)

    set(TEST_COUNT "${TEST_COUNT}+1")
    message("${PROJECT_NAME}: Test '${TEST_NAME}' added")

endforeach ()

math(EXPR TEST_COUNT "${TEST_COUNT}")
message("${PROJECT_NAME}: Total of ${TEST_COUNT} Test(s) added")
