<h1>Files</h1>
<p>The <code>Files</code> class is defined in <code>files.h</code> at <code>liboai::Files</code>, and its interface can ideally be accessed through a <code>liboai::OpenAI</code> object.

This class and its associated <code>liboai::OpenAI</code> interface allow access to the <a href="https://beta.openai.com/docs/api-reference/files">Files</a> endpoint of the OpenAI API; this endpoint's functionality can be found below.</p>
- Files are used to upload documents that can be used with features like Fine-tuning.

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>files.h</code>. You can find their function signature(s) below.</p>

<h3>List Files.</h3>
<p>Gets a list of files that belong to the user's organization. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response list() const &;
```

<h3>Upload File.</h3>
<p>Upload a file that contains document(s) to be used across various endpoints/features. Currently, the size of all the files uploaded by one organization can be up to 1 GB. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response create(
  const std::filesystem::path& file,
  const std::string& purpose
) const &;
```

<h3>Delete a File.</h3>
<p>Deletes a file. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response remove(
  const std::string& file_id
) const &;
```

<h3>Retrieve File.</h3>
<p>Returns information about a specific file. Returns a <code>liboai::Response</code> containing response data.</p>

```cpp
Response retrieve(
  const std::string& file_id
) const &;
```

<h3>Retrieve File Content (Download).</h3>
<p>Returns the contents of the specified file and downloads it to the provided path. Returns a <code>bool</code> indicating failure or success.</p>

```cpp
bool download(
  const std::string& file_id,
  const std::string& save_to
) const &;		
```

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="/examples">examples</a> folder.
