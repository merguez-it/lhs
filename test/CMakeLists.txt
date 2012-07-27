cmake_minimum_required (VERSION 2.8)
project(LHS_TESTS)

include(FindGTest)

include_directories(${LHS_SOURCE_DIR}/include ${GTEST_INCLUDE_DIRS})

add_executable(util_types_unittest "util_types_unittest.cc")
target_link_libraries(util_types_unittest lhs-static ${GTEST_BOTH_LIBRARIES})
add_test(UTIL_TYPES_UNITTEST util_types_unittest)

add_executable(header_unittest "header_unittest.cc")
target_link_libraries(header_unittest lhs-static ${GTEST_BOTH_LIBRARIES})
add_test(HEADER_UNITTEST header_unittest)

add_executable(request_unittest "request_unittest.cc")
target_link_libraries(request_unittest lhs-static ${GTEST_BOTH_LIBRARIES})
add_test(REQUEST_UNITTEST request_unittest)

add_executable(uri_unittest "uri_unittest.cc")
target_link_libraries(uri_unittest lhs-static ${GTEST_BOTH_LIBRARIES})
add_test(URI_UNITTEST uri_unittest)

add_executable(re_unittest "re_unittest.cc")
target_link_libraries(re_unittest lhs-static ${GTEST_BOTH_LIBRARIES})
add_test(RE_UNITTEST re_unittest)
