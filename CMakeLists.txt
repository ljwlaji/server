cmake_minimum_required(VERSION 3.0)
project(Hans)


include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

message("====== Current System ======")
message(${CMAKE_SYSTEM_NAME})
message("============================")


# set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
	set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
endif (CMAKE_SYSTEM_NAME MATCHES "Linux")
# set default buildoptions and print them
include(cmake/options.cmake)

find_package(MySQL REQUIRED)
find_package(Jsoncpp REQUIRED)

SET(EXECUTABLE_OUTPUT_PATH 	${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH 	${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(Src)

add_compile_options(-std=c++11)