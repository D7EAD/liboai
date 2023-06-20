<p align="center">
  <img src="/images/_logo.png">
</p>

<hr>
<h1>Introduction</h1>
<p><code>liboai</code> is a simple, <b>unofficial</b> C++17 library for the OpenAI API. It allows developers to access OpenAI endpoints through a simple collection of methods and classes. The library can most effectively be thought of as a <b>spiritual port</b> of OpenAI's Python library, simply called <code>openai</code>, due to its similar structure - with few exceptions.

<h3>Features</h3>

- [x] [ChatGPT](https://github.com/D7EAD/liboai/tree/main/documentation/chat)
- [X] [Audio](https://github.com/D7EAD/liboai/tree/main/documentation/audio)
- [X] [Azure](https://github.com/D7EAD/liboai/tree/main/documentation/azure)
- [ ] [Functions](https://platform.openai.com/docs/api-reference/chat/create) (testing)
- [x] [Image DALL·E](https://github.com/D7EAD/liboai/tree/main/documentation/images)
- [x] [Models](https://github.com/D7EAD/liboai/tree/main/documentation/models)
- [x] [Completions](https://github.com/D7EAD/liboai/tree/main/documentation/completions) 
- [x] [Edit](https://github.com/D7EAD/liboai/tree/main/documentation/edits) 
- [x] [Embeddings](https://github.com/D7EAD/liboai/tree/main/documentation/embeddings) 
- [x] [Files](https://github.com/D7EAD/liboai/tree/main/documentation/files) 
- [x] [Fine-tunes](https://github.com/D7EAD/liboai/tree/main/documentation/fine-tunes) 
- [x] [Moderation](https://github.com/D7EAD/liboai/tree/main/documentation/moderations)
- [X] Asynchronous Support

<h1>Usage</h1>
See below for just how similar in style <code>liboai</code> and its Python alternative are when generating an image using DALL-E.</p>
<details open>
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

<details open>
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

<p>Running the above will print out the URL to the resulting generated image, which may or may not look similar to the one found below.</p>
<table>
<tr>
<th>Example Image</th>
</tr>
<td>

<img src="/images/snake.png">

</td>
</tr>
</table>

<p><i>Keep in mind the above C++ example is a minimal example and is not an exception-safe snippet. Please see <a href="/documentation">the documentation</a> for more detailed and exception-safe code snippets.</i></p>

<h1>Dependencies</h1>
<p>For the library to work the way it does, it relies on two major dependencies. These dependencies can be found listed below.<p>

- <a href="https://github.com/nlohmann/json">nlohmann-json</a>
- <a href="https://curl.se/">cURL</a>

*If building the library using the provided solution, it is recommended to install these dependencies using <b>vcpkg</b>.*

<h1>Documentation</h1>
<p>For detailed documentation and additional code examples, see the library's documentation <a href="/documentation">here</a>.

<h1>Contributing</h1>
<p>Artificial intelligence is an exciting and quickly-changing field. 

If you'd like to partake in further placing the power of AI in the hands of everyday people, please consider contributing by either submitting new code and features via a **Pull Request**. If you have any issues using the library, or just want to suggest new features, feel free to contact me directly using the info on my <a href="https://github.com/D7EAD">profile</a> or open an **Issue**.
