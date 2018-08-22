
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -pthread")
else()
  add_definitions(-D_WIN32_WINNT=0x601)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
  if(${CMAKE_SIZEOF_VOID_P} EQUAL "8")
    set (PROJECTNAME_ARCHITECTURE "x64")
  else ()
    set (PROJECTNAME_ARCHITECTURE "x86")
  endif ()    # disabling SAFESEH

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")


    message(STATUS "Disabling Safe Exception Handlers..")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")

    # edit and continue
    message(STATUS "Enabling Edit and Continue..")
    add_definitions(/ZI)

    # incremental linking
    message(STATUS "Enabling Incremental Linking..")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /INCREMENTAL")

    # enable building with multiple processes
    message(STATUS "Enabling Build with Multiple Processes..")
    add_definitions(/MP)
  endif()
endif()


