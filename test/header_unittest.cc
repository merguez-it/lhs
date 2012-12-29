#include <iostream>
#include "protocol/http/header.h"
#include "gtest/gtest.h"

TEST(Header, testNoData) {
  lhs::http::header h;
  h["Content-Length"] = 10;
  h["User-Agent"] = "Test/1.0.0";

  ASSERT_EQ(10, mgz::util::any_cast<int>(h["Content-Length"]));
  ASSERT_TRUE("Test/1.0.0" == mgz::util::any_cast<std::string>(h["User-Agent"]));
}
