#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      if (oai.File->download("file-XjGxS3KTG0uNmNOK362iJua3", "C:/some/folder/file.jsonl")) {
        std::cout << "File downloaded successfully!" << std::endl;
      }
      else {
        std::cout << "File download failed!" << std::endl;
      }
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
