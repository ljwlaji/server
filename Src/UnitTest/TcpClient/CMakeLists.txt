file(GLOB sources_localdir *.cpp *.h *.hpp)

set(SocketClient_SRCS
	${sources_localdir}
)

add_executable(SocketClient
	${SocketClient_SRCS}
)
