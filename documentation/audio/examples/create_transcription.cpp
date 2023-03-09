#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  oai.auth.SetKeyEnv("OPENAI_API_KEY");

  try {
    Response res = oai.Audio->transcribe(
      "C:/some/folder/file.mp3",
      "whisper-1"
    );
    std::cout << res["text"] << std::endl;
  }
  catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}
