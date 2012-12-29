cmake_minimum_required(VERSION 2.8)
project(LHS)

include(FindDoxygen)
include(FindPkgConfig)
include(CheckFunctionExists)
include(CheckIncludeFiles)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CheckSymbolExists)

set(LHS_OUTPUT_TEMP_DIR ${CMAKE_BINARY_DIR}/.build)
file(MAKE_DIRECTORY ${LHS_OUTPUT_TEMP_DIR})

add_definitions(-Wall)
if(DEBUG)
  set(CMAKE_C_FLAGS "-g -O0 -DDEBUG")
  set(CMAKE_CXX_FLAGS "-g -O0 -DDEBUG")
  set(CMAKE_VERBOSE_MAKEFILE ON)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__WIN32__")
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -D__WIN32__")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__APPLE__")
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -D__APPLE__")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D__UNIX__")
  set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -D__UNIX__")
endif()

CHECK_FUNCTION_EXISTS(vsnprintf HAVE_VSNPRINTF)
CHECK_FUNCTION_EXISTS(vasprintf HAVE_VASPRINTF)
CHECK_INCLUDE_FILES(pthread.h HAVE_PTHREAD_H)
CHECK_INCLUDE_FILES(windows.h HAVE_WINDOWS_H)
CHECK_SYMBOL_EXISTS(GetFileAttributes windows.h HAVE_GETFILEATTRIBUTES)
CHECK_FUNCTION_EXISTS(pthread_create HAVE_PTHREAD)
CHECK_C_COMPILER_FLAG(-pthread C_HAS_PTHREAD)
CHECK_CXX_COMPILER_FLAG(-pthread CXX_HAS_PTHREAD)
if(C_HAS_PTHREAD)
  set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS} -pthread") 
  set(CMAKE_SHARED_MODULE_CREATE_C_FLAGS "${CMAKE_SHARED_MODULE_CREATE_C_FLAGS} -pthread") 
elseif(HAVE_PTHREAD)
  set(CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_C_FLAGS} -lpthread") 
  set(CMAKE_SHARED_MODULE_CREATE_C_FLAGS "${CMAKE_SHARED_MODULE_CREATE_C_FLAGS} -lpthread") 
endif()
if(CXX_HAS_PTHREAD)
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -pthread") 
  set(CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS "${CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS} -pthread") 
elseif(HAVE_PTHREAD)
  set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -lpthread") 
  set(CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS "${CMAKE_SHARED_MODULE_CREATE_CXX_FLAGS} -lpthread") 
endif()
if(C_HAS_PTHREAD)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
elseif(CXX_HAS_PTHREAD)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")
elseif(HAVE_PTHREAD)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lpthread")
endif()

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
  ${LHS_OUTPUT_TEMP_DIR}/config.h
  )

pkg_search_module(MGZ_NET REQUIRED mgz-net)
pkg_search_module(MGZ_REGEX REQUIRED mgz-regex)
include_directories(${MGZ_NET_INCLUDE_DIRS} ${MGZ_REGEX_INCLUDE_DIRS})
link_directories(${MGZ_NET_LIBRARY_DIRS} ${MGZ_REGEX_LIBRARY_DIRS})

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
