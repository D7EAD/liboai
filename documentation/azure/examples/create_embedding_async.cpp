#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    try {
      auto fut = oai.Azure->create_embedding_async(
        "resource", "deploymentID", "api_version",
        "String to get embedding for"
      );

      // do other work

      // wait for the future to complete
      auto res = fut.get();

      // output the response
      std::cout << res << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
