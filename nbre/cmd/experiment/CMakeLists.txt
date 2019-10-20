
add_executable(compilation_time compilation_time.cpp)
target_link_libraries(compilation_time nbre_rt)

add_executable(execution_time execution_time.cpp)
target_link_libraries(execution_time nbre_rt)

add_executable(libnbre_small libnbre_small.cpp)
target_link_libraries(libnbre_small nbre_rt)

add_executable(libnbre_middle libnbre_middle.cpp)
target_link_libraries(libnbre_middle nbre_rt)

add_executable(libnbre_large libnbre_large.cpp)
target_link_libraries(libnbre_large nbre_rt)

add_executable(rw_time rw_time.cpp)
target_link_libraries(rw_time nbre_rt)
