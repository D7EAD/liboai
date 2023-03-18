#include "../include/core/netimpl.h"

liboai::netimpl::CurlHolder::CurlHolder() noexcept {
	curl_easy_get_mutex_().lock();
	this->curl_ = curl_easy_init();		
	curl_easy_get_mutex_().unlock();
}

liboai::netimpl::Session::~Session() noexcept {
	if (this->curl_) {
		curl_easy_cleanup(this->curl_);
		this->curl_ = nullptr;
	}

	if (this->headers) {
		curl_slist_free_all(this->headers);
		this->headers = nullptr;
	}
	
	if (this->form) {
		curl_formfree(this->form);
		this->form = nullptr;
	}
}

void liboai::netimpl::Session::Prepare() {
	// add parameters to base url
	if (!this->parameter_string_.empty()) {
		this->url_ += "?";
		this->url_ += this->parameter_string_;
	}
	this->url_str = this->url_;
	
	curl_easy_setopt(this->curl_, CURLOPT_URL, this->url_.c_str());

	// set proxy if available
	const std::string protocol = url_.substr(0, url_.find(':'));
	if (proxies_.has(protocol)) {
		curl_easy_setopt(this->curl_, CURLOPT_PROXY, proxies_[protocol]);
		if (proxyAuth_.has(protocol)) {
			curl_easy_setopt(this->curl_, CURLOPT_PROXYUSERNAME, proxyAuth_.GetUsername(protocol));
			curl_easy_setopt(this->curl_, CURLOPT_PROXYPASSWORD, proxyAuth_.GetPassword(protocol));
		}
	}

	// accept all encoding types
	curl_easy_setopt(this->curl_, CURLOPT_ACCEPT_ENCODING, "");

	#if LIBCURL_VERSION_MAJOR >= 7
		#if LIBCURL_VERSION_MINOR >= 71
			curl_easy_setopt(this->curl_, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);
		#endif
	#endif

	// set string the response will be sent to
	if (!this->write_.callback) {
		curl_easy_setopt(this->curl_, CURLOPT_WRITEFUNCTION, liboai::netimpl::components::writeFunction);
		curl_easy_setopt(this->curl_, CURLOPT_WRITEDATA, &this->response_string_);
	}

	// set string the raw headers will be sent to
	curl_easy_setopt(this->curl_, CURLOPT_HEADERFUNCTION, liboai::netimpl::components::writeFunction);
	curl_easy_setopt(this->curl_, CURLOPT_HEADERDATA, &this->header_string_);
}

void liboai::netimpl::Session::PrepareDownloadInternal() {
	if (!this->parameter_string_.empty()) {
		this->url_ += "?";
		this->url_ += this->parameter_string_;
	}
	this->url_str = this->url_;
	
	curl_easy_setopt(this->curl_, CURLOPT_URL, this->url_.c_str());

	const std::string protocol = url_.substr(0, url_.find(':'));
	if (proxies_.has(protocol)) {
		curl_easy_setopt(this->curl_, CURLOPT_PROXY, proxies_[protocol]);
		if (proxyAuth_.has(protocol)) {
			curl_easy_setopt(this->curl_, CURLOPT_PROXYUSERNAME, proxyAuth_.GetUsername(protocol));
			curl_easy_setopt(this->curl_, CURLOPT_PROXYPASSWORD, proxyAuth_.GetPassword(protocol));
		}
	}

	curl_easy_setopt(this->curl_, CURLOPT_HEADERFUNCTION, liboai::netimpl::components::writeFunction);
	curl_easy_setopt(this->curl_, CURLOPT_HEADERDATA, &this->header_string_);
}

CURLcode liboai::netimpl::Session::Perform() {
	return curl_easy_perform(this->curl_);
}

