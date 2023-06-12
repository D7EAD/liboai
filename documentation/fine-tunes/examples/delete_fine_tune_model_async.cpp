#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      // call async method; returns a future
      auto fut = oai.FineTune->remove_async(
        "curie:ft-acmeco-2021-03-03-21-44-20"
      );

      // do other work...

      // check if the future is ready
      fut.wait();

      // get the contained response
      auto response = fut.get();

      // print some response data
      std::cout << response["deleted"].get<bool>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
