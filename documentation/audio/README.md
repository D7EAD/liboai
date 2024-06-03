<h1>Audio</h1>
<p>The <code>Audio</code> class is defined in <code>audio.h</code> at <code>liboai::Audio</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://beta.openai.com/docs/api-reference/audio">Audio</a> endpoint of the OpenAI API; this endpoint's functionality can be found below.</p>
- Turn audio to text.
- Turn text to audio.

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>audio.h</code>. You can find their function signature(s) below.</p>

<h3>Create a Transcription</h3>
<p>Transcribes audio into the input language. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response transcribe(
  const std::filesystem::path& file,
  const std::string& model,
  std::optional<std::string> prompt = std::nullopt,
  std::optional<std::string> response_format = std::nullopt,
  std::optional<float> temperature = std::nullopt,
  std::optional<std::string> language = std::nullopt
) const & noexcept(false);
```

<h3>Create a Transcription (async)</h3>
<p>Asynchronously transcribes audio into the input language. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse transcribe_async(
  const std::filesystem::path& file,
  const std::string& model,
  std::optional<std::string> prompt = std::nullopt,
  std::optional<std::string> response_format = std::nullopt,
  std::optional<float> temperature = std::nullopt,
  std::optional<std::string> language = std::nullopt
) const& noexcept(false);
```

<h3>Create a Translation</h3>
<p>Translates audio into English. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
liboai::Response translate(
  const std::filesystem::path& file,
  const std::string& model,
  std::optional<std::string> prompt = std::nullopt,
  std::optional<std::string> response_format = std::nullopt,
  std::optional<float> temperature = std::nullopt
) const & noexcept(false);
```

<h3>Create a Translation (async)</h3>
<p>Asynchronously translates audio into English. Returns a <code>liboai::FutureResponse</code> containing future response data.</p>

```cpp
liboai::FutureResponse translate_async(
  const std::filesystem::path& file,
  const std::string& model,
  std::optional<std::string> prompt = std::nullopt,
  std::optional<std::string> response_format = std::nullopt,
  std::optional<float> temperature = std::nullopt
) const& noexcept(false);
```

<h3>Text to speech</h3>
<p>Turn text into lifelike spoken audio. Returns a <code>liboai::Response</code> containing response data. The audio data is in the <code>content</code> field of the <code>liboai::Response</code></p>

```cpp
liboai::Response speech(
  const std::string& model,
  const std::string& voice,
  const std::string& input,
  std::optional<std::string> response_format = std::nullopt,
  std::optional<float> speed = std::nullopt
) const& noexcept(false);
```

<h3>Text to speech (async)</h3>
<p>Asynchronously turn text into lifelike spoken audio. Returns a <code>liboai::FutureResponse</code> containing response data. The audio data is in the <code>content</code> field of the <code>liboai::Response</code></p>

```cpp
liboai::FutureResponse speech_async(
  const std::string& model,
  const std::string& voice,
  const std::string& input,
  std::optional<std::string> response_format = std::nullopt,
  std::optional<float> speed = std::nullopt
) const& noexcept(false);
```

<p>All function parameters marked <code>optional</code> are not required and are resolved on OpenAI's end if not supplied.</p>

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="./examples">examples</a> folder.
