hunter_config(civetweb VERSION 1.11-p0
              CMAKE_ARGS CIVETWEB_ENABLE_WEBSOCKETS=ON)

hunter_config(smol-v VERSION 0.0.0-4b52c16-p0)

hunter_config(benchmark VERSION 1.5.0)

hunter_config(draco VERSION 1.3.4-p0)

hunter_config(GTest VERSION 1.10.0-p1)

hunter_config(SPIRV-Headers VERSION 1.5.1)

hunter_config(glslang 
              URL https://github.com/elisemorysc/glslang/archive/9eef54b2-p0.tar.gz
              SHA1 f99ff5307ee891ed57aed6efd5097816bffe5c7f
              CMAKE_ARGS ENABLE_HLSL=OFF ENABLE_GLSLANG_BINARIES=OFF ENABLE_OPT=OFF BUILD_TESTING=OFF)

hunter_config(SPIRV-Tools 
              URL https://github.com/elisemorysc/SPIRV-Tools/archive/hunter-v2020.1.tar.gz
              SHA1 34c5b3c1182b8d9fca32ff4a25d1ce94f43c92f0)

hunter_config(spirv-cross VERSION 20200917)

hunter_config(astc-encoder
              URL https://github.com/cpp-pm/astc-encoder/archive/1.3-a47b80f-p0.zip
              SHA1 0143896afc679c05171a20a46217bbc0b8a5896d)

hunter_config(etc2comp VERSION 0.0.0-9cd0f9c-p0)

hunter_config(Assimp VERSION 5.0.1-dbd99bf)

hunter_config(tinyexr VERSION 1.0.0-297e645-p0)



                
  