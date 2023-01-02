#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.Edit->create(
        "text-davinci-edit-001",
        "What day of the wek is it?",
        "Fix the spelling mistakes"
      );
      std::cout << response["choices"][0]["text"].get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