liboai::Response liboai::netimpl::Session::BuildResponseObject() {
	// fill status line and reason
	this->ParseResponseHeader(this->header_string_, &this->status_line, &this->reason);
	
	// get status code
	curl_easy_getinfo(this->curl_, CURLINFO_RESPONSE_CODE, &this->status_code);

	// get elapsed time
	curl_easy_getinfo(this->curl_, CURLINFO_TOTAL_TIME, &this->elapsed);
	
	// get url
	curl_easy_getinfo(this->curl_, CURLINFO_EFFECTIVE_URL, &this->url_str);

	// fill content
	this->content = this->response_string_;
	
	return liboai::Response {
		std::move(this->url_str),
		std::move(this->content),
		std::move(this->status_line),
		std::move(this->reason),
		this->status_code,
		this->elapsed
	};
}

liboai::Response liboai::netimpl::Session::Complete() {
	this->hasBody = false;
	return this->BuildResponseObject();
}

liboai::Response liboai::netimpl::Session::CompleteDownload() {
	curl_easy_setopt(this->curl_, CURLOPT_HEADERFUNCTION, nullptr);
	curl_easy_setopt(this->curl_, CURLOPT_HEADERDATA, 0);
	this->hasBody = false;
	return this->BuildResponseObject();
}

void liboai::netimpl::Session::PrepareGet() {
	if (this->hasBody) {
		curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
		curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, "GET");
	}
	else {
		curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
		curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, nullptr);
		curl_easy_setopt(this->curl_, CURLOPT_HTTPGET, 1L);
	}
	this->Prepare();
}

liboai::Response liboai::netimpl::Session::Get() {
	this->PrepareGet();
	this->Perform();
	return Complete();
}

void liboai::netimpl::Session::PreparePost() {
	curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);

	if (this->hasBody) {
		curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, nullptr);
	}
	else {
		curl_easy_setopt(this->curl_, CURLOPT_POSTFIELDS, "");
		curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, "POST");
	}
	this->Prepare();
}

liboai::Response liboai::netimpl::Session::Post() {
	this->PreparePost();
	Perform();
	return Complete();
}

void liboai::netimpl::Session::PrepareDelete() {
	curl_easy_setopt(this->curl_, CURLOPT_HTTPGET, 0L);
	curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
	curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, "DELETE");
	this->Prepare();
}

liboai::Response liboai::netimpl::Session::Delete() {
	this->PrepareDelete();
	Perform();
	return Complete();
}

void liboai::netimpl::Session::PrepareDownload(std::ofstream& file) {
	curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
	curl_easy_setopt(this->curl_, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(this->curl_, CURLOPT_WRITEFUNCTION, liboai::netimpl::components::writeFileFunction);
	curl_easy_setopt(this->curl_, CURLOPT_WRITEDATA, &file);
	curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, nullptr);

	this->PrepareDownloadInternal();
}

liboai::Response liboai::netimpl::Session::Download(std::ofstream& file) {
	this->PrepareDownload(file);
	this->Perform();
	return CompleteDownload();
}

void liboai::netimpl::Session::ParseResponseHeader(const std::string& headers, std::string* status_line, std::string* reason) {
    std::vector<std::string> lines;
    std::istringstream stream(headers);
    {
        std::string line;
        while (std::getline(stream, line, '\n')) {
            lines.push_back(line);
        }
    }

    for (std::string& line : lines) {
        if (line.substr(0, 5) == "HTTP/") {
            // set the status_line if it was given
            if ((status_line != nullptr) || (reason != nullptr)) {
                line.resize(std::min<size_t>(line.size(), line.find_last_not_of("\t\n\r ") + 1));
                if (status_line != nullptr) {
                    *status_line = line;
                }

                // set the reason if it was given
                if (reason != nullptr) {
                    const size_t pos1 = line.find_first_of("\t ");
                    size_t pos2 = std::string::npos;
                    if (pos1 != std::string::npos) {
                        pos2 = line.find_first_of("\t ", pos1 + 1);
                    }
                    if (pos2 != std::string::npos) {
                        line.erase(0, pos2 + 1);
                        *reason = line;
                    }
                }
            }
        }

        if (line.length() > 0) {
            const size_t found = line.find(':');
            if (found != std::string::npos) {
                std::string value = line.substr(found + 1);
                value.erase(0, value.find_first_not_of("\t "));
                value.resize(std::min<size_t>(value.size(), value.find_last_not_of("\t\n\r ") + 1));
            }
        }
    }
}

