#include "liboai/include/liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      std::ofstream ocout("demo.mp3", std::ios::binary);
      auto fut = oai.Audio->speech_async(
        "tts-1",
        "alloy",
        "Today is a wonderful day to build something people love!"
      );
      // do other work...

      // check if the future is ready
      fut.wait();

      // get the contained response
      auto res = fut.get();

      ocout << res.content;
      ocout.close();
      std::cout << res.content.size() << std::endl;
    }
    catch (const std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
