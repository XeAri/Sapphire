cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0015 NEW)
project(Sapphire)

file(GLOB SERVER_PUBLIC_INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*")
file(GLOB SERVER_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}*.c*")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  "${CMAKE_CURRENT_SOURCE_DIR}/../../../bin/")


add_executable( sapphire_lobby
                ${SERVER_PUBLIC_INCLUDE_FILES}
                ${SERVER_SOURCE_FILES} )

target_link_libraries( sapphire_lobby
                       PUBLIC
                         common
                         boost
                         mysql
                         zlib )

target_include_directories( sapphire_lobby
                            PUBLIC
                              "${CMAKE_CURRENT_SOURCE_DIR}"
                              "${Boost_INCLUDE_DIR}" )

