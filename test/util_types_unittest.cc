#include <iostream>
#include "util/types.h"
#include "gtest/gtest.h"

TEST(UtilTypes, intToString) {
  int value = 10;

  std::string result = lhs::util::convert<int, std::string>(value);

  ASSERT_TRUE("10" == result);
}

TEST(UtilTypes, stringToInt) {
  std::string value("10");

  int result = lhs::util::convert<std::string, int>(value);

  ASSERT_TRUE(10 == result);
}

TEST(UtilTypes, size_tToString) {
  size_t value = 10;

  std::string result = lhs::util::convert<size_t, std::string>(value);

  ASSERT_TRUE("10" == result);
}

TEST(UtilTypes, stringToSize_t) {
  std::string value("10");

  size_t result = lhs::util::convert<std::string, size_t>(value);

  ASSERT_TRUE(10 == result);
}

TEST(UtilTypes, intToFloat) {
  int value = 10;

  float result = lhs::util::convert<int, float>(value);

  ASSERT_TRUE(10.0 == result);
}

TEST(UtilTypes, floatToInt) {
  float value = 10.3;

  int result = lhs::util::convert<float, int>(value);

  ASSERT_TRUE(10 == result);
}

TEST(UtilTypes, stringToBoolFalse) {
  std::string value;

  bool result = lhs::util::convert<std::string, bool>(value);

  ASSERT_FALSE(result);
}

TEST(UtilTypes, stringToBoolTrue) {
  std::string value("1");

  bool result = lhs::util::convert<std::string, bool>(value);

  ASSERT_TRUE(result);
}

TEST(UtilTypes, boolToStringFalse) {
  bool value = false;

  std::string result = lhs::util::convert<bool, std::string>(value);

  ASSERT_TRUE("0" == result);
}

TEST(UtilTypes, boolToStringTrue) {
  bool value = true;

  std::string result = lhs::util::convert<bool, std::string>(value);

  ASSERT_TRUE("1" == result);
}
