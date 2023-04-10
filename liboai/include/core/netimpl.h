#pragma once

/*
	Copyright (c) 2017-2021 Huu Nguyen
	Copyright (c) 2022 libcpr and many other contributors

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.

	netimpl.h : Holds the internal network control-flow implementation.
		This header file provides the internal interface(s) used to
		allow files such as network.h to properly work. It contains
		the internal cURL network wrapping functionality and all
		other network-related functionality.
		
		This was created to remove the dependency on the library
		cURL for People (CPR).
*/

#if defined(__linux__) || defined(__APPLE__)
	#define LIBOAI_EXPORT
#else
	#define LIBOAI_EXPORT __declspec(dllexport)
#endif

#include <fstream>
#include <optional>	
#include <mutex>
#include <future>
#include <sstream>
#include <curl/curl.h>
#include "response.h"

namespace liboai {
	namespace netimpl {		
		static bool _flag = false;
		
		void ErrorCheck(CURLcode* ecodes, size_t size, std::string_view where);
		void ErrorCheck(CURLFORMcode* ecodes, size_t size, std::string_view where);
		void ErrorCheck(CURLcode ecode, std::string_view where);
		void ErrorCheck(CURLFORMcode ecode, std::string_view where);

		class CurlHolder {
			public:
				CurlHolder();
				virtual ~CurlHolder() = default;
				CurlHolder(const CurlHolder&) = delete;
				CurlHolder(CurlHolder&&) = delete;

				CurlHolder& operator=(const CurlHolder&) = delete;
				CurlHolder& operator=(CurlHolder&&) = delete;

				std::string urlEncode(const std::string& s);
				std::string urlDecode(const std::string& s);

			private:
				static std::mutex& curl_easy_get_mutex_() {
					static std::mutex g_curl_mutex;
					return g_curl_mutex;
				}

			protected:
				CURL* curl_ = nullptr;
		};

		/*
			Contains all components that can be passed to below free methods
			Get, Post, and Delete such as Url, Headers, Body, Multipart,
			etc.
		*/
		namespace components {
			template <class T>
			class StringHolder {
				public:
					StringHolder() = default;
					explicit StringHolder(std::string str) : str_(std::move(str)) {}
					explicit StringHolder(std::string_view str) : str_(str) {}
					explicit StringHolder(const char* str) : str_(str) {}
					StringHolder(const char* str, size_t len) : str_(str, len) {}
					StringHolder(const std::initializer_list<std::string> args) {
						str_ = std::accumulate(args.begin(), args.end(), str_);
					}
					StringHolder(const StringHolder& other) = default;
					StringHolder(StringHolder&& old) noexcept = default;
					virtual ~StringHolder() = default;

					StringHolder& operator=(StringHolder&& old) noexcept = default;

					StringHolder& operator=(const StringHolder& other) = default;

					explicit operator std::string() const {
						return str_;
					}

					T operator+(const char* rhs) const {
						return T(str_ + rhs);
					}

					T operator+(const std::string& rhs) const {
						return T(str_ + rhs);
					}

					T operator+(const StringHolder<T>& rhs) const {
						return T(str_ + rhs.str_);
					}

					void operator+=(const char* rhs) {
						str_ += rhs;
					}
					void operator+=(const std::string& rhs) {
						str_ += rhs;
					}
					void operator+=(const StringHolder<T>& rhs) {
						str_ += rhs;
					}

					bool operator==(const char* rhs) const {
						return str_ == rhs;
					}
					bool operator==(const std::string& rhs) const {
						return str_ == rhs;
					}
					bool operator==(const StringHolder<T>& rhs) const {
						return str_ == rhs.str_;
					}

					bool operator!=(const char* rhs) const {
						return str_.c_str() != rhs;
					}
					bool operator!=(const std::string& rhs) const {
						return str_ != rhs;
					}
					bool operator!=(const StringHolder<T>& rhs) const {
						return str_ != rhs.str_;
					}

					const std::string& str() {
						return str_;
					}
					const std::string& str() const {
						return str_;
					}
					const char* c_str() const {
						return str_.c_str();
					}
					const char* data() const {
						return str_.data();
					}

				protected:
					std::string str_{};
			};

			struct File final {
				explicit File(std::string p_filepath, const std::string& p_overrided_filename = {}) : filepath(std::move(p_filepath)), overrided_filename(p_overrided_filename) {}

