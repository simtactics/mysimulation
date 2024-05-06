set(NASM "nasm")

if(WIN32)
    if(64BIT)
        set(ASMFLAGS -O3 -f win64 -DWIN64 -D__x86_64__ -DPIC -I${CMAKE_CURRENT_SOURCE_DIR}/)
        message("Building 64bit")
    else()
        set(ASMFLAGS -O3 -f win32 -DWIN32 -I${CMAKE_CURRENT_SOURCE_DIR}/)
    endif()
else()
    if(64BIT)
        set(ASMFLAGS -O3 -f elf64 -DELF -D__x86_64__ -DPIC -I${CMAKE_CURRENT_SOURCE_DIR}/)
    else()
        set(ASMFLAGS -O3 -f elf32 -DELF -I${CMAKE_CURRENT_SOURCE_DIR}/)
    endif()
endif()

if(64BIT)
    set(JPEG_SIMD_SOURCES       jfsseflt-64 jccolss2-64 jdcolss2-64 jcgrass2-64
        jcsamss2-64 jdsamss2-64 jdmerss2-64 jcqnts2i-64 jfss2fst-64 jfss2int-64
        jiss2red-64 jiss2int-64 jiss2fst-64 jcqnts2f-64 jiss2flt-64)
else()
    set(JPEG_SIMD_SOURCES jsimdcpu jccolmmx jcgrammx jdcolmmx jcsammmx jdsammmx
        jdmermmx jcqntmmx jfmmxfst jfmmxint jimmxred jimmxint jimmxfst jcqnt3dn
        jf3dnflt ji3dnflt jcqntsse jfsseflt jisseflt jccolss2 jcgrass2 jdcolss2
        jcsamss2 jdsamss2 jdmerss2 jcqnts2i jfss2fst jfss2int jiss2red jiss2int
        jiss2fst jcqnts2f jiss2flt)
endif()

foreach(file ${JPEG_SIMD_SOURCES})
  set(SIMD_SRC ${CMAKE_CURRENT_SOURCE_DIR}/${file}.asm)
  set(SIMD_OBJ ${CMAKE_CURRENT_BINARY_DIR}/${file}.obj)
  add_custom_command(OUTPUT ${SIMD_OBJ} DEPENDS ${SIMD_SRC} *.inc
    COMMAND ${NASM} ${ASMFLAGS} ${SIMD_SRC} -o ${SIMD_OBJ})
  set(SIMD_OBJS ${SIMD_OBJS} ${SIMD_OBJ})
  message("boop: " + ${file})
endforeach()

set(SIMD_OBJS ${SIMD_OBJS} PARENT_SCOPE)
add_custom_target(jpegturbo_simd DEPENDS ${SIMD_OBJS})