void liboai::netimpl::Session::SetOption(const components::Url& url) {
	this->SetUrl(url);
}

void liboai::netimpl::Session::SetUrl(const components::Url& url) {
	this->url_ = url.str();
}

void liboai::netimpl::Session::SetOption(const components::Body& body) {
	this->SetBody(body);
}

void liboai::netimpl::Session::SetBody(const components::Body& body) {
	this->hasBody = true;
	curl_easy_setopt(this->curl_, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(body.str().length()));
	curl_easy_setopt(this->curl_, CURLOPT_POSTFIELDS, body.c_str());
}

void liboai::netimpl::Session::SetOption(components::Body&& body) {
	this->SetBody(std::move(body));
}

void liboai::netimpl::Session::SetBody(components::Body&& body) {
	this->hasBody = true;
	curl_easy_setopt(this->curl_, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(body.str().length()));
	curl_easy_setopt(this->curl_, CURLOPT_COPYPOSTFIELDS, body.c_str());
}

void liboai::netimpl::Session::SetOption(const components::Multipart& multipart) {
	this->SetMultipart(multipart);
}

void liboai::netimpl::Session::SetMultipart(const components::Multipart& multipart) {
    curl_httppost* lastptr = nullptr;

    for (const auto& part : multipart.parts) {
        std::vector<curl_forms> formdata;
        if (!part.content_type.empty()) {
            formdata.push_back({CURLFORM_CONTENTTYPE, part.content_type.c_str()});
        }
        if (part.is_file) {
            for (const auto& file : part.files) {
                formdata.push_back({CURLFORM_COPYNAME, part.name.c_str()});
                formdata.push_back({CURLFORM_FILE, file.filepath.c_str()});
                if (file.hasOverridedFilename()) {
                    formdata.push_back({CURLFORM_FILENAME, file.overrided_filename.c_str()});
                }
                formdata.push_back({CURLFORM_END, nullptr});
                curl_formadd(&this->form, &lastptr, CURLFORM_ARRAY, formdata.data(), CURLFORM_END);
                formdata.clear();
            }
        } else if (part.is_buffer) {
            curl_formadd(&this->form, &lastptr, CURLFORM_COPYNAME, part.name.c_str(), CURLFORM_BUFFER, part.value.c_str(), CURLFORM_BUFFERPTR, part.data, CURLFORM_BUFFERLENGTH, part.datalen, CURLFORM_END);
        } else {
            formdata.push_back({CURLFORM_COPYNAME, part.name.c_str()});
            formdata.push_back({CURLFORM_COPYCONTENTS, part.value.c_str()});
            formdata.push_back({CURLFORM_END, nullptr});
            curl_formadd(&this->form, &lastptr, CURLFORM_ARRAY, formdata.data(), CURLFORM_END);
        }
    }
    curl_easy_setopt(this->curl_, CURLOPT_HTTPPOST, this->form);
	this->hasBody = true;
}

void liboai::netimpl::Session::SetOption(components::Multipart&& multipart) {
	this->SetMultipart(std::move(multipart));
}

