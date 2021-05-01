# cmake总结 
    官网地址：https://cmake.org/
# 一、常用的简单的命令解析

## 指定cmake最小版本

- cmake_minimum_required(VERSION 3.16.3)  

## 设置项目名称

* project(MyProject)  

## 生成可执行文件

* add_executable(main main.cpp) 

## 生成静态库

* add_library(common STATIC test.cpp) 

## 生成动态库或共享库

* add_library(common SHARED test.cpp)
1. linux 

        libtest.a       静态库
        libtest.so      动态库
2. windows

        test.lib        静态库
        test.dll        动态库

## 明确指定包含的源文件

* add_library(demo demo.cpp test.cpp util.cpp)

## 搜素所有的cpp文件

* aux_source_directory(. SRC_LIST) # 搜索当前目录下的所有.cpp文件
* add_library(demo ${SRC_LIST})

## 自定义搜索规则

- file(GLOB SRC_LIST "*.cpp" "protocol/*.cpp")

- add_library(demo ${SRC_LIST})

  ##### 或者

- file(GLOB SRC_LIST "*.cpp")*

- file(GLOB SRC_PROTOCOL_LIST "protocol/*\.cpp")

- add_library(demo ${SRC_LIST} ${SRC_PROTOCOL_LIST})

  ##### 或者

- aux_source_directory(. SRC_LIST)

- aux_source_directory(protocol SRC_PROTOCOL_LIST)

- add_library(demo ${SRC_LIST} ${SRC_PROTOCOL_LIST})

## 查找指定的库文件

find_library(VAR name path)查找到指定的预编译库，并将它的路径存储在变量中。
默认的搜索路径为 cmake 包含的系统库，因此如果是 NDK 的公共库只需要指定库的 name 即可。

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
类似的命令还有 find_file()、find_path()、find_program()、find_package()。

设置包含的目录
include_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}/include
)
Linux 下还可以通过如下方式设置包含的目录

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")

设置链接库搜索目录
link_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}/libs
)
Linux 下还可以通过如下方式设置包含的目录

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_CURRENT_SOURCE_DIR}/libs")

设置 target 需要链接的库
target_link_libraries( # 目标库
                    demo

```
                # 目标库需要链接的库
                # log-lib 是上面 find_library 指定的变量名
                ${log-lib} )
```
在 Windows 下，系统会根据链接库目录，搜索xxx.lib 文件，Linux 下会搜索 xxx.so 或者 xxx.a 文件，如果都存在会优先链接动态库（so 后缀）。

8.1 指定链接动态库或静态库

target_link_libraries(demo libface.a) # 链接libface.a
target_link_libraries(demo libface.so) # 链接libface.so
8.2 指定全路径

target_link_libraries(demo ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.a)
target_link_libraries(demo ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.so)
8.3 指定链接多个库

target_link_libraries(demo
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/libface.a
    boost_system.a
    boost_thread
    pthread)

设置变量
9.1 set 直接设置变量的值

set(SRC_LIST main.cpp test.cpp)
add_executable(demo ${SRC_LIST})
9.2 set 追加设置变量的值

set(SRC_LIST main.cpp)
set(SRC_LIST ${SRC_LIST} test.cpp)
add_executable(demo ${SRC_LIST})
9.3 list 追加或者删除变量的值

set(SRC_LIST main.cpp)
list(APPEND SRC_LIST test.cpp)
list(REMOVE_ITEM SRC_LIST main.cpp)
add_executable(demo ${SRC_LIST})
10. 条件控制
10.1 if…elseif…else…endif

逻辑判断和比较：
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

数字比较：
if (variable LESS number)：LESS 小于
if (string LESS number)
if (variable GREATER number)：GREATER 大于
if (string GREATER number)
if (variable EQUAL number)：EQUAL 等于
if (string EQUAL number)

字母表顺序比较：
if (variable STRLESS string)
if (string STRLESS string)
if (variable STRGREATER string)
if (string STRGREATER string)
if (variable STREQUAL string)
if (string STREQUAL string)
示例：

if(MSVC)
    set(LINK_LIBS common)
else()
    set(boost_thread boost_log.a boost_system.a)
endif()
target_link_libraries(demo ${LINK_LIBS})
# 或者
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



打印信息
message(${PROJECT_SOURCE_DIR})
message("build with debug mode")
message(WARNING "this is warnning message")
message(FATAL_ERROR "this build has many error") # FATAL_ERROR 会导致编译失败

包含其它 cmake 文件
include(./common.cmake) # 指定包含文件的全路径
include(def) # 在搜索路径中搜索def.cmake文件
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake) # 设置include的搜索路径


