# cmake总结 
    官网地址：https://cmake.org/
    文档参考地址：https://blog.csdn.net/afei__/article/details/81201039

# 一、常见变量

## 预定义变量

```CMAKE
PROJECT_SOURCE_DIR：工程的根目录
PROJECT_BINARY_DIR：运行 cmake 命令的目录，通常是 ${PROJECT_SOURCE_DIR}/build
PROJECT_NAME：返回通过 project 命令定义的项目名称
CMAKE_CURRENT_SOURCE_DIR：当前处理的 CMakeLists.txt 所在的路径
CMAKE_CURRENT_BINARY_DIR：target 编译目录
CMAKE_CURRENT_LIST_DIR：CMakeLists.txt 的完整路径
CMAKE_CURRENT_LIST_LINE：当前所在的行
CMAKE_MODULE_PATH：定义自己的 cmake 模块所在的路径，SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)，然后用INCLUDE命令来调用自己的模块
EXECUTABLE_OUTPUT_PATH：重新定义目标二进制可执行文件的存放位置
LIBRARY_OUTPUT_PATH：重新定义目标链接库文件的存放位置
```

## 环境变量

使用环境变量

```
 $ENV{Name}
```

写入环境变量

```
 set(ENV{Name} value) # 这里没有“$”符号
```

## 系统信息

```CMAKE
CMAKE_MAJOR_VERSION：cmake 主版本号，比如 3.4.1 中的 3
CMAKE_MINOR_VERSION：cmake 次版本号，比如 3.4.1 中的 4
CMAKE_PATCH_VERSION：cmake 补丁等级，比如 3.4.1 中的 1
CMAKE_SYSTEM：系统名称，比如 Linux-­2.6.22
CMAKE_SYSTEM_NAME：不包含版本的系统名，比如 Linux
CMAKE_SYSTEM_VERSION：系统版本，比如 2.6.22
CMAKE_SYSTEM_PROCESSOR：处理器名称，比如 i686
UNIX：在所有的类 UNIX 平台下该值为 TRUE，包括 OS X 和 cygwin
WIN32：在所有的 win32 平台下该值为 TRUE，包括 cygwin

主要开关选项
BUILD_SHARED_LIBS：这个开关用来控制默认的库编译方式，如果不进行设置，使用 add_library 又没有指定库类型的情况下，默认编译生成的库都是静态库。如果 set(BUILD_SHARED_LIBS ON) 后，默认生成的为动态库
CMAKE_C_FLAGS：设置 C 编译选项，也可以通过指令 add_definitions() 添加
CMAKE_CXX_FLAGS：设置 C++ 编译选项，也可以通过指令 add_definitions() 添加
add_definitions(-DENABLE_DEBUG -DABC) # 参数之间用空格分隔
```
# 二、常用的简单的命令解析

## 指定cmake最小版本

```CMAKE
cmake_minimum_required(VERSION 3.16.3)  
```

## 设置项目名称

```CMAKE
project(MyProject)  
```

## 生成可执行文件

```CMAKE
add_executable(main main.cpp) 
```

## 生成静态库

```CMAKE
add_library(common STATIC test.cpp) 
```

## 生成动态库或共享库

```CMAKE
add_library(common SHARED test.cpp)

linux 
   libtest.a       静态库
   libtest.so      动态库
windows
   test.lib        静态库
   test.dll        动态库
```

## 明确指定包含的源文件

```CMAKE
add_library(demo demo.cpp test.cpp util.cpp)
```

## 搜素所有的cpp文件

```CMAKE
aux_source_directory(. SRC_LIST) # 搜索当前目录下的所有.cpp文件
add_library(demo ${SRC_LIST})
```

## 自定义搜索规则

```CMAKE
file(GLOB SRC_LIST "*.cpp" "protocol/*.cpp")
add_library(demo ${SRC_LIST})
```

或者

```CMAKE
file(GLOB SRC_LIST "*.cpp")*
file(GLOB SRC_PROTOCOL_LIST "protocol/*\.cpp")
add_library(demo ${SRC_LIST} ${SRC_PROTOCOL_LIST})
```

或者

```CMAKE
aux_source_directory(. SRC_LIST)
aux_source_directory(protocol SRC_PROTOCOL_LIST)
add_library(demo ${SRC_LIST} ${SRC_PROTOCOL_LIST})
```



## 查找指定的库文件

find_library(VAR name path)查找到指定的预编译库，并将它的路径存储在变量中。
默认的搜索路径为 cmake 包含的系统库，因此如果是 NDK 的公共库只需要指定库的 name 即可。

```
find_library( # Sets the name of the path variable.log-lib
          # Specifies the name of the NDK library that
          # you want CMake to locate.log )
```

