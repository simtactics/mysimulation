cmake_minimum_required(VERSION 2.6)
project(iff2html)

include_directories(${LIBPNG_INCLUDE})
include_directories(${ZLIB_INCLUDE})

include_directories( "${CMAKE_SOURCE_DIR}/formats")

set(IFF2HTML_SOURCES
    iff2html.c
    md5.c
    image.c
    opngreduc.c
    "${CMAKE_SOURCE_DIR}/formats/iff/iff.h"
    "${CMAKE_SOURCE_DIR}/formats/bmp/read_bmp.c"
)

add_executable(iff2html ${IFF2HTML_SOURCES})
target_link_libraries(iff2html iff_static FileHandler_static ${LIBPNG_LINK} ${ZLIB_LINK})
