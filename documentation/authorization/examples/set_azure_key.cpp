#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetAzureKey("hard-coded-key")) { // NOT recommended
    // ...
  }
}
