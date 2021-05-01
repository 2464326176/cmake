cmake_minimum_required(VERSION 3.16)
project(cmaketest)


set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
check_function_exists (mypow HAVE_POW)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)

option(USE_MYMATH "use provided math implementation" on)

if(USE_MYMATH)
    include_directories("${PROJECT_SOURCE_DIR}/math")
    add_subdirectory(math)
    set(extra_libs ${extra_Libs} MathFunctions)
endif(USE_MYMATH)

include_directories("${PROJECT_SOURCE_DIR}/math")

aux_source_directory(. DIR_SRCS)

add_executable(main ${DIR_SRCS})

target_link_libraries(main ${extra_libs})

install (TARGETS main DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/config.h" DESTINATION include)










# 启用测试
enable_testing()

# 测试程序是否成功运行
add_test (test_run main 5 2)

macro (do_test arg1 arg2 result)
  add_test (test_${arg1}_${arg2} main ${arg1} ${arg2})
  set_tests_properties (test_${arg1}_${arg2}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)

do_test (2 2 "4")
do_test (2 3 "8")