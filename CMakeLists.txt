cmake_minimum_required(VERSION 3.0)
project(Hans)


include(CheckCXXSourceRuns)
include(CheckIncludeFiles)



# set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	set(CMAKE_CXX_FLAGS "-std=c++11 -pthread")
endif (CMAKE_SYSTEM_NAME MATCHES "Linux")
# set default buildoptions and print them
include(cmake/options.cmake)

find_package(MySQL REQUIRED)


SET(EXECUTABLE_OUTPUT_PATH 	${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH 	${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(Src)