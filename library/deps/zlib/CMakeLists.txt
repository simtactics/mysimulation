cmake_minimum_required(VERSION 2.6)
project(zlib)

set(ZLIB_SERIES 1)
set(ZLIB_MAJOR 2)
set(ZLIB_MINOR 7)

set(ZLIB_SOURCES
    adler32.c
    compress.c
    crc32.c
    deflate.c
    gzclose.c
    gzlib.c
    gzread.c
    gzwrite.c
    inflate.c
    infback.c
    inftrees.c
    inffast.c
    trees.c
    uncompr.c
    zutil.c
)

add_library(zlib_static STATIC ${ZLIB_SOURCES})
set_target_properties(zlib_static PROPERTIES
    OUTPUT_NAME "z"
    CLEAN_DIRECT_OUTPUT 1)

#### Shared library (uncomment to build)
#add_library(zlib_shared SHARED ${ZLIB_SOURCES})
#if(WIN32)
#    set_target_properties(zlib_shared PROPERTIES
#        OUTPUT_NAME "zlib${ZLIB_SERIES}"
#        PREFIX ""
#        IMPORT_PREFIX "")
#else()
#    set_target_properties(zlib_shared PROPERTIES OUTPUT_NAME "z")
#endif()
#set_target_properties(zlib_shared PROPERTIES
#    VERSION ${ZLIB_SERIES}.${ZLIB_MAJOR}.${ZLIB_MINOR}
#    CLEAN_DIRECT_OUTPUT 1)