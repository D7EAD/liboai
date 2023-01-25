#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      // call async method; returns a future
      auto fut = oai.File->download_async(
        "file-XjGxS3KTG0uNmNOK362iJua3", "C:/some/folder/file.jsonl"
      );

      // do other work...

      // check if the future is ready
      fut.wait();

      // check if downloaded successfully
      if (fut.get()) {
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
