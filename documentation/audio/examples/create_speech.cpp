#include "liboai/include/liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  std::ofstream ocout("demo.mp3", std::ios::binary);

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response res = oai.Audio->speech(
        "tts-1",
        "alloy",
        "Today is a wonderful day to build something people love!"
      );
      ocout << res.content;
      ocout.close();
      std::cout << res.content.size() << std::endl;
    }
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
