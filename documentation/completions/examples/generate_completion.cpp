#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.Completion->create(
        "text-davinci-003",
        "Say this is a test",
        std::nullopt,
        7
      );
      std::cout << response["choices"][0]["text"].get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
