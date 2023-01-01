<h1>Completions</h1>
<h2>Introduction</h2>
<p>The <code>Completions</code> class is defined in <code>completions.h</code> at <code>liboai::Completions</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://beta.openai.com/docs/api-reference/completions">Completions</a> endpoint of the OpenAI API; this endpoint's functionality can be found below.</p>
- Given a prompt, the model will return one or more predicted completions, and can also return the probabilities of alternative tokens at each position.

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>completions.h</code>. You can find their function signature(s) below.</p>

<h3>Create a Completion.</h3>
<p>Given a prompt, the model will return one or more predicted completions, and can also return the probabilities of alternative tokens at each position. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response create(
  const std::string& model_id,
  const std::optional<std::string>& prompt = std::nullopt,
  const std::optional<std::string>& suffix = std::nullopt,
  const std::optional<uint16_t>& max_tokens = std::nullopt,
  const std::optional<float>& temperature = std::nullopt,
  const std::optional<float>& top_p = std::nullopt,
  const std::optional<uint16_t>& n = std::nullopt,
  const std::optional<std::function<bool(std::string, intptr_t)>>& stream = std::nullopt,
  const std::optional<uint8_t>& logprobs = std::nullopt,
  const std::optional<bool>& echo = std::nullopt,
  const std::optional<std::vector<std::string>>& stop = std::nullopt,
  const std::optional<float>& presence_penalty = std::nullopt,
  const std::optional<float>& frequency_penalty = std::nullopt,
  const std::optional<uint16_t>& best_of = std::nullopt,
  const std::optional<std::unordered_map<std::string, int8_t>>& logit_bias = std::nullopt,
  const std::optional<std::string>& user = std::nullopt
) const &;
```

<p>All function parameters marked <code>optional</code> are not required and resolved on OpenAI's end if not supplied.</p>

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="/examples">examples</a> folder.
