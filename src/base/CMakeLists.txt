aux_source_directory(. DIR_LIB_SRCS)
add_library (lib_tmms_base ${DIR_LIB_SRCS})



aux_source_directory(. SRCS)
add_executable(tmms_base ${SRCS})
install(TARGETS tmms_base RUNTIME DESTINATION sbin)