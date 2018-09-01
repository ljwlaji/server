file(GLOB sources_localdir *.cpp *.h)


set(WorldServer_SRCS
	${sources_localdir}
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/WorldServer
	${CMAKE_SOURCE_DIR}/Src/LibNetwork
	${CMAKE_SOURCE_DIR}/Src/Shared
	${MYSQL_INCLUDE_DIR}
)

add_executable(WorldServer
	${WorldServer_SRCS}
)

target_link_libraries(WorldServer
	LibShared
	LibNetwork
)