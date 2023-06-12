#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    try {
      auto fut = oai.Azure->delete_generated_image_async(
        "resource", "api_version",
        "f508bcf2-e651-4b4b-85a7-58ad77981ffa"
      );

      // do other work

      // wait for the future to complete
      fut.wait();

      // get the result
      auto res = fut.get();

      // output the response
      std::cout << res << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
