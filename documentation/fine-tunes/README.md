<h1>Fine-Tunes</h1>
<p>The <code>FineTunes</code> class is defined in <code>fine_tunes.h</code> at <code>liboai::FineTunes</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://beta.openai.com/docs/api-reference/fine-tunes">Fine-tunes</a> endpoint of the OpenAI API; this endpoint's functionality can be found below.</p>
- Manage fine-tuning jobs to tailor a model to your specific training data.

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>fine_tunes.h</code>. You can find their function signature(s) below.</p>

<h3>Create a Fine-Tune.</h3>
<p>Creates a job that fine-tunes a specified model from a given dataset. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response create(
  const std::string& training_file,
  const std::optional<std::string>& validation_file = std::nullopt,
  const std::optional<std::string>& model_id = std::nullopt,
  const std::optional<uint8_t>& n_epochs = std::nullopt,
  const std::optional<uint16_t>& batch_size = std::nullopt,
  const std::optional<float>& learning_rate_multiplier = std::nullopt,
  const std::optional<float>& prompt_loss_weight = std::nullopt,
  const std::optional<bool>& compute_classification_metrics = std::nullopt,
  const std::optional<uint16_t>& classification_n_classes = std::nullopt,
  const std::optional<std::string>& classification_positive_class = std::nullopt,
  const std::optional<std::vector<float>>& classification_betas = std::nullopt,
  const std::optional<std::string>& suffix = std::nullopt
) const &;	
```

<h3>List Fine-Tunes.</h3>
<p>List your organization's fine-tuning jobs. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response list() const &;
```

<h3>Retrieve Fine-Tune.</h3>
<p>Gets info about the fine-tune job. Returns a <code>liboai::Response</code> containing response data.</p>
  
```cpp
Response retrieve(
  const std::string& fine_tune_id
) const &;
```

<h3>Cancel Fine-Tune.</h3>
<p>Immediately cancel a fine-tune job. Returns a <code>liboai::Response</code> containing response data.</p>
  
```cpp
Response cancel(
  const std::string& fine_tune_id
) const &;			
```

<h3>List Fine-Tune Events.</h3>
<p>Get fine-grained status updates for a fine-tune job. Returns a <code>liboai::Response</code> containing response data.</p>
  
```cpp
Response list_events(
  const std::string& fine_tune_id,
  const std::optional<std::function<bool(std::string, intptr_t)>>& stream = std::nullopt
) const &;
```

<p>All function parameters marked <code>optional</code> are not required and resolved on OpenAI's end if not supplied.</p>

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="/examples">examples</a> folder.
