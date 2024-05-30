<h1>Conversation</h1>

<h3>Contents</h3>
<p>You can jump to any content found on this page using the links below.
<ul>
  <li><a href="https://github.com/D7EAD/liboai/tree/v2.3.0/documentation/chat/conversation#basic-use">Basic Use</a></li>
  <li><a href="https://github.com/D7EAD/liboai/tree/v2.3.0/documentation/chat/conversation#the-use-of-system">The Use of System</a></li>
  <li><a href="https://github.com/D7EAD/liboai/tree/v2.3.0/documentation/chat/conversation#usage-pattern">Usage Pattern</a></li>  
  <li><a href="https://github.com/D7EAD/liboai/tree/v2.3.0/documentation/chat/conversation#synopsis">Synopsis</a</li>
  <li><a href="https://github.com/D7EAD/liboai/tree/v2.3.0/documentation/chat/conversation#synopsis">Methods</a></li>
</ul>

The <code>Conversation</code> class is defined at <code>liboai::Conversation</code>.

This class can most effectively be thought of as a container for any conversation(s) that one may wish to carry out with a given model using the <code>ChatCompletion</code> methods. It keeps track of the history of the conversation for subsequent calls to the methods, allows a developer to set <a href="https://platform.openai.com/docs/guides/chat/instructing-chat-models">system</a> directions, retrieve the last response, add user input, and so on.

<h3>Basic Use</h3>
  
Each method found in <code>ChatCompletion</code> requires an existing object of class <code>Conversation</code> be provided. Before providing such an object to a method such as <code>liboai::ChatCompletion::create</code>, we must first populate it--perhaps with a question to ask the model we choose, like so:

<table>
<tr>
<th>Creating a Conversation</th>
</tr>
<tr>
<td>

```cpp
#include "liboai.h"

using namespace liboai;

int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // add a message to the conversation
  convo.AddUserData("Hello, how are you?");
  
  ...
}
```

</td>
</tr>
</table>

Once we add a message to our <code>Conversation</code>, we can then supply it to a method such as <code>liboai::ChatCompletion::create</code> to begin our conversation starting with our user data, like so:
<table>
<tr>
<th>Starting the Conversation</th>
</tr>
<tr>
<td>

```cpp
int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // add a message to the conversation
  convo.AddUserData("Hello, how are you?");

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.ChatCompletion->create(
        "gpt-3.5-turbo", convo
      );

      ...
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

Assuming that our request succeeded without throwing an exception, the response to our user data in our <code>Conversation</code> can be found in our <code>Response</code> object. We must now update our <code>Conversation</code> object with the response like so:
<table>
<tr>
<th>Updating our Conversation</th>
</tr>
<tr>
<td>

```cpp
int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // add a message to the conversation
  convo.AddUserData("Hello, how are you?");

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.ChatCompletion->create(
        "gpt-3.5-turbo", convo
      );

      // update our conversation with the response
      convo.Update(response);  
  
      ...
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

After we update our <code>Conversation</code>, it now contains the original question we asked the model, as well as the response from the model. Now we can extract the response like so:
<table>
<tr>
<th>Printing the Response</th>
</tr>
<tr>
<td>