				const std::string filepath;
				const std::string overrided_filename;

				bool hasOverridedFilename() const noexcept {
					return !overrided_filename.empty();
				};
			};

			class Files final {
				public:
					Files() = default;
					Files(const File& p_file) : files{ p_file } {};
					Files(const std::initializer_list<File>& p_files) : files{ p_files } {};
					Files(const std::initializer_list<std::string>& p_filepaths) {
						for (const std::string& filepath : p_filepaths) {
							files.emplace_back(File(filepath));
						}
					};
					~Files() noexcept = default;

					using iterator = std::vector<File>::iterator;
					using const_iterator = std::vector<File>::const_iterator;

					iterator begin();
					iterator end();
					const_iterator begin() const;
					const_iterator end() const;
					const_iterator cbegin() const;
					const_iterator cend() const;
					void emplace_back(const File& file);
					void push_back(const File& file);
					void pop_back();

				private:
					std::vector<File> files;
			};

			class Url final : public StringHolder<Url> {
				public:
					Url() = default;
					Url(std::string url) : StringHolder<Url>(std::move(url)) {}
					Url(std::string_view url) : StringHolder<Url>(url) {}
					Url(const char* url) : StringHolder<Url>(url) {}
					Url(const char* str, size_t len) : StringHolder<Url>(std::string(str, len)) {}
					Url(const std::initializer_list<std::string> args) : StringHolder<Url>(args) {}
					Url(const Url& other) = default;
					Url(Url&& old) noexcept = default;
					~Url() override = default;

					Url& operator=(Url&& old) noexcept = default;
					Url& operator=(const Url& other) = default;
			};

			class Body final : public StringHolder<Body> {
				public:
					Body() = default;
					Body(std::string body) : StringHolder<Body>(std::move(body)) {}
					Body(std::string_view body) : StringHolder<Body>(body) {}
					Body(const char* body) : StringHolder<Body>(body) {}
					Body(const char* str, size_t len) : StringHolder<Body>(str, len) {}
					Body(const std::initializer_list<std::string> args) : StringHolder<Body>(args) {}
					Body(const File& file) {
						std::ifstream is(file.filepath, std::ifstream::binary);
						if (!is) {
							throw std::invalid_argument("Can't open the file for HTTP request body!");
						}

						is.seekg(0, std::ios::end);
						const std::streampos length = is.tellg();
						is.seekg(0, std::ios::beg);
						std::string buffer;
						buffer.resize(static_cast<size_t>(length));
						is.read(buffer.data(), length);
						str_ = std::move(buffer);
					}
					Body(const Body& other) = default;
					Body(Body&& old) noexcept = default;
					~Body() override = default;

					Body& operator=(Body&& old) noexcept = default;
					Body& operator=(const Body& other) = default;
			};

			struct Buffer final {
				using data_t = const unsigned char*;

				template <typename Iterator>
				Buffer(Iterator begin, Iterator end, std::filesystem::path&& p_filename)
					: data{ reinterpret_cast<data_t>(&(*begin)) }, datalen{ static_cast<long>(std::distance(begin, end)) }, filename(std::move(p_filename)) {
					is_random_access_iterator(begin, end);
					static_assert(sizeof(*begin) == 1, "Only byte buffers can be used");
				}

				template <typename Iterator>
				typename std::enable_if<std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::random_access_iterator_tag>::value>::type is_random_access_iterator(Iterator /* begin */, Iterator /* end */) {}

				data_t data;
				long datalen;
				const std::filesystem::path filename;
			};

			struct Part final {
				Part(const std::string& p_name, const std::string& p_value, const std::string& p_content_type = {}) : name{ p_name }, value{ p_value }, content_type{ p_content_type }, is_file{ false }, is_buffer{ false } {}
				Part(const std::string& p_name, const std::int32_t& p_value, const std::string& p_content_type = {}) : name{ p_name }, value{ std::to_string(p_value) }, content_type{ p_content_type }, is_file{ false }, is_buffer{ false } {}
				Part(const std::string& p_name, const Files& p_files, const std::string& p_content_type = {}) : name{ p_name }, value{}, content_type{ p_content_type }, is_file{ true }, is_buffer{ false }, files{ p_files } {}
				Part(const std::string& p_name, Files&& p_files, const std::string& p_content_type = {}) : name{ p_name }, value{}, content_type{ p_content_type }, is_file{ true }, is_buffer{ false }, files{ std::move(p_files) } {}
				Part(const std::string& p_name, const Buffer& buffer, const std::string& p_content_type = {}) : name{ p_name }, value{ buffer.filename.string() }, content_type{ p_content_type }, data{ buffer.data }, datalen{ buffer.datalen }, is_file{ false }, is_buffer{ true } {}

