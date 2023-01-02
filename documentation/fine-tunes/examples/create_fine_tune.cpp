#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.FineTune->create(
        "file-XGinujblHPwGLSztz8cPS8XY"
      );
      std::cout << response["events"][0]["message"] << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
