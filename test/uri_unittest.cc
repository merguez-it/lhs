#include <iostream>
#include "protocol/http/uri.h"
#include "gtest/gtest.h"

TEST(URI, test) {
  ASSERT_TRUE(lhs::http::uri::encode("ABC") == "ABC");

  const std::string ORG("\0\1\2", 3);
  const std::string ENC("%00%01%02");
  ASSERT_TRUE(lhs::http::uri::encode(ORG) == ENC);
  ASSERT_TRUE(lhs::http::uri::decode(ENC) == ORG);

  ASSERT_TRUE(lhs::http::uri::encode("\xFF") == "%FF");
  ASSERT_TRUE(lhs::http::uri::decode("%FF") == "\xFF");
  ASSERT_TRUE(lhs::http::uri::decode("%ff") == "\xFF");

  // unsafe chars test, RFC1738
  const std::string UNSAFE(" <>#{}|\\^~[]`");
  std::string sUnsafeEnc = lhs::http::uri::encode(UNSAFE);
  ASSERT_TRUE(std::string::npos == sUnsafeEnc.find_first_of(UNSAFE));
  ASSERT_TRUE(lhs::http::uri::decode(sUnsafeEnc) == UNSAFE);

  // random test
  const int MAX_LEN = 128;
  char a[MAX_LEN];
  srand((unsigned)time(NULL));
  for (int i = 0; i < 100; i++) {
    for (int j = 0; j < MAX_LEN; j++) {
      a[j] = rand() % (1 << 8);
    }
    int nLen = rand() % MAX_LEN;
    std::string sOrg(a, nLen);
    std::string sEnc = lhs::http::uri::encode(sOrg);
    ASSERT_TRUE(sOrg == lhs::http::uri::decode(sEnc));
  }
}
