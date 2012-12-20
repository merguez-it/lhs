cmake_minimum_required (VERSION 2.8)
project(LHS_SERVER)

include_directories(
  ${LHS_OUTPUT_TEMP_DIR}
  ${LHS_SOURCE_DIR}/include
  )

set(LHS_SERVER_SOURCES
  lhs-server.cc
  )

set(LHS_SERVER_LINK lhs)
add_executable(lhs-server ${LHS_SERVER_SOURCES})
target_link_libraries(lhs-server ${LHS_SERVER_LINK})

