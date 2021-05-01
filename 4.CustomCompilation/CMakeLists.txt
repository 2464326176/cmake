project(cmaketest)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

option(use_mymath "use provided math implementation" on)

if(use_mymath)
    include_directories("${PROJECT_SOURCE_DIR}/math")
    add_subdirectory(math)
    set(extra_libs ${extra_Libs} MathFunctions)
endif(use_mymath)

include_directories("${PROJECT_SOURCE_DIR}/math")

aux_source_directory(. DIR_SRCS)

add_executable(main ${DIR_SRCS})

target_link_libraries(main ${extra_libs})