void liboai::netimpl::Session::SetMultipart(components::Multipart&& multipart) {
	curl_httppost* lastptr = nullptr;

	for (const auto& part : multipart.parts) {
		std::vector<curl_forms> formdata;
		if (!part.content_type.empty()) {
			formdata.push_back({ CURLFORM_CONTENTTYPE, part.content_type.c_str() });
		}
		if (part.is_file) {
			for (const auto& file : part.files) {
				formdata.push_back({ CURLFORM_COPYNAME, part.name.c_str() });
				formdata.push_back({ CURLFORM_FILE, file.filepath.c_str() });
				if (file.hasOverridedFilename()) {
					formdata.push_back({ CURLFORM_FILENAME, file.overrided_filename.c_str() });
				}
				formdata.push_back({ CURLFORM_END, nullptr });
				curl_formadd(&this->form, &lastptr, CURLFORM_ARRAY, formdata.data(), CURLFORM_END);
				formdata.clear();
			}
		}
		else if (part.is_buffer) {
			curl_formadd(&this->form, &lastptr, CURLFORM_COPYNAME, part.name.c_str(), CURLFORM_BUFFER, part.value.c_str(), CURLFORM_BUFFERPTR, part.data, CURLFORM_BUFFERLENGTH, part.datalen, CURLFORM_END);
		}
		else {
			formdata.push_back({ CURLFORM_COPYNAME, part.name.c_str() });
			formdata.push_back({ CURLFORM_COPYCONTENTS, part.value.c_str() });
			formdata.push_back({ CURLFORM_END, nullptr });
			curl_formadd(&this->form, &lastptr, CURLFORM_ARRAY, formdata.data(), CURLFORM_END);
		}
	}
	curl_easy_setopt(this->curl_, CURLOPT_HTTPPOST, this->form);
	this->hasBody = true;
}

std::string liboai::netimpl::CurlHolder::urlEncode(const std::string& s) {
	char* output = curl_easy_escape(this->curl_, s.c_str(), static_cast<int>(s.length()));
	if (output) {
		std::string result = output;
		curl_free(output);
		return result;
	}
	return "";
}

std::string liboai::netimpl::CurlHolder::urlDecode(const std::string& s) {
	char* output = curl_easy_unescape(this->curl_, s.c_str(), static_cast<int>(s.length()), nullptr);
	if (output) {
		std::string result = output;
		curl_free(output);
		return result;
	}
	return "";
}

std::string liboai::netimpl::components::urlEncodeHelper(const std::string& s) {
	CurlHolder c;
	return c.urlEncode(s);
}

std::string liboai::netimpl::components::urlDecodeHelper(const std::string& s) {
	CurlHolder c;
	return c.urlDecode(s);
}

size_t liboai::netimpl::components::writeUserFunction(char* ptr, size_t size, size_t nmemb, const WriteCallback* write) {
	size *= nmemb;
	return (*write)({ ptr, size }) ? size : 0;
}

size_t liboai::netimpl::components::writeFunction(char* ptr, size_t size, size_t nmemb, std::string* data) {
	size *= nmemb;
	data->append(ptr, size);
	return size;
}

size_t liboai::netimpl::components::writeFileFunction(char* ptr, size_t size, size_t nmemb, std::ofstream* file) {
	size *= nmemb;
	file->write(ptr, static_cast<std::streamsize>(size));
	return size;
}

long liboai::netimpl::components::Timeout::Milliseconds() const {
	static_assert(std::is_same<std::chrono::milliseconds, decltype(this->ms)>::value, "Following casting expects milliseconds.");

	if (ms.count() > static_cast<std::chrono::milliseconds::rep>((std::numeric_limits<long>::max)())) {
		throw std::overflow_error("cpr::Timeout: timeout value overflow: " + std::to_string(ms.count()) + " ms.");
	}

	if (ms.count() < static_cast<std::chrono::milliseconds::rep>((std::numeric_limits<long>::min)())) {
		throw std::underflow_error("cpr::Timeout: timeout value underflow: " + std::to_string(ms.count()) + " ms.");
	}

	return static_cast<long>(ms.count());
}

liboai::netimpl::components::Files::iterator liboai::netimpl::components::Files::begin() {
	return this->files.begin();
}

liboai::netimpl::components::Files::iterator liboai::netimpl::components::Files::end() {
	return this->files.end();
}

liboai::netimpl::components::Files::const_iterator liboai::netimpl::components::Files::begin() const {
	return this->files.begin();
}

liboai::netimpl::components::Files::const_iterator liboai::netimpl::components::Files::end() const {
	return this->files.end();
}

