file(GLOB sources_localdir *.cpp *.h)


set(LibNetWork_SRCS
	${sources_localdir}
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/LibNetwork
	${CMAKE_SOURCE_DIR}/Src/Shared
	${CMAKE_SOURCE_DIR}/Src/Shared/Zip
	${CMAKE_SOURCE_DIR}/Src/Shared/Zip/ZLib
	${MYSQL_INCLUDE_DIR}
)

add_library(LibNetwork
	${LibNetWork_SRCS}
)

target_link_libraries(LibNetwork
	LibShared
)