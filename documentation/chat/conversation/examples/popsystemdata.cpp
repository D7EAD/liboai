#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // set system message to guide the chat model
  convo.SetSystemData("You are helpful bot.");

  // remove the set system message
  convo.PopSystemData();

  // add a different system message
  convo.SetSystemData("You are a helpful bot that enjoys business.");

  // ...
}
