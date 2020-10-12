cmake_minimum_required(VERSION 3.10)
project(glslminifier)

set(TARGET glslminifier)

# ==================================================================================================
# Source files
# ==================================================================================================
set(SRCS src/main.cpp src/GlslMinify.cpp)

# ==================================================================================================
# Target definitions
# ==================================================================================================
add_executable(${TARGET} ${SRCS})
if(HUNTER_ENABLED)
  target_link_libraries(${TARGET} PRIVATE utils)
else(HUNTER_ENABLED)
  target_link_libraries(${TARGET} PRIVATE utils getopt)
endif(HUNTER_ENABLED)

# =================================================================================================
# Licenses
# ==================================================================================================
set(MODULE_LICENSES getopt)
set(GENERATION_ROOT ${CMAKE_CURRENT_BINARY_DIR}/generated)
list_licenses(${GENERATION_ROOT}/licenses/licenses.inc ${MODULE_LICENSES})
target_include_directories(${TARGET} PRIVATE ${GENERATION_ROOT})

# ==================================================================================================
# Installation
# ==================================================================================================
install(TARGETS ${TARGET} RUNTIME DESTINATION bin)

# ==================================================================================================
# Tests
# ==================================================================================================
if (NOT ANDROID AND NOT HUNTER_ENABLED) #eli fixme
    add_executable(test_${TARGET}
            src/GlslMinify.cpp
            tests/test_glslminifier.cpp
    )
    target_include_directories(test_${TARGET} PRIVATE src)
    if(HUNTER_ENABLED)
      target_link_libraries(test_${TARGET} PRIVATE GTest::gtest GTest::gtest_main)
    else(HUNTER_ENABLED)
      target_link_libraries(test_${TARGET} PRIVATE gtest)
    endif(HUNTER_ENABLED)
endif()
