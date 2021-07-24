file(GLOB sources_localdir *.cpp *.h)


set(ClientTest_SRCS
	${sources_localdir}
	${sources_Debugging}
)

include_directories(
	${CMAKE_SOURCE_DIR}/Src/ClientTest
	${MYSQL_INCLUDE_DIR}
)

add_executable(ClientTest
	${ClientTest_SRCS}
)