				std::string name;
				std::string value;
				std::string content_type;
				Buffer::data_t data{ nullptr };
				long datalen{ 0 };
				bool is_file;
				bool is_buffer;

				Files files;
			};

			class Multipart final {
				public:
					Multipart(const std::initializer_list<Part>& parts);

					std::vector<Part> parts;
			};

			struct CaseInsensitiveCompare {
				bool operator()(const std::string& a, const std::string& b) const noexcept {
					return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), [](unsigned char ac, unsigned char bc) {
						return std::tolower(ac) < std::tolower(bc);
					});
				}
			};
			using Header = std::map<std::string, std::string, CaseInsensitiveCompare>;

			struct Parameter final {
				Parameter(std::string p_key, std::string p_value) : key{ std::move(p_key) }, value{ std::move(p_value) } {}

				std::string key;
				std::string value;
			};

			class Parameters final {
				public:
					Parameters() = default;
					Parameters(const std::initializer_list<Parameter>& parameters);

					void Add(const std::initializer_list<Parameter>& parameters);
					void Add(const Parameter& parameter);
					bool Empty() const;

					std::string BuildParameterString() const;

				private:
					std::vector<Parameter> parameters_;
			};

			class Timeout final {
				public:
					Timeout(const std::chrono::milliseconds& duration) : ms{ duration } {}
					Timeout(const std::int32_t& milliseconds) : Timeout{ std::chrono::milliseconds(milliseconds) } {}

					long Milliseconds() const;

					std::chrono::milliseconds ms;
			};

			class Proxies final {
				public:
					Proxies() = default;
					Proxies(const std::initializer_list<std::pair<const std::string, std::string>>& hosts);
					Proxies(const std::map<std::string, std::string>& hosts);

					bool has(const std::string& protocol) const;
					const std::string& operator[](const std::string& protocol);

				private:
					std::map<std::string, std::string> hosts_;
			};

			std::string urlEncodeHelper(const std::string& s);
			std::string urlDecodeHelper(const std::string& s);

			class ProxyAuthentication;
			class EncodedAuthentication final {
				friend ProxyAuthentication;
				
				public:
					EncodedAuthentication() = default;
					EncodedAuthentication(const std::string& p_username, const std::string& p_password) : username(urlEncodeHelper(p_username)), password(urlEncodeHelper(p_password)) {}
					EncodedAuthentication(const EncodedAuthentication& other) = default;
					EncodedAuthentication(EncodedAuthentication&& old) noexcept = default;
					virtual ~EncodedAuthentication() noexcept;

					EncodedAuthentication& operator=(EncodedAuthentication&& old) noexcept = default;
					EncodedAuthentication& operator=(const EncodedAuthentication& other) = default;

					[[nodiscard]] const std::string& GetUsername() const;
					[[nodiscard]] const std::string& GetPassword() const;

					void SecureStringClear(std::string& str);

				private:
					std::string username;
					std::string password;
			};
			
			class ProxyAuthentication final {
				public:
					ProxyAuthentication() = default;
					ProxyAuthentication(const std::initializer_list<std::pair<const std::string, EncodedAuthentication>>& auths) : proxyAuth_{auths} {}
					explicit ProxyAuthentication(const std::map<std::string, EncodedAuthentication>& auths) : proxyAuth_{auths} {}

					[[nodiscard]] bool has(const std::string& protocol) const;
					const char* GetUsername(const std::string& protocol);
					const char* GetPassword(const std::string& protocol);

				private:
					std::map<std::string, EncodedAuthentication> proxyAuth_;
			};

			class WriteCallback final {
				public:
					WriteCallback() = default;
					WriteCallback(std::function<bool(std::string data, intptr_t userdata)> p_callback, intptr_t p_userdata = 0)
						: userdata(p_userdata), callback(std::move(p_callback)) {}

					bool operator()(std::string data) const {
						return callback(std::move(data), userdata);
					}

