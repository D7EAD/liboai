<h1>Azure</h1>
<p>The <code>Azure</code> class is defined in <code>azure.h</code> at <code>liboai::Azure</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://learn.microsoft.com/en-us/azure/cognitive-services/openai/reference">Azure</a> OpenAI API components.

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>azure.h</code>. You can find their function signature(s) below.</p>

<h3>Create a Completion</h3>
<p>Given a prompt, the model will return one or more predicted completions. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response create_completion(
  const std::string& resource_name,
  const std::string& deployment_id,
  const std::string& api_version,
  std::optional<std::string> prompt = std::nullopt,
  std::optional<std::string> suffix = std::nullopt,
  std::optional<uint16_t> max_tokens = std::nullopt,
  std::optional<float> temperature = std::nullopt,
  std::optional<float> top_p = std::nullopt,
  std::optional<uint16_t> n = std::nullopt,
  std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
  std::optional<uint8_t> logprobs = std::nullopt,
  std::optional<bool> echo = std::nullopt,
  std::optional<std::vector<std::string>> stop = std::nullopt,
  std::optional<float> presence_penalty = std::nullopt,
  std::optional<float> frequency_penalty = std::nullopt,
  std::optional<uint16_t> best_of = std::nullopt,
  std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
  std::optional<std::string> user = std::nullopt
) const & noexcept(false);
```

<h3>Create a Completion (async)</h3>
<p>Given a prompt, the model will asynchronously return one or more predicted completions. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse create_completion_async(
  const std::string& resource_name,
  const std::string& deployment_id,
  const std::string& api_version,
  std::optional<std::string> prompt = std::nullopt,
  std::optional<std::string> suffix = std::nullopt,
  std::optional<uint16_t> max_tokens = std::nullopt,
  std::optional<float> temperature = std::nullopt,
  std::optional<float> top_p = std::nullopt,
  std::optional<uint16_t> n = std::nullopt,
  std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
  std::optional<uint8_t> logprobs = std::nullopt,
  std::optional<bool> echo = std::nullopt,
  std::optional<std::vector<std::string>> stop = std::nullopt,
  std::optional<float> presence_penalty = std::nullopt,
  std::optional<float> frequency_penalty = std::nullopt,
  std::optional<uint16_t> best_of = std::nullopt,
  std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
  std::optional<std::string> user = std::nullopt
) const & noexcept(false);
```

<h3>Create an Embedding</h3>
<p>Creates an embedding vector representing the input text. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response create_embedding(
  const std::string& resource_name,
  const std::string& deployment_id,
  const std::string& api_version,
  const std::string& input,
  std::optional<std::string> user = std::nullopt
) const & noexcept(false);
```

<h3>Create an Embedding (async)</h3>
<p>Asynchronously creates an embedding vector representing the input text. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse create_embedding_async(
  const std::string& resource_name,
  const std::string& deployment_id,
  const std::string& api_version,
  const std::string& input,
  std::optional<std::string> user = std::nullopt
) const & noexcept(false);
```

<h3>Create a Chat Completion</h3>
<p>Creates a completion for the chat message. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response create_chat_completion(
  const std::string& resource_name,
  const std::string& deployment_id,
  const std::string& api_version,
  const Conversation& conversation,
  std::optional<float> temperature = std::nullopt,
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
<p>Asynchronously creates a completion for the chat message. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse create_chat_completion_async(
  const std::string& resource_name,
  const std::string& deployment_id,
  const std::string& api_version,
  const Conversation& conversation,
  std::optional<float> temperature = std::nullopt,
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

<h3>Request an Image Generation</h3>
<p>Generate a batch of images from a text caption. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response request_image_generation(
  const std::string& resource_name,
  const std::string& api_version,
  const std::string& prompt,
  std::optional<uint8_t> n = std::nullopt,
  std::optional<std::string> size = std::nullopt
) const & noexcept(false);
```

<h3>Request an Image Generation (async)</h3>
<p>Asynchronously generate a batch of images from a text caption. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse request_image_generation_async(
  const std::string& resource_name,
  const std::string& api_version,
  const std::string& prompt,
  std::optional<uint8_t> n = std::nullopt,
  std::optional<std::string> size = std::nullopt
) const & noexcept(false);
```

<h3>Get a Previously Generated Image</h3>
<p>Retrieve the results (URL) of a previously called image generation operation. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response get_generated_image(
  const std::string& resource_name,
  const std::string& api_version,
  const std::string& operation_id
) const & noexcept(false);
```

<h3>Get a Previously Generated Image (async)</h3>
<p>Asynchronously retrieve the results (URL) of a previously called image generation operation. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse get_generated_image_async(
  const std::string& resource_name,
  const std::string& api_version,
  const std::string& operation_id
) const & noexcept(false);
```

<h3>Delete a Previously Generated Image</h3>
<p>Deletes the corresponding image from the Azure server. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response delete_generated_image(
  const std::string& resource_name,
  const std::string& api_version,
  const std::string& operation_id
) const & noexcept(false);
```

<h3>Delete a Previously Generated Image (async)</h3>
<p>Asynchronously deletes the corresponding image from the Azure server. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse delete_generated_image_async(
  const std::string& resource_name,
  const std::string& api_version,
  const std::string& operation_id
) const & noexcept(false);
```

<p>All function parameters marked <code>optional</code> are not required and are resolved on OpenAI's end if not supplied.</p>

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="./examples">examples</a> folder.
