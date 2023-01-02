#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  oai.auth.SetProxies({
    { "http", "http://www.fakeproxy.com" },
    { "https", "https://www.fakeproxy.com" },
    { "http", "12.34.56.78:8080" }
  });

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    ...
  }
}