					intptr_t userdata{};
					std::function<bool(std::string data, intptr_t userdata)> callback;
			};
			size_t writeUserFunction(char* ptr, size_t size, size_t nmemb, const WriteCallback* write);
			size_t writeFunction(char* ptr, size_t size, size_t nmemb, std::string* data);
			size_t writeFileFunction(char* ptr, size_t size, size_t nmemb, std::ofstream* file);
		}

		/*
			Class for sessions; each session is a single request.
				Each call to Network::Request should follow the
				following schema:
				
				1. Create a session object.
				2. Set the session's options.
				3. Call the session's X() method where X is the
					request method (GET, POST, etc.).
				4. Return the resulting Response object.
		*/
		class Session final : private CurlHolder {
			public:
				Session() noexcept = default;
				~Session() override;

				liboai::Response Get();
				liboai::Response Post();
				liboai::Response Delete();
				liboai::Response Download(std::ofstream& file);
				
			private:
				template <class... _Options>
				friend void set_options(Session&, _Options&&...);

				void Prepare();
				void PrepareDownloadInternal();
				CURLcode Perform();
				liboai::Response BuildResponseObject();
				liboai::Response Complete();
				liboai::Response CompleteDownload();

				void PrepareGet();
				void PreparePost();
				void PrepareDelete();
				void PrepareDownload(std::ofstream& file);

				void ParseResponseHeader(const std::string& headers,
					std::string* status_line, std::string* reason);

				void SetOption(const components::Url& url);
				void SetUrl(const components::Url& url);

				void SetOption(const components::Body& body);
				void SetBody(const components::Body& body);
				//void SetOption(components::Body&& body);
//				void SetBody(const components::Body& body);

				void SetOption(const components::Multipart& multipart);
				void SetMultipart(const components::Multipart& multipart);
				void SetOption(components::Multipart&& multipart);
				void SetMultipart(components::Multipart&& multipart);

				void SetOption(const components::Header& header);
				void SetHeader(const components::Header& header);
				
				void SetOption(const components::Parameters& parameters);
				void SetParameters(const components::Parameters& parameters);
				void SetOption(components::Parameters&& parameters);
				void SetParameters(components::Parameters&& parameters);

				void SetOption(const components::Timeout& timeout);
				void SetTimeout(const components::Timeout& timeout);

				void SetOption(const components::Proxies& proxies);
				void SetProxies(const components::Proxies& proxies);
				void SetOption(components::Proxies&& proxies);
				void SetProxies(components::Proxies&& proxies);

				void SetOption(const components::ProxyAuthentication& proxy_auth);
				void SetProxyAuthentication(const components::ProxyAuthentication& proxy_auth);
				void SetOption(components::ProxyAuthentication&& proxy_auth);
				void SetProxyAuthentication(components::ProxyAuthentication&& proxy_auth);

				void SetOption(const components::WriteCallback& write);
				void SetWriteCallback(const components::WriteCallback& write);

				long status_code = 0; double elapsed = 0.0;
				std::string status_line{}, content{}, url_str{}, reason{};
			
				// internally-used members...
				curl_slist* headers = nullptr;
				curl_httppost* form = nullptr;
				
				bool hasBody = false;
				std::string parameter_string_, url_,
					response_string_, header_string_;
				components::Proxies proxies_;
				components::ProxyAuthentication proxyAuth_;
				components::WriteCallback write_;
		};

		template <class... _Options>
		liboai::Response Get(_Options&&... options) {
			Session session;
			set_options(session, std::forward<_Options>(options)...);
			return session.Get();
		}

		template <class... _Options>
		liboai::Response Post(_Options&&... options) {
			 
			Session session;
			set_options(session, std::forward<_Options>(options)...);
			return session.Post();
		}
		
		template <class... _Options>
		liboai::Response Delete(_Options&&... options) {
			Session session;
			set_options(session, std::forward<_Options>(options)...);
			return session.Delete();
		}

		template <class... _Options>
		liboai::Response Download(std::ofstream& file, _Options&&... options) {
			Session session;
			set_options(session, std::forward<_Options>(options)...);
			return session.Download(file);
		}

		template <class... _Options>
		void set_options(Session& session, _Options&&... opts) {
			(session.SetOption(std::forward<_Options>(opts)), ...);
		}
	}
}