liboai::netimpl::components::Files::const_iterator liboai::netimpl::components::Files::cbegin() const {
	return this->files.cbegin();
}

liboai::netimpl::components::Files::const_iterator liboai::netimpl::components::Files::cend() const {
	return this->files.cend();
}

void liboai::netimpl::components::Files::emplace_back(const File& file) {
	this->files.emplace_back(file);
}

void liboai::netimpl::components::Files::push_back(const File& file) {
	this->files.push_back(file);
}

void liboai::netimpl::components::Files::pop_back() {
	this->files.pop_back();
}

liboai::netimpl::components::Multipart::Multipart(const std::initializer_list<Part>& parts)
	: parts{ parts } {}

liboai::netimpl::components::Parameters::Parameters(const std::initializer_list<Parameter>& parameters) {
	this->Add(parameters);
}

void liboai::netimpl::components::Parameters::Add(const std::initializer_list<Parameter>& parameters) {
	for (const auto& parameter : parameters) {
		this->parameters_.emplace_back(parameter.key, parameter.value);
	}
}

void liboai::netimpl::components::Parameters::Add(const Parameter& parameter) {
	this->parameters_.emplace_back(parameter.key, parameter.value);
}

bool liboai::netimpl::components::Parameters::Empty() const {
	return this->parameters_.empty();
}

std::string liboai::netimpl::components::Parameters::BuildParameterString() const {
	std::string parameter_string;
	
	if (this->parameters_.size() == 1) {
		parameter_string += this->parameters_.front().key + "=" + this->parameters_.front().value;
	}
	else {
		for (const auto& parameter : this->parameters_) {
			parameter_string += parameter.key + "=" + parameter.value + "&";
		}
		parameter_string.pop_back();
	}

	return parameter_string;
}

void liboai::netimpl::Session::SetOption(const components::Header& header) {
	this->SetHeader(header);
}

void liboai::netimpl::Session::SetHeader(const components::Header& header) {
    for (const std::pair<const std::string, std::string>& item : header) {
        std::string header_string = item.first;
        if (item.second.empty()) {
            header_string += ";";
        } else {
            header_string += ": " + item.second;
        }

        curl_slist* temp = curl_slist_append(this->headers, header_string.c_str());
        if (temp) {
			this->headers = temp;
        }
    }

    curl_slist* temp = curl_slist_append(this->headers, "Transfer-Encoding:chunked");
    if (temp) {
		this->headers = temp;
    }

	// remove preset curl headers for files >1MB
    temp = curl_slist_append(this->headers, "Expect:");
    if (temp) {
		this->headers = temp;
    }

    curl_easy_setopt(this->curl_, CURLOPT_HTTPHEADER, this->headers);
}

void liboai::netimpl::Session::SetOption(const components::Parameters& parameters) {
	this->SetParameters(parameters);
}

void liboai::netimpl::Session::SetParameters(const components::Parameters& parameters) {
	if (!parameters.Empty()) {
		this->parameter_string_ = parameters.BuildParameterString();
	}
}

void liboai::netimpl::Session::SetOption(components::Parameters&& parameters) {
	this->SetParameters(std::move(parameters));
}

void liboai::netimpl::Session::SetParameters(components::Parameters&& parameters) {
	if (!parameters.Empty()) {
		this->parameter_string_ = parameters.BuildParameterString();
	}
}

void liboai::netimpl::Session::SetOption(const components::Timeout& timeout) {
	this->SetTimeout(timeout);
}

void liboai::netimpl::Session::SetTimeout(const components::Timeout& timeout) {
	curl_easy_setopt(this->curl_, CURLOPT_TIMEOUT_MS, timeout.Milliseconds());
}

void liboai::netimpl::Session::SetOption(const components::Proxies& proxies) {
	this->SetProxies(proxies);
}

void liboai::netimpl::Session::SetProxies(const components::Proxies& proxies) {
	this->proxies_ = proxies;
}

