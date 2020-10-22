hunter_config(civetweb 
              VERSION 1.11-p0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON CIVETWEB_ENABLE_WEBSOCKETS=ON)

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
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(SPIRV-Tools 
              URL https://github.com/cpp-pm/SPIRV-Tools/archive/update-v2020.4.zip
              SHA1 c9e2b9a3baa141f6daaed31447cecd4985f47101
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(spirv-cross 
              VERSION 20200917
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(astc-encoder
              URL https://github.com/cpp-pm/astc-encoder/archive/1.3-a47b80f-p0.zip
              SHA1 0143896afc679c05171a20a46217bbc0b8a5896d
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(etc2comp
              VERSION 0.0.0-9cd0f9c-p0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(Assimp
              VERSION 5.0.1-dbd99bf
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(tinyexr
              VERSION 1.0.0-297e645-p0
              CMAKE_ARGS 
                  CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

# BEGIN ASSIMP TEMPS FIXME
hunter_config(openddlparser
              VERSION 0.1.0-p2
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(poly2tri
              VERSION 1.0.0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(pugixml
              VERSION 1.10-p0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(polyclipping
              VERSION 4.8.8-p0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)
#END ASSIMP TEMP

#Testing only
hunter_config(md5
              VERSION 1.6
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)


                
  