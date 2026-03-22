add_executable(test_hw4 test_graph.c)

target_link_libraries(test_hw4 hw4_files)

add_test(NAME test_hw4 COMMAND test_hw4)

