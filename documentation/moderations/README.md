<h1>Moderations</h1>
<p>The <code>Moderations</code> class is defined in <code>moderations.h</code> at <code>liboai::Moderations</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://beta.openai.com/docs/api-reference/moderations">Moderations</a> endpoint of the OpenAI API; this endpoint's functionality can be found below.</p>
- Given a input text, outputs if the model classifies it as violating OpenAI's content policy.

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>moderations.h</code>. You can find their function signature(s) below.</p>

<h3>Create a Moderation</h3>
<p>Classifies if text violates OpenAI's Content Policy Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response create(
  const std::string& input,
  const std::optional<std::string>& model = std::nullopt
) const &;
```

<p>All function parameters marked <code>optional</code> are not required and are resolved on OpenAI's end if not supplied.</p>

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="/examples">examples</a> folder.
