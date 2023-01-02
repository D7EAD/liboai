#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKey("hard-coded-key")) { // NOT recommended
    ...
  }
}
