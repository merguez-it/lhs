cmake_minimum_required(VERSION 2.8)
project(LHS)

include(FindDoxygen)
include(CheckFunctionExists)

set(LHS_OUTPUT_TEMP_DIR ${CMAKE_BINARY_DIR}/.build)
file(MAKE_DIRECTORY ${LHS_OUTPUT_TEMP_DIR})

add_definitions(-Wall)
if(DEBUG)
  set(CMAKE_C_FLAGS "-g -O0 -DDEBUG")
  set(CMAKE_CXX_FLAGS "-g -O0 -DDEBUG")
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

CHECK_FUNCTION_EXISTS(vsnprintf HAVE_VSNPRINTF)
CHECK_FUNCTION_EXISTS(vasprintf HAVE_VASPRINTF)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${LHS_OUTPUT_TEMP_DIR}/config.h
  )

# Documentation
if(DOXYGEN_FOUND)
  find_program(GRAPHVIZ
    dot
    )
  if("${GRAPHVIZ}" STREQUAL "GRAPHVIZ-NOTFOUND")
    message(STATUS "GraphViz not found!")
    set(HAVE_DOT NO)
  else()
    message(STATUS "Found GraphViz: ${GRAPHVIZ}")
    set(HAVE_DOT YES)
  endif()
  configure_file(
    ${LHS_SOURCE_DIR}/Doxyfile.in 
    ${LHS_OUTPUT_TEMP_DIR}/Doxyfile
    )

  set(DOXYGEN_INPUT ${LHS_OUTPUT_TEMP_DIR}/Doxyfile)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT})
endif()

enable_testing()
add_subdirectory(src)
add_subdirectory(bin)
add_subdirectory(ext)
add_subdirectory(test)