三、常用变量
1. 预定义变量
PROJECT_SOURCE_DIR：工程的根目录
PROJECT_BINARY_DIR：运行 cmake 命令的目录，通常是 ${PROJECT_SOURCE_DIR}/build
PROJECT_NAME：返回通过 project 命令定义的项目名称
CMAKE_CURRENT_SOURCE_DIR：当前处理的 CMakeLists.txt 所在的路径
CMAKE_CURRENT_BINARY_DIR：target 编译目录
CMAKE_CURRENT_LIST_DIR：CMakeLists.txt 的完整路径
CMAKE_CURRENT_LIST_LINE：当前所在的行
CMAKE_MODULE_PATH：定义自己的 cmake 模块所在的路径，SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)，然后可以用INCLUDE命令来调用自己的模块
EXECUTABLE_OUTPUT_PATH：重新定义目标二进制可执行文件的存放位置
LIBRARY_OUTPUT_PATH：重新定义目标链接库文件的存放位置

2. 环境变量
使用环境变量

 $ENV{Name}
写入环境变量

 set(ENV{Name} value) # 这里没有“$”符号
3. 系统信息
­CMAKE_MAJOR_VERSION：cmake 主版本号，比如 3.4.1 中的 3
­CMAKE_MINOR_VERSION：cmake 次版本号，比如 3.4.1 中的 4
­CMAKE_PATCH_VERSION：cmake 补丁等级，比如 3.4.1 中的 1
­CMAKE_SYSTEM：系统名称，比如 Linux-­2.6.22
­CMAKE_SYSTEM_NAME：不包含版本的系统名，比如 Linux
­CMAKE_SYSTEM_VERSION：系统版本，比如 2.6.22
­CMAKE_SYSTEM_PROCESSOR：处理器名称，比如 i686
­UNIX：在所有的类 UNIX 平台下该值为 TRUE，包括 OS X 和 cygwin
­WIN32：在所有的 win32 平台下该值为 TRUE，包括 cygwin

4. 主要开关选项
BUILD_SHARED_LIBS：这个开关用来控制默认的库编译方式，如果不进行设置，使用 add_library 又没有指定库类型的情况下，默认编译生成的库都是静态库。如果 set(BUILD_SHARED_LIBS ON) 后，默认生成的为动态库
CMAKE_C_FLAGS：设置 C 编译选项，也可以通过指令 add_definitions() 添加
CMAKE_CXX_FLAGS：设置 C++ 编译选项，也可以通过指令 add_definitions() 添加

add_definitions(-DENABLE_DEBUG -DABC) # 参数之间用空格分隔

--------------------------------------------------------------------
# 目录结构：
* 1.signalFile  
    最简单的例子
* 2.multipleFileInSameDirectory  
    同一目录下，多个cpp文件案例
* 3.multipleFileInDifferentDirectory  
    多个目录下，多个cpp文件
* 4.CustomCompilation  
    用户自定义编译
* 5.installAndTest
    安装和测试
* 6.gdb-environment-macro
    gdb调试 宏
* 7.addVersion
    添加版本信息
* 8.createInstallPack 
    创建发行包

