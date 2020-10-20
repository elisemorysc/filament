hunter_config(civetweb 
              VERSION 1.11-p0
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CIVETWEB_ENABLE_WEBSOCKETS=ON)

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
              URL https://github.com/elisemorysc/SPIRV-Tools/archive/hunter-2020.4.tar.gz
              SHA1 abaf2e0a33140e186509176bd03939229c451394
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(spirv-cross 
              VERSION 20200917
              CMAKE_ARGS 
                CMAKE_CXX_FLAGS=-stdlib=libc++ CMAKE_POSITION_INDEPENDENT_CODE=ON)

hunter_config(astc-encoder
              URL https://github.com/elisemorysc/astc-encoder/archive/hunter-a47b80f-p0.tar.gz
              SHA1 b53307db2b37ab75d7639feefeefb1a38b46a79d
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

                
  