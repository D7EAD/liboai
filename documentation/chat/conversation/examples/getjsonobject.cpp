#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // add a message to the conversation
  convo.AddUserData("Hello, how are you? What time is it for you?");

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.ChatCompletion->create(
        "gpt-3.5-turbo", convo
      );

      // update the conversation with the response
      convo.Update(response);

      // get the internal conversation JSON object
      nlohmann::json json = convo.GetJSON();
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
