cmake_minimum_required(VERSION 2.6)
project(libjpeg-turbo)

set(JPEG_SERIES 1)
set(JPEG_MAJOR  2)
set(JPEG_MINOR  0)

add_subdirectory(simd)

set(JPEG_SOURCES jcomapi.c jdapimin.c jdapistd.c jdatasrc.c jdcoefct.c jdcolor.c
    jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c jdmerge.c
    jdphuff.c jdpostct.c jdsample.c jerror.c jidctflt.c jidctfst.c jidctint.c
    jidctred.c jquant1.c jquant2.c jutils.c jmemmgr.c jmemnobs.c jaricom.c jdarith.c
)

if(64BIT)
    set(JPEG_SOURCES ${JPEG_SOURCES} simd/jsimd_x86_64.c)
else()
    set(JPEG_SOURCES ${JPEG_SOURCES} simd/jsimd_i386.c)
endif()

if(64BIT)
    add_definitions(-DWIN64 -D__x86_64__ -DPIC)
endif()

# This tells CMake that the "source" files still need to be generated
set_source_files_properties(${SIMD_OBJS} PROPERTIES GENERATED 1)

add_library(jpegturbo_static STATIC ${JPEG_SOURCES} ${SIMD_OBJS})
set_target_properties(jpegturbo_static PROPERTIES
    OUTPUT_NAME "jpeg-turbo${JPEG_SERIES}"
    PREFIX "lib"
    IMPORT_PREFIX "lib"
    CLEAN_DIRECT_OUTPUT 1)
add_dependencies(jpegturbo_static jpegturbo_simd)

#### Shared library (uncomment to build)
#add_library(jpegturbo_shared SHARED ${JPEG_SOURCES} ${SIMD_OBJS})
#if(WIN32)
#    set_target_properties(jpegturbo_shared PROPERTIES OUTPUT_NAME "jpeg-turbo${JPEG_SERIES}")
#else()
#    set_target_properties(jpegturbo_shared PROPERTIES OUTPUT_NAME "jpeg-turbo")
#endif()
#set_target_properties(jpegturbo_shared PROPERTIES
#    VERSION ${JPEG_SERIES}.${JPEG_MAJOR}.${JPEG_MINOR}
#    PREFIX "lib"
#    IMPORT_PREFIX "lib"
#    CLEAN_DIRECT_OUTPUT 1)
#add_dependencies(jpegturbo_shared jpegturbo_simd)