类似的命令还有 find_file()、find_path()、find_program()、find_package()。

设置包含的目录

```
include_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}/include
)
```

Linux 下还可以通过如下方式设置包含的目录

```
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")
```

## 设置链接库搜索目录

```
link_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}/libs
)
```

Linux 下还可以通过如下方式设置包含的目录

```
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR}/libs")
```

## 设置 target 需要链接的库

```
target_link_libraries( # 目标库 demo
        # 目标库需要链接的库
        # log-lib 是上面 find_library 指定的变量名
​        ${log-lib} )
```

在 Windows 下，系统会根据链接库目录，搜索xxx.lib 文件，Linux 下会搜索 xxx.so 或者 xxx.a 文件，如果都存在会优先链接动态库（so 后缀）。

## 指定链接动态库或静态库

```CMAKE
target_link_libraries(demo libface.a) # 链接libface.a
target_link_libraries(demo libface.so) # 链接libface.so
指定全路径
target_link_libraries(demo ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.a)
target_link_libraries(demo ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.so)
指定链接多个库
target_link_libraries(demo
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.a
    boost_system.a
    boost_thread
    pthread)
```

## 设置变量

```CMAKE
set 直接设置变量的值

set(SRC_LIST main.cpp test.cpp)
add_executable(demo ${SRC_LIST})
 set 追加设置变量的值

set(SRC_LIST main.cpp)
set(SRC_LIST ${SRC_LIST} test.cpp)
add_executable(demo ${SRC_LIST})
list 追加或者删除变量的值

set(SRC_LIST main.cpp)
list(APPEND SRC_LIST test.cpp)
list(REMOVE_ITEM SRC_LIST main.cpp)
add_executable(demo ${SRC_LIST})
```

## 条件控制

if…elseif…else…endif

## 逻辑判断和比较：

```CMAKE
if (expression)：expression 不为空（0,N,NO,OFF,FALSE,NOTFOUND）时为真
if (not exp)：与上面相反
if (var1 AND var2)
if (var1 OR var2)
if (COMMAND cmd)：如果 cmd 确实是命令并可调用为真
if (EXISTS dir) if (EXISTS file)：如果目录或文件存在为真
if (file1 IS_NEWER_THAN file2)：当 file1 比 file2 新，或 file1/file2 中有一个不存在时为真，文件名需使用全路径
if (IS_DIRECTORY dir)：当 dir 是目录时为真
if (DEFINED var)：如果变量被定义为真
if (var MATCHES regex)：给定的变量或者字符串能够匹配正则表达式 regex 时为真，此处 var 可以用 var 名，也可以用 ${var}
if (string MATCHES regex)
```



## 数字比较：

```CMAKE
if (variable LESS number)：LESS 小于
if (string LESS number)
if (variable GREATER number)：GREATER 大于
if (string GREATER number)
if (variable EQUAL number)：EQUAL 等于
if (string EQUAL number)
```

字母表顺序比较：

```
if (variable STRLESS string)
if (string STRLESS string)
if (variable STRGREATER string)
if (string STRGREATER string)
if (variable STREQUAL string)
if (string STREQUAL string)
```

示例：

```CMAKE
if(MSVC)
    set(LINK_LIBS common)
else()
    set(boost_thread boost_log.a boost_system.a)
endif()
target_link_libraries(demo ${LINK_LIBS})
```

或者

```CMAKE
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive -g")
else()
    add_definitions(-D_SCL_SECURE_NO_WARNINGS
    D_CRT_SECURE_NO_WARNINGS
    -D_WIN32_WINNT=0x601
    -D_WINSOCK_DEPRECATED_NO_WARNINGS)
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "debug")
    ...
else()
    ...
endif()
10.2 while…endwhile

while(condition)
    ...
endwhile()
10.3 foreach…endforeach

foreach(loop_var RANGE start stop [step])
    ...
endforeach(loop_var)
start 表示起始数，stop 表示终止数，step 表示步长，示例：

foreach(i RANGE 1 9 2)
    message(${i})
endforeach(i)
```

## 打印信息

```CMAKE
message(${PROJECT_SOURCE_DIR})
message("build with debug mode")
message(WARNING "this is warnning message")
message(FATAL_ERROR "this build has many error") # FATAL_ERROR 会导致编译失败
```

## 包含其它 cmake 文件

```CMAKE
include(./common.cmake) # 指定包含文件的全路径
include(def) # 在搜索路径中搜索def.cmake文件
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # 设置include的搜索路径
```

--------------------------------------------------------------------
# 三、代码目录结构：

## 最简单的例子

