cmake_minimum_required( VERSION 3.0.2 )
project( MysqlConnector )

file( GLOB UTILS_PUBLIC_INCLUDE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*" )
file( GLOB UTILS_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*" )

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

add_library( mysqlConnector ${UTILS_PUBLIC_INCLUDE_FILES} ${UTILS_SOURCE_FILES} )

target_include_directories( mysqlConnector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR} )

target_link_libraries( mysqlConnector mysql )

install( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/ DESTINATION lib USE_SOURCE_PERMISSIONS FILES_MATCHING PATTERN "*.lib*" )