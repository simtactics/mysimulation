cmake_minimum_required(VERSION 2.6)
project(FileHandler)

set(FILEHANDLER_SERIES 0)
set(FILEHANDLER_MAJOR  0)
set(FILEHANDLER_MINOR  0)

if(64BIT)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fpic")
endif()

add_subdirectory(far)
add_subdirectory(iff)
add_subdirectory(utk)
add_subdirectory(xa)

set(FILEHANDLER_SOURCES
    #Audio.cpp
    File.cpp
    Image.cpp
    bmp/read_bmp.c
    cst/cst.c
    utk/read_utk.c
    #wav/read_wav.c
    xa/read_xa.c
)
if(WIN32)
    set(FILEHANDLER_SOURCES ${FILEHANDLER_SOURCES} resource.rc)
endif()

#include_directories(${FILEHANDLER_INCLUDE} ${LIBMPG123_INCLUDE} ${LIBJPEGTURBO_INCLUDE} ${LIBPNG_INCLUDE})
include_directories(${FILEHANDLER_INCLUDE} ${LIBJPEGTURBO_INCLUDE} ${LIBPNG_INCLUDE})
#TODO-JIP: hack, include vars are not set properly
include_directories( "../_deps/libjpeg-turbo" )

set(FILEHANDLER_LIB)
if(BUILD_SHARED_LIBS)
    # BUILD_SHARED_LIBS
    add_library(FileHandler_shared SHARED ${FILEHANDLER_SOURCES})
    if(WIN32)
        set_target_properties(FileHandler_shared PROPERTIES OUTPUT_NAME "FileHandler${FILEHANDLER_SERIES}")
    else()
        set_target_properties(FileHandler_shared PROPERTIES OUTPUT_NAME "FileHandler")
    endif()
    set_target_properties(FileHandler_shared PROPERTIES
        VERSION ${FILEHANDLER_SERIES}.${FILEHANDLER_MAJOR}.${FILEHANDLER_MINOR}
        PREFIX ""
        IMPORT_PREFIX ""
        CLEAN_DIRECT_OUTPUT 1)
    
    set(FILEHANDLER_LIB FileHandler_shared)
else()
    #### Static library (uncomment to build)
    add_library(FileHandler_static STATIC ${FILEHANDLER_SOURCES})
    set_target_properties(FileHandler_static PROPERTIES
        OUTPUT_NAME "FileHandler${FILEHANDLER_SERIES}"
        PREFIX ""
        CLEAN_DIRECT_OUTPUT 1)

    target_link_libraries(FileHandler_static far_static iff_static ${LIBJPEG_LINK} ${LIBMPG123_LINK} ${LIBPNG_LINK} ${ZLIB_LINK})

    set(FILEHANDLER_LIB FileHandler_static)
endif()
