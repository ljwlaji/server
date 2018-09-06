file(GLOB sources_localdir *.cpp *.h)
file(GLOB_RECURSE sources_RouterServer RouterServer/*.cpp RouterServer/*.h)


set(RouterServer_SRCS
	${sources_localdir}
	${sources_Debugging}
	${sources_RouterServer}
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/RouterServer
	${CMAKE_SOURCE_DIR}/Src/RouterServer/RouterServer
	${CMAKE_SOURCE_DIR}/Src/LibNetwork
	${CMAKE_SOURCE_DIR}/Src/Shared
	${MYSQL_INCLUDE_DIR}
)

add_executable(RouterServer
	${RouterServer_SRCS}
)

target_link_libraries(RouterServer
	LibShared
	LibNetwork
)