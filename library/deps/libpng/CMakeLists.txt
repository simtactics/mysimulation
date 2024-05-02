cmake_minimum_required(VERSION 2.6)
project(libpng)

set(LIBPNG_SERIES 1)
set(LIBPNG_MAJOR  5)
set(LIBPNG_MINOR  11)

set(LIBPNG_SOURCES
    png.c
    pngerror.c
    pngget.c
    pngmem.c
    pngpread.c
    pngread.c
    pngrio.c
    pngrtran.c
    pngrutil.c
    pngset.c
    pngtrans.c
    pngwio.c
    pngwrite.c
    pngwtran.c
    pngwutil.c
)

include_directories(${ZLIB_INCLUDE})

add_library(libpng_static STATIC ${LIBPNG_SOURCES})
set_target_properties(libpng_static PROPERTIES
    OUTPUT_NAME "png${LIBPNG_SERIES}${LIBPNG_MAJOR}"
    PREFIX "lib"
    IMPORT_PREFIX "lib"
    CLEAN_DIRECT_OUTPUT 1)

#### Shared library (uncomment to build)
#add_library(libpng_shared SHARED ${LIBPNG_SOURCES})
#set_target_properties(libpng_shared PROPERTIES
#    OUTPUT_NAME "png${LIBPNG_SERIES}${LIBPNG_MAJOR}"
#    VERSION ${LIBPNG_SERIES}.${LIBPNG_MAJOR}.${LIBPNG_MINOR}
#    PREFIX "lib"
#    IMPORT_PREFIX "lib"
#    CLEAN_DIRECT_OUTPUT 1)
#target_link_libraries(libpng_shared zlib_shared m)