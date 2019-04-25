add_contract(actx.system actx.system ${CMAKE_CURRENT_SOURCE_DIR}/src/actx.system.cpp)
#add_executable(actx.system.wasm ${CMAKE_CURRENT_SOURCE_DIR}/src/actx.system.cpp)
target_include_directories(actx.system.wasm
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../actx.token/include)

set_target_properties(actx.system.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
