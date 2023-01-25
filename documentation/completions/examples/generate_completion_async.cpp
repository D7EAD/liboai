#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      // call async method; returns a future
      auto fut = oai.Completion->create_async(
        "text-davinci-003",
        "Say this is a test",
        std::nullopt, 
        7
      );

      // do other work...

      // check if the future is ready
      fut.wait();

      // get the contained response
      auto response = fut.get();

      // print some response data
      std::cout << response["choices"][0]["text"] << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
