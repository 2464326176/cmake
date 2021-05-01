project(cmaketest)

include_directories("${PROJECT_SOURCE_DIR}/math")

aux_source_directory(. DIR_SRCS)

add_subdirectory(math)

add_executable(main ${DIR_SRCS})

target_link_libraries(main MathFunctions)