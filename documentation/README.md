<h1>Documentation</h1>
<p>Both above and below, you can find resources and documentation for each component of the library.</p>

<h3>Basic Usage</h3>
<p>In order to understand how to use each component of the library, it would be ideal to first understand the basic structure of the library as a whole. When using <code>liboai</code> in a project, you <b>should</b> only include one header file, <code>liboai.h</code>. This header provides an interface to all other components of the library such as <code>Images</code>, <code>Completions</code>, etc.

See below for both a correct and incorrect example.</p>
<table>
<tr>
<th>Correct</th>
<th>Incorrect</th>
</tr>
<tr>
<td>

```cpp
#include "liboai.h"

int main() {
  ...
}
```

</td>
<td>

```cpp
#include "fine_tunes.h"
#include "models.h"
// etc...

int main() {
  ...
}
```

</td>
</tr>
</table>

<br>
<p>Once we have properly included the necessary header file to use the library--and assuming symbols are linked properly--we can make use of the class in <code>liboai.h</code> to get started. At some point in our source code, we will have to choose when to define a <code>liboai::OpenAI</code> object to access component interfaces. Each component interface stored in this object offers methods associated with it, so, for instance, interface <code>Image</code> will have a method <code>create(...)</code> to generate an image from text. Each non-async method returns a <code>liboai::Response</code> containing response information whereas async methods return a <code>liboai::FutureResponse</code>. However, before we start using these methods, we must first set our authorization information--otherwise it will not work!

<code>liboai::OpenAI</code> also houses another important member, the authorization member, which is used to set authorization information (such as the API key and organization IDs) before we call the API methods. For more information on additional members found in <code>liboai::Authorization</code>, refer to the <a href="./authorization">authorization</a> folder above.

See below for both a correct and incorrect control flow when generating an image.</p>
<table>
<tr>
<th>Correct</th>
<th>Incorrect</th>
</tr>
<tr>
<td>

```cpp
#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  
  // Set our API key using an environment variable.
  // This is recommended as hard-coding API keys is
  // insecure.
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    Response response = oai.Image->create(
      "a siamese cat!"
    );
  }
  
  ...
}
```

</td>
<td>

```cpp
#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
	
  // Failure to set authorization info!
  // Will fail, exception will be thrown!
  Response response = oai.Image->create(
    "a siamese cat!"
  );
  
  ...
}
```

</td>
</tr>
</table>

<br>
<p>As you can see above, authentication-set related functions return booleans to indicate success and failure, whereas component methods will throw an exception, <code>OpenAIException</code> or <code>OpenAIRateLimited</code>, to indicate their success or failure; these should be checked for accordingly. Below you can find an exception-safe version of the above correct snippet.</p>
<table>
<tr>
<th>Correct, exception-safe</th>
</tr>
<tr>
<td>

```cpp
#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.Image->create(
        "a siamese cat!"
      );
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
    
    ...
  }
}
```

</td>
</tr>
</table>

<br>
<p>Now, once we have made a call using a component interface, we most certainly want to get the information out of it. To do this, using our knowledge of the format of the API responses, we can extract the information, such as the resulting image's URL, using JSON indexing on the <code>liboai::Response</code> object. See below for an example where we print the generated image's URL.</p>
<table>
<tr>
<th>Accessing JSON Response Data</th>
</tr>
<tr>
<td>

```cpp
#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.Image->create(
        "a siamese cat!"
      );
      std::cout << response["data"][0]["url"].get<std::string>() << std::endl;
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
```

</td>
</tr>
</table>

<br>
<p>What if we want to do more than just print the URL of the image? Why not download it right when it's done? Thankfully, <code>liboai</code> has a convenient function for that, <code>Network::Download(...)</code> (and <code>Network::DownloadAsync(...)</code>). See below for an example of downloading a freshly generated image.
<table>
<tr>
<th>Downloading a Generated Image</th>
</tr>
<td>

```cpp
#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;
  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.Image->create(
        "a siamese cat!"
      );
      Network::Download(
        "C:/some/folder/file.png",                    // to
        response["data"][0]["url"].get<std::string>() // from
      );
    }
    catch (std::exception& e) {
      std::cout << e.what() << std::endl;
    }
  }
}
```

</td>
</tr>
</table>

<br>
<p>After a successful run of the above snippet, the file found at the URL returned from the component call will be download to the path <code>C:/some/folder/file.png</code>.
<br>

<h1>Synopsis</h1>
<p>Each component interface found within <code>liboai::OpenAI</code> follows the same pattern found above. Whether you want to generate images, completions, or fine-tune models, the control flow should follow--or remain similar to--the above examples.

For detailed examples regarding individual component interfaces, refer to the appropriate folder listed above.</p>
