add_library(doctest STATIC
    doctest.hpp
    doctest_main.cpp
)

add_executable(test-cmd_line_util t-cmd_line_util.cpp)
target_include_directories(test-cmd_line_util PRIVATE  ../code/confy)
target_link_libraries(test-cmd_line_util doctest)
add_test(NAME cmd_line_util COMMAND $<TARGET_FILE:test-cmd_line_util>)
