#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // add user data - such as a question
  convo.AddUserData("What is the meaning of life?");

  // ...
}
