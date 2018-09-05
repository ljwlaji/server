
set(sources_Debugging "")

if (WIN32)
    set(sources_Debugging
      ${CMAKE_SOURCE_DIR}/Src/Shared/WheatyExceptionReport.cpp
      ${CMAKE_SOURCE_DIR}/Src/Shared/WheatyExceptionReport.h
    )
else(WIN32)
  if (UNIX)
      set(sources_Debugging ${CMAKE_SOURCE_DIR}/Src/Shared/UnixDebugger.cpp)
  endif (UNIX)
endif(WIN32)

add_subdirectory(LibNetwork)
add_subdirectory(Shared)
add_subdirectory(UnitTest)
add_subdirectory(WorldServer)
add_subdirectory(LoginServer)