```cpp
int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // add a message to the conversation
  convo.AddUserData("Hello, how are you?");

  if (oai.auth.SetKeyEnv("OPENAI_API_KEY")) {
    try {
      Response response = oai.ChatCompletion->create(
        "gpt-3.5-turbo", convo
      );

      // update our conversation with the response
      convo.Update(response);  
  
      // print the response
      std::cout << convo.GetLastResponse() << std::endl;
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

This may print something along the lines of the following:
* "<i>As an AI language model, I do not have emotions, but I am always responsive and ready to assist. How can I help you today?</i>"

<h3>Usage Pattern</h3>
As you have hopefully noticed, there is a pattern that can be followed with <code>Conversation</code>. Generally, when we want to make use of the methods found within <code>liboai::ChatCompletion</code>, we should adhere to the following series of steps:
<ol>
  <li>Create a <code>Conversation</code> object.</li>
  <li>Set the user data (or optional, single-time system data as well), which is the user's input such as a question or a command.</li>
  <li>Provide the <codeChatCompletion::Conversation</code> object to <code>ChatCompletion::create</code> or a similar method.</li>
  <li>Update the <code>Conversation</code> object with the response from the API.</li>
  <li>Retrieve the chat model's response from the <code>Conversation</code> object.</li>
  <li>Repeat steps 2, 3, 4, and 5 until the conversation is complete.</li>
</ol>

<h3>The Use of System</h3>
Other than setting user data in our <code>Conversation</code> objects, we can also set an optional system parameter that instructs the model on how to respond. If we wish to make use of this system parameter, we can do so like so:
<table>
<tr>
<th>Setting System Data to Guide Models</th>
</tr>
<tr>
<td>

```cpp
int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // set the system message first - helps guide the model
  convo.SetSystemData("You are a helpful bot that only answers questions about OpenAI.");

  // add a message to the conversation
  convo.AddUserData("Hello, how are you?");
  
  ...
}
```

</td>
</tr>
</table>

Keep in mind that it is **highly** important to set the system data before user data. Furthermore, it is important to note that, according to OpenAI, some models (such as gpt-3.5-turbo-0301) do not always pay attention to this system data. As a result, it may be more efficient to set guiding data as user data like so:
<table>
<tr>
<th>Alternate Ways to Guide</th>
</tr>
<tr>
<td>

```cpp
int main() {
  OpenAI oai;

  // create a conversation
  Conversation convo;

  // add guiding data and a message to the conversation
  convo.AddUserData("You are a helpful bot that only answers questions about OpenAI: Hello, how are you?");

  ...
}
```

</td>
</tr>
</table>

<h3>Synopsis</h3>
With the use of <code>Conversation</code> objects, as we carry on a given conversation, our object will keep track of not only the history of the conversation we are having, but its contained context as well. That means that if we were to, at first, ask our model "When was last year's Super Bowl," and then subsequently ask it, "Who played in it," it would be aware of the context of the conversation for the second inquiry and answer accordingly.
<br>
<br>
In general, objects of class <code>liboai::Conversation</code> allow us to more easily engage in conversation with existing and future conversational chat models via the use of <code>liboai::ChatCompletion</code> methods.

<h2>Methods</h2>
Below you can find the function signature(s) of the class methods found within <code>liboai::Conversation</code>.

<h3>Constructors</h3>
<p>Constructors available to construct a <code>Conversation</code> object.</p>

```cpp
Conversation();
Conversation(const Conversation& other);
Conversation(Conversation&& old) noexcept;
Conversation(std::string_view system_data);
Conversation(std::string_view system_data, std::string_view user_data);
Conversation(std::string_view system_data, std::initializer_list<std::string_view> user_data);
Conversation(std::initializer_list<std::string_view> user_data);
explicit Conversation(const std::vector<std::string>& user_data);
```

<h3>Assignment Operators</h3>
<p>Operator overloads for assignment.</p>

```cpp
Conversation& operator=(const Conversation& other);
Conversation& operator=(Conversation&& old) noexcept;
```

<h3>Set System Data</h3>
<p>Sets the system parameter in the conversation that can be used to influence how the model may respond to input. This should always be called before setting user data, if used. Returns a <code>bool</code> indicating success.</p>

```cpp
bool SetSystemData(std::string_view data) & noexcept(false);
```

<h3>Pop System Data</h3>
<p>Removes (pops) the set system data. Returns a <code>bool</code> indicating success.</p>

```cpp
bool PopSystemData() & noexcept(false);
```


<h3>Add User Data</h3>
<p>Adds user input to the conversation, such as a command or question to pose to a model. Returns a <code>bool</code> indicating success.</p>

```cpp
bool AddUserData(std::string_view data) & noexcept(false);
bool AddUserData(std::string_view data, std::string_view name) & noexcept(false);
```

<h3>Pop User Data</h3>
<p>Removes (pops) the most recently added user input to the conversation as long as it is the tail of the conversation. Returns a <code>bool</code> indicating success.</p>

```cpp
bool PopUserData() & noexcept(false);
```

<h3>Get Last Response</h3>
<p>Retrieves the last response from the conversation if one exists. This can be called when the last item in the conversation is an answer from a chat model, such as after the conversation is updated with a successful response from <code>liboai::ChatCompletion::create</code>. Returns a non-empty <code>std::string</code> containing the response from the chat model if one exists, empty otherwise.</p>

```cpp
std::string GetLastResponse() const & noexcept;
```

<h3>Pop Last Response</h3>
<p>Removes (pops) the last response from a chat model within the conversation if the tail of the conversation is a response. This can be called to remove a chat model response from the conversation after updating the conversation with said response. Returns a <code>bool</code> indicating success.</p>

```cpp
bool PopLastResponse() & noexcept(false);
```

<h3>Check if Last Response is Function Call</h3>
<p>Returns whether the most recent response, following a call to <code>Update</code> or a complete <code>AppendStreamData</code>, contains a function_call or not. Returns a boolean indicating if the last response is a function call.</p>

```cpp
bool LastResponseIsFunctionCall() const & noexcept;
```

<h3>Get the Name of the Last Response's Function Call</h3>
<p>Returns the name of the function_call in the most recent response. This should only be called if <code>LastResponseIsFunctionCall()</code> returns true. Returns a <code>std::string</code> containing the name of the last response's function call, empty if non-existent.</p>

```cpp
std::string GetLastFunctionCallName() const & noexcept(false);
```

<h3>Get the Arguments of the Last Response's Function Call</h3>
<p>Returns the arguments of the function_call in the most recent response in their raw JSON form. This should only be called if <code>LastResponseIsFunctionCall()</code> returns true. Returns a <code>std::string</code> containing the name of the last response's arguments in JSON form, empty if non-existent.</p>

```cpp
std::string GetLastFunctionCallArguments() const & noexcept(false);	
```

<h3>Update Conversation</h3>
<p>Updates the conversation given a Response object. This method updates the conversation given a Response object. This method should only be used if <code>AppendStreamData</code> was NOT used immediately before it.

For instance, if we made a call to <code>create*()</code>, and provided a callback function to stream and, within this callback, we used <code>AppendStreamData</code> to update the conversation per message, we would NOT want to use this method. In this scenario, the <code>AppendStreamData</code> method would have already updated the conversation, so this method would be a bad idea to call afterwards. Returns a <code>bool</code> indicating success.</p>

```cpp
bool Update(std::string_view history) & noexcept(false);
bool Update(const Response& response) & noexcept(false);
```

<h3>Export Conversation</h3>
<p>Exports the entire conversation to a JSON string. This method exports the conversation to a JSON string. The JSON string can be used to save the conversation to a file. The exported string contains both the conversation and included functions, if any. Returns the JSON string representing the conversation.</p>

```cpp
std::string Export() const & noexcept(false);	
```

<h3>Import Conversation</h3>
<p>Imports a conversation from a JSON string. This method imports a conversation from a JSON string. The JSON string should be the JSON string returned from a call to <code>Export()</code>. Returns a boolean indicating success.</p>

```cpp
bool Import() const & noexcept(false);	
```

<h3>Append Stream Data</h3>
<p>Appends stream data (SSEs) from streamed methods. This method updates the conversation given a token from a streamed method. This method should be used when using streamed methods such as <code>ChatCompletion::create</code> or <code>create_async</code> with a callback supplied. This function should be called from within the stream's callback function receiving the SSEs. Returns a boolean indicating data appending success.</p>

```cpp
bool AppendStreamData(std::string data) & noexcept(false);
```

<h3>Set Function(s)</h3>
<p>Sets the functions to be used for the conversation. This method sets the functions to be used for the conversation. Returns a boolean indicating success.</p>

```cpp
bool SetFunctions(Functions functions) & noexcept(false);
```

<h3>Pop Function(s)</h3>
<p>Pops any previously set functions.</p>

```cpp
void PopFunctions() & noexcept(false);
```

<h3>Get Raw JSON Conversation</h3>
<p>Retrieves the raw JSON of the conversation; the same functionality can be achieved using the <code>operator<<(...)</code> overload. Returns a <code>std::string</code> containing the JSON of the conversation.</p> 

```cpp
std::string GetRawConversation() const & noexcept;
```

<h3>Get Raw JSON Functions</h3>
<p>Returns the raw JSON dump of the internal functions object in string format - if one exists.</p> 

```cpp
std::string GetRawFunctions() const & noexcept;
```

<h3>Get Functions JSON Object</h3>
<p>Returns the JSON object of the set functions.</p> 

```cpp
const nlohmann::json& GetFunctionsJSON() const & noexcept;
```

<h3>Get Internal JSON </h3>
<p>Retrieves a <code>const</code>-ref of the internal JSON object containing the conversation. Returns a <code>const nlohmann::json&</code> object.</p>

```cpp
const nlohmann::json& GetJSON() const & noexcept;
```

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="./examples">examples</a> folder here and in the previous directory.</p>
