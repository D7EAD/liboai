#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // add a message to the conversation
  convo.AddUserData("What is the point of taxes?");

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      auto fut = oai.ChatCompletion->create_async(
        "gpt-3.5-turbo", convo
      );

      // do other work...

      // check if the future is ready
      fut.wait();

      // get the contained response
      auto response = fut.get();

      // update our conversation with the response
      convo.Update(response);

      // print the response
      std::cout << convo.GetLastResponse() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
