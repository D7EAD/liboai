<h1>Edits</h1>
<p>The <code>Edits</code> class is defined in <code>edits.h</code> at <code>liboai::Edits</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://beta.openai.com/docs/api-reference/edits">Edits</a> endpoint of the OpenAI API; this endpoint's functionality can be found below.</p>
- Given a prompt and an instruction, the model will return an edited version of the prompt.

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>edits.h</code>. You can find their function signature(s) below.</p>

<h3>Create an Edit</h3>
<p>Creates a new edit for the provided input, instruction, and parameters. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response create(
  const std::string& model_id,
  std::optional<std::string> input = std::nullopt,
  std::optional<std::string> instruction = std::nullopt,
  std::optional<uint16_t> n = std::nullopt,
  std::optional<float> temperature = std::nullopt,
  std::optional<float> top_p = std::nullopt
) const & noexcept(false);
```

<h3>Create an Edit (async)</h3>
<p>Asynchronously creates a new edit for the provided input, instruction, and parameters. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse create_async(
  const std::string& model_id,
  std::optional<std::string> input = std::nullopt,
  std::optional<std::string> instruction = std::nullopt,
  std::optional<uint16_t> n = std::nullopt,
  std::optional<float> temperature = std::nullopt,
  std::optional<float> top_p = std::nullopt
) const & noexcept(false);
```

<p>All function parameters marked <code>optional</code> are not required and are resolved on OpenAI's end if not supplied.</p>

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="./examples">examples</a> folder.
