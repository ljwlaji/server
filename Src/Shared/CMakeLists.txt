file(GLOB sources_localdir 			*.cpp *.h)
file(GLOB sources_db 				DB/*.cpp DB/*.h)

set(Shared_SRCS
	${sources_localdir}
	${sources_db}
)

include_directories(
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/Shared
	${MYSQL_INCLUDE_DIR}
)

add_library(LibShared STATIC
	${Shared_SRCS}
)