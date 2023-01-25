#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      // call async method; returns a future
      auto fut = oai.FineTune->create_async(
        "file-XGinujblHPwGLSztz8cPS8XY"
      );

      // do other work...

      // check if the future is ready
      fut.wait();

      // get the contained response
      auto response = fut.get();

      // print some response data
      std::cout << response["events"][0]["message"] << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
