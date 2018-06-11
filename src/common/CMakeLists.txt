cmake_minimum_required( VERSION 3.0.2 )
project( Sapphire )

include( ExternalProject )

#############################################################################



#############################################################################

file(GLOB UTILS_SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Config/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Crypt/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Database/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Exd/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Logging/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Network/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Network/PacketDef/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Script/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Util/*.cpp")

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib )


add_library( common ${UTILS_PUBLIC_INCLUDE_FILES} ${UTILS_SOURCE_FILES} )

target_link_libraries( common
                       PUBLIC
                         xivdat
                         mysqlConnector
                         boost )

target_include_directories( common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/"
                                          "${MYSQLCONNECTOR_ROOT}/src/MysqlConnector/src/"
                                          "${XIVDAT_ROOT}/src/XivDat/src/"
                                          "${Boost_INCLUDE_DIR}" )
