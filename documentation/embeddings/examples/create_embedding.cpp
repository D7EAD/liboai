#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.Embedding->create(
        "text-embedding-ada-002",
        "The food was delicious and the waiter..."
      );
      std::cout << response["data"][0]["embedding"] << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
