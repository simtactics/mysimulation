cmake_minimum_required(VERSION 2.6)
project(freetype)

set(FREETYPE_SERIES 2)
set(FREETYPE_MAJOR 4)
set(FREETYPE_MINOR 10)

set(FREETYPE_SOURCES
    src/autofit/autofit.c
    src/base/ftbase.c
    src/base/ftbitmap.c
    src/base/ftglyph.c
    src/base/ftinit.c
    src/base/ftsystem.c
    src/raster/raster.c
    src/sfnt/sfnt.c
    src/smooth/smooth.c
    src/truetype/truetype.c
)

include_directories(${FREETYPE_INCLUDE})
add_definitions(-DFT2_BUILD_LIBRARY -DFT_CONFIG_MODULES_H=<ftmodule.h>)

#### Static library (uncomment to build)
#add_library(freetype_static STATIC ${FREETYPE_SOURCES})
#set_target_properties(freetype_static PROPERTIES
#    OUTPUT_NAME "freetype"
#    PREFIX ""
#    IMPORT_PREFIX ""
#    COMPILE_DEFINITIONS ""
#    CLEAN_DIRECT_OUTPUT 1)

add_library(freetype_shared SHARED ${FREETYPE_SOURCES})
if(WIN32)
    set_target_properties(freetype_shared PROPERTIES OUTPUT_NAME "freetype${FILEHANDLER_SERIES}")
else()
    set_target_properties(freetype_shared PROPERTIES OUTPUT_NAME "freetype")
endif()
set_target_properties(freetype_shared PROPERTIES
    VERSION ${FREETYPE_SERIES}.${FREETYPE_MAJOR}.${FREETYPE_MINOR}
    PREFIX ""
    IMPORT_PREFIX ""
    CLEAN_DIRECT_OUTPUT 1)