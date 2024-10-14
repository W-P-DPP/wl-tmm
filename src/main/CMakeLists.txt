aux_source_directory(. SRCS)
add_executable(tmms ${SRCS})
target_link_libraries(tmms base jsoncpp.a)
install(TARGETS tmms RUNTIME DESTINATION sbin)