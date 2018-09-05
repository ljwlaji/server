file(GLOB sources_localdir 			*.cpp *.h)

set(DataBase_SRCS
	${sources_localdir}
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/DataBase
	${CMAKE_SOURCE_DIR}/Src/Shared
	${MYSQL_INCLUDE_DIR}
)

add_library(DataBase STATIC
	${DataBase_SRCS}
)

target_link_libraries(DataBase
	LibShared
)
