file(GLOB sources_localdir *.cpp *.h)


set(ServerLog_SRCS
	${sources_localdir}
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/LibNetwork
	${CMAKE_SOURCE_DIR}/Src/Shared
	${MYSQL_INCLUDE_DIR}
)

add_executable(LibNetwork
	${ServerLog_SRCS}
)

target_link_libraries(LibNetwork
	LibShared
)