file(GLOB sources_localdir *.cpp *.h)


set(LoginServer_SRCS
	${sources_localdir}
	${sources_Debugging}
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/LoginServer
	${CMAKE_SOURCE_DIR}/Src/LibNetwork
	${CMAKE_SOURCE_DIR}/Src/Shared
	${MYSQL_INCLUDE_DIR}
)

add_executable(LoginServer
	${LoginServer_SRCS}
)

target_link_libraries(LoginServer
	LibShared
	LibNetwork
)