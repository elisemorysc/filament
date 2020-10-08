cmake_minimum_required(VERSION 3.10)
project(vkmemalloc)

set(TARGET vkmemalloc)

set(PUBLIC_HDR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(PUBLIC_HDRS ${PUBLIC_HDR_DIR}/vk_mem_alloc.h)

include_directories(${PUBLIC_HDR_DIR})

add_library(${TARGET} INTERFACE)

if(HUNTER_ENABLED)
  target_include_directories(${TARGET} INTERFACE  
    $<BUILD_INTERFACE:${PUBLIC_HDR_DIR}>)
  install(TARGETS ${TARGET} EXPORT ${TARGETS_EXPORT_NAME})
else(HUNTER_ENABLED)
  target_sources(${TARGET} INTERFACE ${PUBLIC_HDRS})
  target_include_directories(${TARGET} INTERFACE ${PUBLIC_HDR_DIR})
endif(HUNTER_ENABLED)
