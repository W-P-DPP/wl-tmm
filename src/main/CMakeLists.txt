aux_source_directory(. SRCS)
add_executable(tmms_main ${SRCS})
install(TARGETS tmms_main RUNTIME DESTINATION sbin)