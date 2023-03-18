#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  /*
    Set some proxies:
      when we go to an http site, use fakeproxy1
      when we go to an https site, use fakeproxy2
  */
  oai.auth.SetProxies({
    { "http", "http://www.fakeproxy1.com" },
    { "https", "https://www.fakeproxy2.com" }
  });

  /*
    Set the per-protocol proxy auth info:
      when we go to an http site, use fakeuser1 and fakepass1
      when we go to an https site, use fakeuser2 and fakepass2
  */
  oai.auth.SetProxyAuth({
    {"http", {"fakeuser1", "fakepass1"}},
    {"https", {"fakeuser2", "fakepass2"}},
  });

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    ...
  }
}
