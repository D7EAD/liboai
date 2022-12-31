<p align="center">
  <img src="/images/_logo.png">
  <br>
  <img src="https://img.shields.io/github/license/D7EAD/liboai.js.svg">     <img src="https://img.shields.io/github/release/D7EAD/liboai.js.svg">
</p>

<hr>
<h1>Introduction</h1>
<p><code>liboai</code> is a simple, <b>unofficial</b> C++17 library for the OpenAI API. It allows developers to access OpenAI endpoints through a simple collection of methods and classes.

It can most efficiently be thought of as a <b>spiritual port</b> of OpenAI's Python library, simply called <code>openai</code>, due to its similar structure--with few exceptions. See below for just how similar in style <code>liboai</code> and its Python alternative are when generating an image using DALL-E.</p>
<details closed>
<summary>DALL-E Generation in Python.</summary>
<br>

```py
import openai
import os

openai.api_key = os.getenv("OPENAI_API_KEY")
response = openai.Image.create(
    prompt="A snake in the grass!",
    n=1,
    size="256x256"
)
print(response["data"][0]["url"])
```
</details>

<details closed>
<summary>DALL-E Generation in C++.</summary>
<br>

```cpp
#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  oai.auth.SetKeyEnv("OPENAI_API_KEY");
	
  Response res = oai.Image->create(
    "A snake in the grass!",
    1,
    "256x256"
  );

  std::cout << res["data"][0]["url"] << std::endl;
}
```

</details>

<p>Keep in mind the above C++ example is a minimal example and is not an exception-safe snippet. Please see <a href="/documentation">the documentation</a> for more detailed and exception-safe code snippets.</p>

<h1>Dependencies</h1>
<p>To make the library work the way it does, it relies on two major dependencies. These dependencies can be found listed below.<p>

- <a href="https://github.com/nlohmann/json">nlohmann-json</a>
- <a href="https://github.com/libcpr/cpr">cpr</a>

*If building the library using the provided solution, it is recommended to install these dependencies using <b>vcpkg</b>.*

<h1>Documentation</h1>
<p>For detailed documentation and additional code examples, see the library's documentation <a href="/documentation">here</a>.

<h1>Contributing</h1>
<p>Artificial intelligence is an exciting and quickly-changing field. 

If you'd like to partake in further placing the power of AI in the hands of everyday people, please consider contributing by either submitting new code and features via a **Pull Request** or by suggesting new features and any existing bugs via **Issues**.<p>
<p><img src="https://img.shields.io/github/issues/D7EAD/liboai.js.svg">     <img src="https://img.shields.io/github/issues-pr/D7EAD/liboai.js.svg"></p>
