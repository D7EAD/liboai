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

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    ...
  }
}