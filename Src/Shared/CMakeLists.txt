file(GLOB sources_localdir *.cpp *.h)


set(ServerLog_SRCS
	${sources_localdir}
)

include_directories(
	${CMAKE_SOURCE_DIR}/Src/Shared
)

add_library(LibShared STATIC
	${ServerLog_SRCS}
)