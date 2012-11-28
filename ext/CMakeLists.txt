cmake_minimum_required (VERSION 2.8)
project(LORM_EXTERN)

include_directories(
  ${LORM_EXTERN_SOURCE_DIR}/gtest-1.6.0/include
  ${LORM_EXTERN_SOURCE_DIR}/gtest-1.6.0
  )

add_library(gtest ${LORM_EXTERN_SOURCE_DIR}/gtest-1.6.0/src/gtest-all.cc)
add_library(gtest_main ${LORM_EXTERN_SOURCE_DIR}/gtest-1.6.0/src/gtest_main.cc)

