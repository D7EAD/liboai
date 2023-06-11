#include "include/liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    try {
      auto fut = oai.Azure->create_completion_async(
        "resource", "deploymentID", "api_version",
        "Write a short poem about a snowman."
      );

      // do other stuff

      // wait for the future to be ready
      fut.wait();

      // get the result
      auto res = fut.get();

      std::cout << res["choices"][0]["text"].get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
