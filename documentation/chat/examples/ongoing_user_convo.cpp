#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // holds next user input
  std::string input;

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      while (true) {
        // get next user input
        std::cout << "You: "; std::getline(std::cin, input);

        // add user input to conversation
        convo.AddUserData(input);

        // get response from OpenAI
        Response response = oai.ChatCompletion->create(
          "gpt-3.5-turbo", convo
        );

        // update our conversation with the response
        convo.Update(response);

        // print the response
        std::cout << "Bot: " << convo.GetLastResponse() << std::endl;
      }
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
