#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      // call async method; returns a future
      auto fut = oai.Image->create_edit_async(
        "C:/some/folder/otter.png",
        "A cute baby sea otter wearing a beret",
        "C:/some/folder/mask.png"        
      );

      // do other work...

      // check if the future is ready
      fut.wait();

      // get the contained response
      auto response = fut.get();

      // print some response data
      std::cout << response["data"][0]["url"].get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
