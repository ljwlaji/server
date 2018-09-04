file(GLOB sources_localdir *.cpp *.h)
file(GLOB json_dir jsoncpp/*.cpp jsoncpp/*.h)
file(GLOB json_include jsoncpp/json/*.cpp jsoncpp/json/*.h)

set(Shared_SRCS
	${sources_localdir}
	${json_dir}
	${json_include}
)

include_directories(
	${CMAKE_SOURCE_DIR}/Src/Shared
	${CMAKE_SOURCE_DIR}/Src/Shared/jsoncpp
	${CMAKE_SOURCE_DIR}/Src/Shared/jsoncpp/json
)

add_library(LibShared STATIC
	${Shared_SRCS}
)