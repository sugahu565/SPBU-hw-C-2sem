add_library(hw4_files
   dijkstra.c
   dijkstra.h
   priorityQueue.c
   priorityQueue.h
   graph.c
   graph.h
   readFile.c
   readFile.h
)

add_executable(hw4 main.c)

target_link_libraries(hw4 hw4_files)

target_include_directories(hw4_files PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

