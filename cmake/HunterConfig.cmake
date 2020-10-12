hunter_config(civetweb 
              VERSION 1.11-p0
              CMAKE_ARGS 
                CIVETWEB_ENABLE_WEBSOCKETS=ON CMAKE_CXX_FLAGS=-stdlib=libc++)

hunter_config(smol-v 
              VERSION 0.0.0-4b52c16-p0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(benchmark 
              VERSION 1.5.0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++)

hunter_config(draco 
              VERSION 1.3.4-p0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(GTest 
              VERSION 1.10.0-p1
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++)

hunter_config(glslang 
              VERSION 7.12.3352-p1
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++)
  