cmake_minimum_required( VERSION 3.0.2 )
project( MysqlConnector )

list( APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" )

include( cmake/compiler.cmake )

find_package( MySQL )

add_subdirectory( src )
