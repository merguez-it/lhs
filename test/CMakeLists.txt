cmake_minimum_required (VERSION 2.8)
project(LHS_TESTS)

include_directories(
  ${LHS_SOURCE_DIR}/include 
  ${LHS_EXTERN_SOURCE_DIR}/gtest-1.6.0/include
  )

set(TEST_LIBS
  lhs-static
  ${MGZ_NET_LIBRARIES}
  ${MGZ_REGEX_LIBRARIES}
  gtest
  gtest_main
  )

add_executable(header_unittest "header_unittest.cc")
target_link_libraries(header_unittest ${TEST_LIBS})
add_test(HEADER_UNITTEST header_unittest)

add_executable(request_unittest "request_unittest.cc")
target_link_libraries(request_unittest ${TEST_LIBS})
add_test(REQUEST_UNITTEST request_unittest)

