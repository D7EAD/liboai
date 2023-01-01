<h1>Images</h1>
<p>The <code>Images</code> class is defined in <code>images.h</code> at <code>liboai::Images</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://beta.openai.com/docs/api-reference/images">Images</a> endpoint of the OpenAI API; this endpoint's functionality can be found below.</p>
- Given a prompt and/or an input image, the model will generate a new image.

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>images.h</code>. You can find their function signature(s) below.</p>

<h3>Create an Image</h3>
<p>Creates an image given a prompt. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response create(
  const std::string& prompt,
  const std::optional<uint8_t>& n = std::nullopt,
  const std::optional<std::string>& size = std::nullopt,
  const std::optional<std::string>& response_format = std::nullopt,
  const std::optional<std::string>& user = std::nullopt
) const &;
```

<h3>Create Image Edit</h3>
<p>Creates an edited or extended image given an original image and a prompt. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response create_edit(
  const std::filesystem::path& image,
  const std::string& prompt,
  const std::optional<std::filesystem::path>& mask = std::nullopt,
  const std::optional<uint8_t>& n = std::nullopt,
  const std::optional<std::string>& size = std::nullopt,
  const std::optional<std::string>& response_format = std::nullopt,
  const std::optional<std::string>& user = std::nullopt
) const &;
```

<h3>Create Image Variation</h3>
<p>Creates a variation of a given image. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response create_variation(
  const std::filesystem::path& image,
  const std::optional<uint8_t>& n = std::nullopt,
  const std::optional<std::string>& size = std::nullopt,
  const std::optional<std::string>& response_format = std::nullopt,
  const std::optional<std::string>& user = std::nullopt
) const &;
```

<p>All function parameters marked <code>optional</code> are not required and are resolved on OpenAI's end if not supplied.</p>

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="/examples">examples</a> folder.
