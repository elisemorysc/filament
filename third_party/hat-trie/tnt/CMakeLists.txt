cmake_minimum_required(VERSION 3.10)
project(trie)

set(TARGET trie)
set(OUR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# ==================================================================================================
# Sources and headers
# ==================================================================================================
set(PUBLIC_HDRS
        ${OUR_DIR}/tsl/htrie_hash.h
        ${OUR_DIR}/tsl/htrie_map.h
        ${OUR_DIR}/tsl/htrie_set.h
)

# ==================================================================================================
# Include and target definitions
# ==================================================================================================
include_directories(${OUR_DIR}/tsl)

add_library(${TARGET} INTERFACE)

if(HUNTER_ENABLED)
  target_include_directories(${TARGET} INTERFACE  
    $<BUILD_INTERFACE:${OUR_DIR}>)
else(HUNTER_ENABLED)
  target_sources(${TARGET} INTERFACE ${PUBLIC_HDRS})
  target_include_directories(${TARGET} INTERFACE ${OUR_DIR})
endif(HUNTER_ENABLED)

# ==================================================================================================
# Installation
# ==================================================================================================
install(FILES ${PUBLIC_HDRS} DESTINATION include/${TARGET})

if(HUNTER_ENABLED)
  install(TARGETS ${TARGET} EXPORT ${TARGETS_EXPORT_NAME})
endif(HUNTER_ENABLED)