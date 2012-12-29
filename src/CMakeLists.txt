cmake_minimum_required (VERSION 2.8)
project(LHS_LIB)

include_directories(
  ${LHS_OUTPUT_TEMP_DIR}
  ${LHS_SOURCE_DIR}/include
  )

set(LHS_SOURCES
  server.cc
  protocol/http.cc
  protocol/http/header.cc
  protocol/http/request.cc
  protocol/http/response.cc
  protocol/http/env.cc
  protocol/http/middleware.cc
  middleware/static_file.cc
  middleware/error_404.cc
  middleware/urlmap.cc
  )

set(LHS_LINKS
  ${MGZ_NET_LIBRARIES}
  ${MGZ_REGEX_LIBRARIES}
  )
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows") 
  set(LHS_LINKS
    ${LHS_LINKS}
    ws2_32
    )
endif()
add_library(lhs SHARED ${LHS_SOURCES})
target_link_libraries(lhs ${LHS_LINKS})

add_library(lhs-static STATIC ${LHS_SOURCES})
target_link_libraries(lhs ${LHS_LINKS})

