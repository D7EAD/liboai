#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetAzureKeyFile("C:/some/folder/key.dat")) {
    // ...
  }
}
