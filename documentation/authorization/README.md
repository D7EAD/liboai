<h1>Authorization</h1>
<p>The <code>Authorization</code> class is defined in <code>authorization.h</code> at <code>liboai::Authorization</code>. This class is responsible for sharing all set authorization information with all component classes in <code>liboai</code>.

All authorization information should be set prior to the calling of any component methods such as <code>Images</code>, <code>Embeddings</code>, and so on. Failure to do so will result in a <code>liboai::OpenAIException</code> due to authorization failure on OpenAI's end.</p>

<br>
<h2>Methods</h2>
<p>This document covers the method(s) located in <code>authorization.h</code>. You can find their function signature(s) below.</p>

<h3>Get Authorizer</h3>
<p>Returns a reference to the <code>liboai::Authorization</code> singleton shared among all components.</p>

```cpp
static Authorization& Authorizer() noexcept;
```

<h3>Set API Key</h3>
<p>Sets the API key to use in subsequent component calls.</p>

```cpp
bool SetKey(std::string_view key) noexcept;
```

<h3>Set API Key (File)</h3>
<p>Sets the API key to use in subsequent component calls from data found in file at path.</p>

```cpp
bool SetKeyFile(const std::filesystem::path& path) noexcept;
```

<h3>Set API Key (Environment Variable)</h3>
<p>Sets the API key to use in subsequent component calls from an environment variable.</p>

```cpp
bool SetKeyEnv(std::string_view var) noexcept;
```
 
<h3>Set Organization ID</h3>
<p>Sets the organization ID to send in subsequent component calls.</p>

```cpp
bool SetOrganization(std::string_view org) noexcept;
```

<h3>Set Organization ID (File)</h3>
<p>Sets the organization ID to send in subsequent component calls from data found in file at path.</p>

```cpp
bool SetOrganizationFile(const std::filesystem::path& path) noexcept;
```

<h3>Set Organization ID (Environment Variable)</h3>
<p>Sets the organization ID to send in subsequent component calls from an environment variable.</p>

```cpp
bool SetOrganizationEnv(std::string_view var) noexcept;
```

<h3>Set Proxies</h3>
<p>Sets the proxy, or proxies, to use in subsequent component calls.</p>

```cpp
void SetProxies(const std::initializer_list<std::pair<const std::string, std::string>>& hosts) noexcept;
void SetProxies(std::initializer_list<std::pair<const std::string, std::string>>&& hosts) noexcept;
void SetProxies(const std::map<std::string, std::string>& hosts) noexcept;
void SetProxies(std::map<std::string, std::string>&& hosts) noexcept;
```

<h3>Set Proxy Authentication</h3>
<p>Sets the username and password to use when using a certain proxy protocol.</p>

```cpp
void SetProxyAuth(const std::map<std::string, netimpl::components::EncodedAuthentication>& proto_up) noexcept;
```

<h3>Set Timeout</h3>
<p>Sets the timeout in milliseconds for the library to use in component calls.</p>

```cpp
void SetMaxTimeout(int32_t ms) noexcept
```

<h3>Get Key</h3>
<p>Returns the currently set API key.</p>

```cpp
constexpr const std::string& GetKey() const noexcept;
```

<h3>Get Organization ID</h3>
<p>Returns the currently set organization ID.</p>

```cpp
constexpr const std::string& GetOrganization() const noexcept;
```


<h3>Get Proxies</h3>
<p>Returns the currently set proxies.</p>

```cpp
netimpl::components::Proxies GetProxies() const noexcept;
```

<h3>Get Proxy Authentication</h3>
<p>Returns the currently set proxy authentication information.</p>

```cpp
netimpl::components::ProxyAuthentication GetProxyAuth() const noexcept;
```

<h3>Get Timeout</h3>
<p>Returns the currently set timeout.</p>

```cpp
netimpl::components::Timeout GetMaxTimeout() const noexcept;
```

<h3>Get Authorization Headers</h3>
<p>Returns the currently set authorization headers based on set information.</p>

```cpp
constexpr const netimpl::components::Header& GetAuthorizationHeaders() const noexcept;
```

<br>
<h2>Example Usage</h2>
<p>For example usage of the above function(s), please refer to the <a href="./examples">examples</a> folder.
