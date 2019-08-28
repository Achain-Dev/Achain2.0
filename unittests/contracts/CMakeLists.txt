# will be implictly used for any compilation unit if not overrided by SYSTEM_INCLUDE_FOLDERS parameter
# these directories go as -isystem <dir> to avoid warnings from code of third-party libraries

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/act.bios/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/act.bios/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/act.msig/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/act.msig/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/act.system/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/act.system/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/act.token/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/act.token/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/act.wrap/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/act.wrap/)
