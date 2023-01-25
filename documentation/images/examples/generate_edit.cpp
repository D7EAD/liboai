#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.Image->create_edit(
        "C:/some/folder/otter.png",
        "A cute baby sea otter wearing a beret",
        "C:/some/folder/mask.png"
      );

      std::cout << response["data"][0]["url"].get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