void liboai::netimpl::Session::SetOption(components::Proxies&& proxies) {
	this->SetProxies(std::move(proxies));
}

void liboai::netimpl::Session::SetProxies(components::Proxies&& proxies) {
	this->proxies_ = std::move(proxies);
}

void liboai::netimpl::Session::SetOption(const components::ProxyAuthentication& proxy_auth) {
	this->SetProxyAuthentication(proxy_auth);
}

void liboai::netimpl::Session::SetProxyAuthentication(const components::ProxyAuthentication& proxy_auth) {
	this->proxyAuth_ = proxy_auth;
}

void liboai::netimpl::Session::SetOption(components::ProxyAuthentication&& proxy_auth) {
	this->SetProxyAuthentication(std::move(proxy_auth));
}

void liboai::netimpl::Session::SetProxyAuthentication(components::ProxyAuthentication&& proxy_auth) {
	this->proxyAuth_ = std::move(proxy_auth);
}

void liboai::netimpl::Session::SetOption(const components::WriteCallback& write) {
	this->SetWriteCallback(write);
}

void liboai::netimpl::Session::SetWriteCallback(const components::WriteCallback& write) {
	curl_easy_setopt(this->curl_, CURLOPT_WRITEFUNCTION, components::writeUserFunction);
	this->write_ = write;
	curl_easy_setopt(this->curl_, CURLOPT_WRITEDATA, &this->write_);
}

liboai::netimpl::components::Proxies::Proxies(const std::initializer_list<std::pair<const std::string, std::string>>& hosts)
	: hosts_{ hosts } {}

liboai::netimpl::components::Proxies::Proxies(const std::map<std::string, std::string>& hosts)
	: hosts_{hosts} {}

bool liboai::netimpl::components::Proxies::has(const std::string& protocol) const {
	return hosts_.count(protocol) > 0;
}

const std::string& liboai::netimpl::components::Proxies::operator[](const std::string& protocol) {
	return hosts_[protocol];
}

liboai::netimpl::components::EncodedAuthentication::~EncodedAuthentication() noexcept {
	this->SecureStringClear(this->username);
	this->SecureStringClear(this->password);
}

const std::string& liboai::netimpl::components::EncodedAuthentication::GetUsername() const {
	return this->username;
}

const std::string& liboai::netimpl::components::EncodedAuthentication::GetPassword() const {
	return this->password;
}

#if defined(__STDC_LIB_EXT1__)
void liboai::netimpl::components::EncodedAuthentication::SecureStringClear(std::string& s) {
	if (s.empty()) {
		return;
	}
	memset_s(&s.front(), s.length(), 0, s.length());
	s.clear();
}
#elif defined(_WIN32)
void liboai::netimpl::components::EncodedAuthentication::SecureStringClear(std::string& s) {
	if (s.empty()) {
		return;
	}
	SecureZeroMemory(&s.front(), s.length());
	s.clear();
}
#else
#if defined(__clang__)
#pragma clang optimize off // clang
#elif defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW32__) || defined(__MINGW64__)
#pragma GCC push_options   // g++
#pragma GCC optimize("O0") // g++
#endif
void liboai::netimpl::components::EncodedAuthentication::SecureStringClear(std::string& s) {
	if (s.empty()) {
		return;
	}
	
	char* ptr = &(s[0]);
	memset(ptr, '\0', s.length());
	s.clear();
}

#if defined(__clang__)
#pragma clang optimize on // clang
#elif defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW32__) || defined(__MINGW64__)
#pragma GCC pop_options // g++
#endif
#endif

bool liboai::netimpl::components::ProxyAuthentication::has(const std::string& protocol) const {
	return proxyAuth_.count(protocol) > 0;
}

const char* liboai::netimpl::components::ProxyAuthentication::GetUsername(const std::string& protocol) {
	return proxyAuth_[protocol].username.c_str();
}

const char* liboai::netimpl::components::ProxyAuthentication::GetPassword(const std::string& protocol) {
	return proxyAuth_[protocol].password.c_str();
}