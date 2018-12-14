#项目名称
project(code)
cmake_minimum_required(VERSION 3.10)
#代码路径
aux_source_directory(.DIR_TOOT_SRCS)
LINK_DIRECTORIES(
/usr/local/lib
/usr/lib
)
include_directories(
	/usr/local/include
	/wf/linux/include
 )
#dubug 模式
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -g")
#生成可执行的文件
add_executable(code ${DIR_TOOT_SRCS})
