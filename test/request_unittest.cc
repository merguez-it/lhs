#include <iostream>
#include <vector>
#include "protocol/http.h"
#include "protocol/http/request.h"
#include "gtest/gtest.h"

TEST(Request, testWithParams) {
  char data[] = "GET /toto?a=A&b=2 HTTP/1.1\n"
    "Host: localhost\n"
    "User-Agent: LHSSample/1.0\n"
    "Content-Length: 7\n"
    "\n"
    "c=C&d=4";
  lhs::http::request req(data);

  ASSERT_EQ(lhs::http::GET, req.method);
  ASSERT_EQ(lhs::http::HTTP_1_1, req.version);
  ASSERT_EQ("/toto?a=A&b=2", req.uri);
  ASSERT_EQ("A", req.params["a"]);
  ASSERT_EQ("2", req.params["b"]);

  ASSERT_EQ("localhost", mgz::util::any_cast<std::string>(req["Host"]));
  ASSERT_EQ("LHSSample/1.0", mgz::util::any_cast<std::string>(req["User-Agent"]));
  ASSERT_EQ("7", mgz::util::any_cast<std::string>(req["Content-Length"]));

  std::string body("c=C&d=4");
  ASSERT_TRUE(std::vector<char>(body.begin(), body.end()) == req.body);
}
