#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    try {
      auto fut = oai.Azure->request_image_generation_async(
        "resource", "api_version",
        "A snake in the grass!",
        1,
        "512x512"
      );

      // do other work

      // wait for the future to complete
      auto res = fut.get();

      // output the response
      std::cout << res["data"][0]["url"].get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
