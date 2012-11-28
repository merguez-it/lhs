cmake_minimum_required (VERSION 2.8)
project(LHS_EXTERN)

include_directories(
  ${LHS_EXTERN_SOURCE_DIR}/gtest-1.6.0/include
  ${LHS_EXTERN_SOURCE_DIR}/gtest-1.6.0
  )

add_library(gtest ${LHS_EXTERN_SOURCE_DIR}/gtest-1.6.0/src/gtest-all.cc)
add_library(gtest_main ${LHS_EXTERN_SOURCE_DIR}/gtest-1.6.0/src/gtest_main.cc)

