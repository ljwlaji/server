file(GLOB sources_localdir 			*.cpp *.h)
file(GLOB sources_db 				DB/*.cpp DB/*.h)
file(GLOB sources_ZWarpper 			Zip/*.cpp Zip/*.h)
file(GLOB sources_ZLib				Zip/ZLib/*.c Zip/ZLib/*.h)

set(Shared_SRCS
	${sources_localdir}
	${sources_db}
	${sources_ZWarpper}
	${sources_ZLib}
)

include_directories(
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/Shared
	${CMAKE_SOURCE_DIR}/Src/Shared/Zip
	${CMAKE_SOURCE_DIR}/Src/Shared/Zip/ZLib
	${CMAKE_SOURCE_DIR}/Src/Shared/DB
)

add_library(LibShared STATIC
	${Shared_SRCS}
)
