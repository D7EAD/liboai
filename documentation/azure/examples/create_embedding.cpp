#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    try {
      Response res = oai.Azure->create_embedding(
        "resource", "deploymentID", "api_version",
        "String to get embedding for"
      );

      std::cout << res << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
