
add_executable(compilation_time compilation_time.cpp)
target_link_libraries(compilation_time nbre_rt)

add_executable(read_write_time read_write_time.cpp)
target_link_libraries(read_write_time nbre_rt)

add_executable(execution_time execution_time.cpp)
target_link_libraries(execution_time nbre_rt)

