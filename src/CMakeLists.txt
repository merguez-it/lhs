cmake_minimum_required (VERSION 2.8)
project(LHS_LIB)

include_directories(
  ${LHS_OUTPUT_TEMP_DIR}
  ${LHS_SOURCE_DIR}/include
  )

set(LHS_SOURCES
  server.cc
  util/string.cc
  util/datetime.cc
  util/trex.c
  util/re.cc
  util/varg.cc
  protocol/http.cc
  protocol/http/uri.cc
  protocol/http/mime.cc
  protocol/http/header.cc
  protocol/http/request.cc
  protocol/http/response.cc
  protocol/http/env.cc
  protocol/http/middleware.cc
  middleware/static_file.cc
  middleware/error_404.cc
  middleware/urlmap.cc
  )

add_library(lhs SHARED ${LHS_SOURCES})
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows") 
  target_link_libraries(lhs ws2_32)
endif()
add_library(lhs-static STATIC ${LHS_SOURCES})
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows") 
  target_link_libraries(lhs-static ws2_32)
endif()