1.signalFile  

├── CMakeLists.txt
└── main.cpp

CMakeLists.txt

```
project(cmaketest)  			#项目名字
set(src_list main.cpp)			#引入源文件 指定名字
add_executable(main ${src_list})#mian是可执行文件名字  注意main和cmaketest可以相同 也可以不一样
```

为了生成不混乱，我们这样

```shell cmake
mkdir build 
cd build 
cmake ..
make
./main
```

## 同一目录下，多个cpp文件案例

2.multipleFileInSameDirectory  

├── CMakeLists.txt
├── main.cpp
├── MyMath.cpp
└── MyMath.h

CMakeLists.txt

```CMAKE
project(cmaketest)
aux_source_directory(. DIR_SRCS)#引入源文件 泛指文件
add_executable(main ${DIR_SRCS})
```




## 多个目录下，多个cpp文件

3.multipleFileInDifferentDirectory  

├── CMakeLists.txt
├── main.cpp
└── math
    ├── CMakeLists.txt
    ├── MyMath.cpp
    └── MyMath.h

 根目录 CMakeLists.txt

```
project(cmaketest)	
include_directories("${PROJECT_SOURCE_DIR}/math") #头文件位置
aux_source_directory(. DIR_SRCS) 
add_subdirectory(math)	#添加子目录位置
add_executable(main ${DIR_SRCS})
target_link_libraries(main MathFunctions) #链接子目录动态库
```

 子目录CMakeLists.txt

```
aux_source_directory(. DIR_LIB_SRCS)
add_library(MathFunctions ${DIR_LIB_SRCS})#子目录生成动态库
```



## 用户自定义编译

4.CustomCompilation  

├── CMakeLists.txt
├── config.h.in
├── main.cpp
└── math
    ├── CMakeLists.txt
    ├── MyMath.cpp
    └── MyMath.h

config.h.in

```CMAKE
#cmakedefine use_mymath #cmake 宏定义
```

 根目录 CMakeLists.txt

```cmake
project(cmaketest)

configure_file(
    "${PROJECT_SOURCE_DIR}/config.h.in"
    "${PROJECT_BINARY_DIR}/config.h"
)	#当执行 camke 会在build目录生成config.h

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
```

 子目录CMakeLists.txt

```
aux_source_directory(. DIR_LIB_SRCS)
add_library(MathFunctions ${DIR_LIB_SRCS})
```



## 安装和测试

5.installAndTest

├── CMakeLists.txt
├── config.h.in
├── main.cpp
└── math
    ├── CMakeLists.txt
    ├── MyMath.cpp
    └── MyMath.h

 根目录 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(cmaketest)

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

do_test (5 2 "25")
do_test (10 5 "100000")
do_test (2 10 "1024")
```



执行如下指令

mkdir build 

cd build 

ccmake .. #进入交互界面

![image-20210502034356864](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210502034356864.png)

make

make test

![image-20210502034616484](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210502034616484.png)

make install

![image-20210502035209879](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210502035209879.png)

## gdb调试 宏

6.gdb-environment-macro

 根目录 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(cmaketest)
#debug
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
```

## 添加版本信息

7.addVersion

├── CMakeLists.txt
├── config.h.in
├── main.cpp
└── math
    ├── CMakeLists.txt
    ├── MyMath.cpp
    └── MyMath.h

config.h.in

```CMAKE
// the configured options and settings for Tutorial
#define Demo_VERSION_MAJOR @Demo_VERSION_MAJOR@
#define Demo_VERSION_MINOR @Demo_VERSION_MINOR@
```

 根目录 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(cmaketest)
#debug
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
```

![image-20210502035553538](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210502035553538.png)

## 创建发行包

8.createInstallPack 

├── CMakeLists.txt
├── config.h.in
├── License.txt
├── main.cpp
└── math
    ├── CMakeLists.txt
    ├── MyMath.cpp
    └── MyMath.h

License.txt 版权说明

 根目录 CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(cmaketest)
set (Demo_VERSION_MAJOR 1)
set (Demo_VERSION_MINOR 0)


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

# 构建一个 CPack 安装包
include (InstallRequiredSystemLibraries)
set (CPACK_RESOURCE_FILE_LICENSE
  "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${Demo_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Demo_VERSION_MINOR}")
include (CPack)
```

cpack -C CPackConfig.cmake #生成二进制安装包

cpack -C CPackSourceConfig.cmake #生成源码安装包

![image-20210502035948631](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210502035948631.png)

![image-20210502040202248](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210502040202248.png![image-20210502040247916](C:\Users\Administrator\AppData\Roaming\Typora\typora-user-images\image-20210502040247916.png)