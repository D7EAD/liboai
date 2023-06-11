#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  Conversation convo;
  convo.AddUserData("Hi, how are you?");

  if (oai.auth.SetAzureKeyEnv("AZURE_API_KEY")) {
    try {
      Response res = oai.Azure->create_chat_completion(
        "resource", "deploymentID", "api_version",
        convo
      );

      // update the conversation with the response
      convo.Update(res);

      // print the response from the API
      std::cout << convo.GetLastResponse() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
