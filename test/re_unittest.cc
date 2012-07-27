#include "util/re.h"
#include "gtest/gtest.h"
#include "util/exception.h"

TEST(RE, CreateWithException) {
  ASSERT_THROW(lhs::util::RE re(".((*"), Exception<lhs::util::RegexException>);
}

TEST(RE, SimpleMatching) {
  lhs::util::RE re("ABBC");
  ASSERT_TRUE(re.find("ABBC"));
}

TEST(RE, SimpleCapture) {
  lhs::util::RE re("A(.*)C");
  ASSERT_TRUE(re.find("ABBC"));
  ASSERT_EQ((size_t)1, re.captures());
  EXPECT_EQ("BB", re.match(0));
}

TEST(RE, SimpleDoubleCapture) {
  lhs::util::RE re("A(.{2})C");
  ASSERT_TRUE(re.find("ABBCAZZC"));
  ASSERT_EQ((size_t)2, re.captures());
  EXPECT_EQ("BB", re.match(0));
  EXPECT_EQ("ZZ", re.match(1));
}

TEST(RE, SimpleReplace) {
  lhs::util::RE re("A(.{2})C");
  ASSERT_TRUE(re.find("ABBCAZZC"));
  ASSERT_EQ((size_t)2, re.captures());
  EXPECT_EQ("A-CAZZC", re.replace(0, "-"));
  EXPECT_EQ("A-CA***C", re.replace(1, "***"));
  EXPECT_EQ("A++CA***C", re.replace(0, "++"));
  EXPECT_EQ("ABBCAZZC", re.undo_all());
  EXPECT_EQ("ABBCAZZC", re.undo_all());
}

TEST(RE, SimpleReplaceTwo) {
  lhs::util::RE re("A(.*)C");
  ASSERT_TRUE(re.find("ABBCAZZC"));
  ASSERT_EQ((size_t)2, re.captures());
  EXPECT_EQ("AZZCAZZC", re.replace(0, 1));
  EXPECT_EQ("A-CAZZC", re.replace(0, "-"));
  EXPECT_EQ("A-CA***C", re.replace(1, "***"));
  EXPECT_EQ("A++CA***C", re.replace(0, "++"));
  EXPECT_EQ("A++CAZZC", re.undo(1));
  EXPECT_EQ("ABBCAZZC", re.undo_all());
  EXPECT_EQ("ABBCAZZC", re.undo_all());
  EXPECT_EQ("A$$$CA$$$C", re.replace_all("$$$"));
  EXPECT_EQ("AZZCAZZC", re.replace_all(1));
}

TEST(RE, FindVersion) {
  lhs::util::RE re("(-\\d[\\d\\w\\.]*)(\\.jar)");
  ASSERT_TRUE(re.find("toto-titi-tata-1.2.3.Final.jar"));
  ASSERT_EQ((size_t)2, re.captures());
  ASSERT_EQ("-1.2.3.Final", re.match(0));
  ASSERT_EQ("toto-titi-tata.jar", re.replace(0, ""));
}

TEST(RE, quote) {
  ASSERT_EQ("A\\[B", lhs::util::RE::quote("A[B"));
  ASSERT_EQ("A\\]B", lhs::util::RE::quote("A]B"));
  ASSERT_EQ("A\\{B", lhs::util::RE::quote("A{B"));
  ASSERT_EQ("A\\}B", lhs::util::RE::quote("A}B"));
  ASSERT_EQ("A\\tB", lhs::util::RE::quote("A\tB"));
}

TEST(RE, pathMatch) {
  std::string qs = "/toto";
  std::string rqs = "^" + lhs::util::replace_all(lhs::util::RE::quote(qs), "/", "/+") + "(.*)";
  ASSERT_TRUE(lhs::util::RE(rqs).find("/toto/titi?a=b"));
  ASSERT_TRUE(lhs::util::RE(rqs).find("/toto"));
}
