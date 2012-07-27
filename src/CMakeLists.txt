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
  util/trex.c
  util/re.cc
  http/uri.cc
  http/mime.cc
  http/header.cc
  http/request.cc
  http/response.cc
  http/env.cc
  http/middleware.cc
  middleware/static_file.cc
  middleware/error_404.cc
  middleware/urlmap.cc
  )

add_library(lhs SHARED ${LHS_SOURCES})
add_library(lhs-static STATIC ${LHS_SOURCES})

