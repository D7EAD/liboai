#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.FineTune->remove(
        "curie:ft-acmeco-2021-03-03-21-44-20"
      );
      std::cout << response["deleted"].get<bool>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
