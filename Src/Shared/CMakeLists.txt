file(GLOB sources_localdir 			*.cpp *.h)
file(GLOB sources_json 				jsoncpp/*.cpp jsoncpp/*.h)
file(GLOB sources_json_include 		jsoncpp/json/*.cpp jsoncpp/json/*.h)
file(GLOB sources_db 				DB/*.cpp DB/*.h)

set(Shared_SRCS
	${sources_localdir}
	#${sources_json}
	#${sources_json_include}
	${sources_db}
)

include_directories(
	${CMAKE_SOURCE_DIR}/Src
	${CMAKE_SOURCE_DIR}/Src/Shared
	${CMAKE_SOURCE_DIR}/Src/Shared/jsoncpp
	${CMAKE_SOURCE_DIR}/Src/Shared/jsoncpp/json
	${MYSQL_INCLUDE_DIR}
)

add_library(LibShared STATIC
	${Shared_SRCS}
)