#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    try {
      Response res = oai.Azure->delete_generated_image(
        "resource", "api_version",
        "f508bcf2-e651-4b4b-85a7-58ad77981ffa"
      );

      // output the response
      std::cout << res << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
