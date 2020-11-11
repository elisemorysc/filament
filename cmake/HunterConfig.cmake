#hunter_config(civetweb VERSION 1.11-p0
#              CMAKE_ARGS CIVETWEB_ENABLE_WEBSOCKETS=ON)

hunter_config(civetweb
              URL https://github.com/elisemorysc/civetweb/archive/v1.13-hunter.tar.gz
              SHA1 f0769451f1c21d32613dd8b3a1e8cec5eaa09e9c
              CMAKE_ARGS CIVETWEB_ENABLE_WEBSOCKETS=ON CIVETWEB_ENABLE_CXX=ON)

hunter_config(SPIRV-Headers VERSION 1.5.1.corrected)

hunter_config(SPIRV-Tools VERSION 2020.1-p0)

hunter_config(spirv-cross VERSION 20200917)

hunter_config(glslang VERSION 8.13.3743-9eef54b2-p0
              CMAKE_ARGS ENABLE_HLSL=OFF ENABLE_GLSLANG_BINARIES=OFF ENABLE_OPT=OFF BUILD_TESTING=OFF)

hunter_config(astc-encoder VERSION 1.3-a47b80f-p1)
