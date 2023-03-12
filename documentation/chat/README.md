<h1>Chat</h1>
<p>The <code>ChatCompletion</code> class is defined in <code>chat.h</code> at <code>liboai::ChatCompletion</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://beta.openai.com/docs/api-reference/chat">Chat</a> endpoint of the OpenAI API; this endpoint's functionality can be found below.</p>
- Given a chat conversation, the model will return a chat completion response.

> **Note**
> 
> Before attempting to use the below methods, it is **highly** recommended
> to read through the documentation, and thoroughly understand the use,
> of the <a href="./conversation">Conversation</a> class as it is used
> in tandem with the `ChatCompletion` methods to keep track of chat 
> history and succinctly form a conversation with the OpenAI chat
> endpoint.

<h2>Methods</h2>
<p>This document covers the method(s) located in <code>chat.h</code>. You can find their function signature(s) below.</p>

<h3>Create a Chat Completion</h3>
<p>Creates a completion for the ongoing conversation. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response create(
  const std::string& model,
  const Conversation& conversation,
  std::optional<float> temperature = std::nullopt,
  std::optional<float> top_p = std::nullopt,
  std::optional<uint16_t> n = std::nullopt,
  std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
  std::optional<std::vector<std::string>> stop = std::nullopt,
  std::optional<uint16_t> max_tokens = std::nullopt,
  std::optional<float> presence_penalty = std::nullopt,
  std::optional<float> frequency_penalty = std::nullopt,
  std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
  std::optional<std::string> user = std::nullopt
) const & noexcept(false);
```

<h3>Create a Chat Completion (async)</h3>
<p>Asynchronously creates a completion for the ongoing conversation. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse create_async(
  const std::string& model,
  const Conversation& conversation,
  std::optional<float> temperature = std::nullopt,
  std::optional<float> top_p = std::nullopt,
  std::optional<uint16_t> n = std::nullopt,
  std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
  std::optional<std::vector<std::string>> stop = std::nullopt,
  std::optional<uint16_t> max_tokens = std::nullopt,
  std::optional<float> presence_penalty = std::nullopt,
  std::optional<float> frequency_penalty = std::nullopt,
  std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
  std::optional<std::string> user = std::nullopt
) const& noexcept(false);
```

<p>All function parameters marked <code>optional</code> are not required and are resolved on OpenAI's end if not supplied.</p>

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="./examples">examples</a> folder.
