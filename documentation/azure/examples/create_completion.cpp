#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    try {
      Response res = oai.Azure->create_completion(
        "resource", "deploymentID", "api_version",
        "Write a short poem about a snowman."
      );

      std::cout << res["choices"][0]["text"].get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
