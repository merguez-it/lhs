cmake_minimum_required (VERSION 2.8)
project(LHS_LIB)

include_directories(
  ${LHS_OUTPUT_TEMP_DIR}
  ${LHS_SOURCE_DIR}/include
  )

set(LHS_SOURCES
  http.cc
  server.cc
  util/string.cc
  util/datetime.cc
  http/uri.cc
  http/mime.cc
  http/header.cc
  http/request.cc
  http/response.cc
  http/env.cc
  http/handler.cc
  handler/static_file.cc
  handler/error_404.cc
  )

add_library(lhs SHARED ${LHS_SOURCES})
add_library(lhs-static STATIC ${LHS_SOURCES})

