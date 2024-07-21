#include "../include/core/netimpl.h"

liboai::netimpl::CurlHolder::CurlHolder() {
	std::lock_guard<std::mutex> lock{ this->curl_easy_get_mutex_() };

	if (!_flag) {
		curl_version_info_data* data = curl_version_info(CURLVERSION_NOW);
		
		// if curl doesn't have ssl enabled, throw an exception
		if (!(data->features & CURL_VERSION_SSL)) {
			throw liboai::exception::OpenAIException(
				"Curl does not have SSL enabled.",
				liboai::exception::EType::E_CURLERROR,
				"liboai::netimpl::CurlHolder::CurlHolder()"
			);
		}
		else {
			// flag set to true to avoid future checks if SSL present
			_flag = true;

			#if defined(LIBOAI_DEBUG)
				_liboai_dbg(
					"[dbg] [@%s] SSL is enabled; check flag set.\n",
					__func__
				);
			#endif
		}
	}
	
	this->curl_ = curl_easy_init();
	if (!this->curl_) {
		throw liboai::exception::OpenAIException(
			curl_easy_strerror(CURLE_FAILED_INIT),
			liboai::exception::EType::E_CURLERROR,
			"liboai::netimpl::CurlHolder::CurlHolder()"
		);
	}

	#if defined(LIBOAI_DEBUG)
		curl_easy_setopt(this->curl_, CURLOPT_VERBOSE, 1L);
	#endif

	#if defined(LIBOAI_DISABLE_PEERVERIFY)
		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] LIBOAI_DISABLE_PEERVERIFY set; peer verification disabled.\n",
				__func__
			);
		#endif
		curl_easy_setopt(this->curl_, CURLOPT_SSL_VERIFYPEER, 0L);
	#else
		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] LIBOAI_DISABLE_PEERVERIFY not set; peer verification enabled.\n",
				__func__
			);
		#endif
		curl_easy_setopt(this->curl_, CURLOPT_SSL_VERIFYPEER, 1L);
	#endif
}

liboai::netimpl::CurlHolder::~CurlHolder() {
	if (this->curl_) {
		curl_easy_cleanup(this->curl_);
		this->curl_ = nullptr;
		
		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] curl_easy_cleanup() called.\n",
				__func__
			);
		#endif
	}
}

liboai::netimpl::Session::~Session() {	
	if (this->headers) {
		curl_slist_free_all(this->headers);
		this->headers = nullptr;

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] curl_slist_free_all() called.\n",
				__func__
			);
		#endif
	}
	
	#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
		if (this->form) {
			curl_formfree(this->form);
			this->form = nullptr;

			#if defined(LIBOAI_DEBUG)
				_liboai_dbg(
					"[dbg] [@%s] curl_formfree() called.\n",
					__func__
				);
			#endif
		}
	#endif
	
	#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
		if (this->mime) {
			curl_mime_free(this->mime);
			this->mime = nullptr;

			#if defined(LIBOAI_DEBUG)
				_liboai_dbg(
					"[dbg] [@%s] curl_mime_free() called.\n",
					__func__
				);
			#endif
		}
	#endif
}

void liboai::netimpl::Session::Prepare() {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[11]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	// add parameters to base url
	if (!this->parameter_string_.empty()) {
		this->url_ += "?";
		this->url_ += this->parameter_string_;
	}
	this->url_str = this->url_;

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set URL for Session (0x%p) to %s.\n",
			__func__, this, this->url_str.c_str()
		);
	#endif
		
	e[0] = curl_easy_setopt(this->curl_, CURLOPT_URL, this->url_.c_str());
	
	const std::string protocol_socket5_hostname = "socket5_hostname";
	if (proxies_.has(protocol_socket5_hostname)) {
		e[1] = curl_easy_setopt(this->curl_, CURLOPT_PROXY, proxies_[protocol_socket5_hostname].c_str());
		e[2] = curl_easy_setopt(this->curl_, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);

		if (proxyAuth_.has(protocol_socket5_hostname)) {
			e[3] = curl_easy_setopt(this->curl_, CURLOPT_PROXYUSERNAME, proxyAuth_.GetUsername(protocol_socket5_hostname));
			e[4] = curl_easy_setopt(this->curl_, CURLOPT_PROXYPASSWORD, proxyAuth_.GetPassword(protocol_socket5_hostname));
		}
  } else {
		// set proxy if available
		const std::string protocol = url_.substr(0, url_.find(':'));
		if (proxies_.has(protocol)) {
			e[1] = curl_easy_setopt(this->curl_, CURLOPT_PROXY, proxies_[protocol].c_str());
			
			#if defined(LIBOAI_DEBUG)
				_liboai_dbg(
					"[dbg] [@%s] Set CURLOPT_PROXY for Session (0x%p) to %s.\n",
					__func__, this, proxies_[protocol].c_str()
				);
			#endif

			if (proxyAuth_.has(protocol)) {
				e[2] = curl_easy_setopt(this->curl_, CURLOPT_PROXYUSERNAME, proxyAuth_.GetUsername(protocol));
				e[3] = curl_easy_setopt(this->curl_, CURLOPT_PROXYPASSWORD, proxyAuth_.GetPassword(protocol));

				#if defined(LIBOAI_DEBUG)
					_liboai_dbg(
						"[dbg] [@%s] Set CURLOPT_PROXYUSERNAME and CURLOPT_PROXYPASSWORD for Session (0x%p) to %s and %s.\n",
						__func__, this, proxyAuth_.GetUsername(protocol), proxyAuth_.GetPassword(protocol)
					);
				#endif
			}
		}
	}

	// accept all encoding types
	e[5] = curl_easy_setopt(this->curl_, CURLOPT_ACCEPT_ENCODING, "");

	#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 71)
		e[6] = curl_easy_setopt(this->curl_, CURLOPT_SSL_OPTIONS, CURLSSLOPT_NATIVE_CA);
		
		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set CURLOPT_SSL_OPTIONS for Session (0x%p) to CURLSSLOPT_NATIVE_CA.\n",
				__func__, this
			);
		#endif
	#endif

	// set string the response will be sent to
	if (!this->write_.callback) {
		e[7] = curl_easy_setopt(this->curl_, CURLOPT_WRITEFUNCTION, liboai::netimpl::components::writeFunction);
		e[8] = curl_easy_setopt(this->curl_, CURLOPT_WRITEDATA, &this->response_string_);
		
		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] No user supplied WriteCallback. Set CURLOPT_WRITEFUNCTION and CURLOPT_WRITEDATA for Session (0x%p) to 0x%p and 0x%p.\n",
				__func__, this, liboai::netimpl::components::writeFunction, &this->response_string_
			);
		#endif
	}

	// set string the raw headers will be sent to
	e[9] = curl_easy_setopt(this->curl_, CURLOPT_HEADERFUNCTION, liboai::netimpl::components::writeFunction);
	e[10] = curl_easy_setopt(this->curl_, CURLOPT_HEADERDATA, &this->header_string_);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set CURLOPT_HEADERFUNCTION and CURLOPT_HEADERDATA for Session (0x%p) to 0x%p and 0x%p.\n",
			__func__, this, liboai::netimpl::components::writeFunction, &this->header_string_
		);
	#endif

	ErrorCheck(e, 11, "liboai::netimpl::Session::Prepare()");
}

void liboai::netimpl::Session::PrepareDownloadInternal() {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[7]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	if (!this->parameter_string_.empty()) {
		this->url_ += "?";
		this->url_ += this->parameter_string_;
	}
	this->url_str = this->url_;
	
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set URL for Session (0x%p) to %s.\n",
			__func__, this, this->url_str.c_str()
		);
	#endif
	
	e[0] = curl_easy_setopt(this->curl_, CURLOPT_URL, this->url_.c_str());

	const std::string protocol_socket5_hostname = "socket5_hostname";
	if (proxies_.has(protocol_socket5_hostname)) {
		e[1] = curl_easy_setopt(this->curl_, CURLOPT_PROXY, proxies_[protocol_socket5_hostname].c_str());
		e[2] = curl_easy_setopt(this->curl_, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5_HOSTNAME);
		if (proxyAuth_.has(protocol_socket5_hostname)) {
			e[3] = curl_easy_setopt(this->curl_, CURLOPT_PROXYUSERNAME, proxyAuth_.GetUsername(protocol_socket5_hostname));
			e[4] = curl_easy_setopt(this->curl_, CURLOPT_PROXYPASSWORD, proxyAuth_.GetPassword(protocol_socket5_hostname));
		}
  } else {
		const std::string protocol = url_.substr(0, url_.find(':'));
		if (proxies_.has(protocol)) {
			e[1] = curl_easy_setopt(this->curl_, CURLOPT_PROXY, proxies_[protocol].c_str());

			#if defined(LIBOAI_DEBUG)
				_liboai_dbg(
					"[dbg] [@%s] Set CURLOPT_PROXY for Session (0x%p) to %s.\n",
					__func__, this, proxies_[protocol].c_str()
				);
			#endif
			
			if (proxyAuth_.has(protocol)) {
				e[2] = curl_easy_setopt(this->curl_, CURLOPT_PROXYUSERNAME, proxyAuth_.GetUsername(protocol));
				e[3] = curl_easy_setopt(this->curl_, CURLOPT_PROXYPASSWORD, proxyAuth_.GetPassword(protocol));

				#if defined(LIBOAI_DEBUG)
					_liboai_dbg(
						"[dbg] [@%s] Set CURLOPT_PROXYUSERNAME and CURLOPT_PROXYPASSWORD for Session (0x%p) to %s and %s.\n",
						__func__, this, proxyAuth_.GetUsername(protocol), proxyAuth_.GetPassword(protocol)
					);
				#endif
			}
		}
	}

	e[5] = curl_easy_setopt(this->curl_, CURLOPT_HEADERFUNCTION, liboai::netimpl::components::writeFunction);
	e[6] = curl_easy_setopt(this->curl_, CURLOPT_HEADERDATA, &this->header_string_);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set CURLOPT_HEADERFUNCTION and CURLOPT_HEADERDATA for Session (0x%p) to 0x%p and 0x%p.\n",
			__func__, this, liboai::netimpl::components::writeFunction, &this->header_string_
		);
	#endif
		
	ErrorCheck(e, 7, "liboai::netimpl::Session::PrepareDownloadInternal()");
}

CURLcode liboai::netimpl::Session::Perform() {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called curl_easy_perform() for Session (0x%p).\n",
			__func__, this
		);
	#endif

	CURLcode e = curl_easy_perform(this->curl_);		
	ErrorCheck(e, "liboai::netimpl::Session::Perform()");
	return e;
}

liboai::Response liboai::netimpl::Session::BuildResponseObject() {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[3]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called ParseResponseHeader() for Session (0x%p).\n",
			__func__, this
		);
	#endif	

	// fill status line and reason
	this->ParseResponseHeader(this->header_string_, &this->status_line, &this->reason);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called curl_easy_getinfo() for Session (0x%p) to get status code.\n",
			__func__, this
		);
	#endif	

	// get status code
	e[0] = curl_easy_getinfo(this->curl_, CURLINFO_RESPONSE_CODE, &this->status_code);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called curl_easy_getinfo() for Session (0x%p) to get elapsed time.\n",
			__func__, this
		);
	#endif	

	// get elapsed time
	e[1] = curl_easy_getinfo(this->curl_, CURLINFO_TOTAL_TIME, &this->elapsed);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called curl_easy_getinfo() for Session (0x%p) to get effective url.\n",
			__func__, this
		);
	#endif	

	// get url
	char* effective_url = nullptr;
	e[2] = curl_easy_getinfo(this->curl_, CURLINFO_EFFECTIVE_URL, &effective_url);
	this->url_str = (effective_url ? effective_url : "");

	ErrorCheck(e, 3, "liboai::netimpl::Session::BuildResponseObject()");

	// fill content
	this->content = this->response_string_;
	
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Constructed response object.\n",
			__func__
		);
	#endif

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
	
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called BuildResponseObject().\n",
			__func__
		);
	#endif

	return this->BuildResponseObject();
}

liboai::Response liboai::netimpl::Session::CompleteDownload() {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[2]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	e[0] = curl_easy_setopt(this->curl_, CURLOPT_HEADERFUNCTION, nullptr);
	e[1] = curl_easy_setopt(this->curl_, CURLOPT_HEADERDATA, 0);

	ErrorCheck(e, 2, "liboai::netimpl::Session::CompleteDownload()");

	this->hasBody = false;

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called BuildResponseObject().\n",
			__func__
		);
	#endif

	return this->BuildResponseObject();
}

void liboai::netimpl::Session::PrepareGet() {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[5]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	if (this->hasBody) {
		e[0] = curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
		e[1] = curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, "GET");

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set CURLOPT_NOBODY and CURLOPT_CUSTOMREQUEST for Session (0x%p) to 0L and \"GET\".\n",
				__func__, this
			);
		#endif
	}
	else {
		e[2] = curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
		e[3] = curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, nullptr);
		e[4] = curl_easy_setopt(this->curl_, CURLOPT_HTTPGET, 1L);

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set CURLOPT_NOBODY, CURLOPT_CUSTOMREQUEST and CURLOPT_HTTPGET for Session (0x%p) to 0L, nullptr and 1L.\n",
				__func__, this
			);
		#endif
	}
	
	ErrorCheck(e, 5, "liboai::netimpl::Session::PrepareGet()");

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Prepare().\n",
			__func__
		);
	#endif
		
	this->Prepare();
}

liboai::Response liboai::netimpl::Session::Get() {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called PrepareGet().\n",
			__func__
		);
	#endif

	this->PrepareGet();
	
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Perform().\n",
			__func__
		);
	#endif
	
	this->Perform();

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Complete().\n",
			__func__
		);
	#endif
		
	return Complete();
}

void liboai::netimpl::Session::PreparePost() {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[4]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	e[0] = curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
	if (this->hasBody) {
		e[1] = curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, nullptr);

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set CURLOPT_NOBODY and CURLOPT_CUSTOMREQUEST for Session (0x%p) to 0L and nullptr.\n",
				__func__, this
			);
		#endif
	}
	else {
		e[2] = curl_easy_setopt(this->curl_, CURLOPT_POSTFIELDS, "");
		e[3] = curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, "POST");

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set CURLOPT_NOBODY, CURLOPT_POSTFIELDS and CURLOPT_CUSTOMREQUEST for Session (0x%p) to 0L, \"\" and \"POST\".\n",
				__func__, this
			);
		#endif
	}
	
	ErrorCheck(e, 4, "liboai::netimpl::Session::PreparePost()");

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Prepare().\n",
			__func__
		);
	#endif

	this->Prepare();
}

liboai::Response liboai::netimpl::Session::Post() {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called PreparePost().\n",
			__func__
		);
	#endif

	this->PreparePost();

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Perform().\n",
			__func__
		);
	#endif
	
	Perform();

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Complete().\n",
			__func__
		);
	#endif

	return Complete();
}

void liboai::netimpl::Session::PrepareDelete() {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[3]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	e[0] = curl_easy_setopt(this->curl_, CURLOPT_HTTPGET, 0L);
	e[1] = curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
	e[2] = curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, "DELETE");

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set CURLOPT_HTTPGET, CURLOPT_NOBODY and CURLOPT_CUSTOMREQUEST for Session (0x%p) to 0L, 0L and \"DELETE\".\n",
			__func__, this
		);
	#endif

	ErrorCheck(e, 3, "liboai::netimpl::Session::PrepareDelete()");

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Prepare().\n",
			__func__
		);
	#endif	

	this->Prepare();
}

liboai::Response liboai::netimpl::Session::Delete() {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called PrepareDelete().\n",
			__func__
		);
	#endif
		
	this->PrepareDelete();
	
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Perform().\n",
			__func__
		);
	#endif

	Perform();

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Complete().\n",
			__func__
		);
	#endif

	return Complete();
}

void liboai::netimpl::Session::PrepareDownload(std::ofstream& file) {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[5]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	e[0] = curl_easy_setopt(this->curl_, CURLOPT_NOBODY, 0L);
	e[1] = curl_easy_setopt(this->curl_, CURLOPT_HTTPGET, 1);
	e[2] = curl_easy_setopt(this->curl_, CURLOPT_WRITEFUNCTION, liboai::netimpl::components::writeFileFunction);
	e[3] = curl_easy_setopt(this->curl_, CURLOPT_WRITEDATA, &file);
	e[4] = curl_easy_setopt(this->curl_, CURLOPT_CUSTOMREQUEST, nullptr);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set CURLOPT_NOBODY, CURLOPT_HTTPGET, CURLOPT_WRITEFUNCTION, CURLOPT_WRITEDATA and CURLOPT_CUSTOMREQUEST for Session (0x%p) to 0L, 1L, liboai::netimpl::components::writeFileFunction, &file and nullptr.\n",
			__func__, this
		);
	#endif
	
	ErrorCheck(e, 5, "liboai::netimpl::Session::PrepareDownload()");

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called PrepareDownloadInternal().\n",
			__func__
		);
	#endif

	this->PrepareDownloadInternal();
}

liboai::Response liboai::netimpl::Session::Download(std::ofstream& file) {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called PrepareDownload().\n",
			__func__
		);
	#endif
	
	this->PrepareDownload(file);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called Perform().\n",
			__func__
		);
	#endif
	
	this->Perform();
	
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called CompleteDownload().\n",
			__func__
		);
	#endif

	return CompleteDownload();
}

void liboai::netimpl::Session::ClearContext() {
  if (curl_) {
    curl_easy_reset(curl_);
  }
  status_code = 0;
  elapsed = 0.0;
  status_line.clear();
  content.clear();
  url_str.clear();
  reason.clear();

  if (this->headers) {
    curl_slist_free_all(this->headers);
    this->headers = nullptr;

#if defined(LIBOAI_DEBUG)
    _liboai_dbg("[dbg] [@%s] curl_slist_free_all() called.\n", __func__);
#endif
  }

#if LIBCURL_VERSION_MAJOR < 7 || \
    (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
  if (this->form) {
    curl_formfree(this->form);
    this->form = nullptr;

#if defined(LIBOAI_DEBUG)
    _liboai_dbg("[dbg] [@%s] curl_formfree() called.\n", __func__);
#endif
  }
#endif

#if LIBCURL_VERSION_MAJOR > 7 || \
    (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
  if (this->mime) {
    curl_mime_free(this->mime);
    this->mime = nullptr;

#if defined(LIBOAI_DEBUG)
    _liboai_dbg("[dbg] [@%s] curl_mime_free() called.\n", __func__);
#endif
  }
#endif

  hasBody = false;
  parameter_string_.clear();
  url_.clear();
  response_string_.clear();
  header_string_.clear();
}

std::unique_ptr<liboai::netimpl::Session> liboai::netimpl::Session::shared_session_ = nullptr;
void liboai::netimpl::Session::EnableShareSession(bool enable) {
	if (enable) {
		if (shared_session_ == nullptr){
			shared_session_ = std::make_unique<liboai::netimpl::Session>();
		}
	} else {
		shared_session_.reset();
	}
}

liboai::netimpl::Session* liboai::netimpl::Session::GetSharedSession(){
	return shared_session_.get();
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

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Parsed response header.\n",
			__func__
		);
	#endif
}

void liboai::netimpl::Session::SetOption(const components::Url& url) {
	this->SetUrl(url);
}

void liboai::netimpl::Session::SetUrl(const components::Url& url) {
	this->url_ = url.str();

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set base URL for Session (0x%p) to \"%s\".\n",
			__func__, this, this->url_.c_str()
		);
	#endif
}

void liboai::netimpl::Session::SetOption(const components::Body& body) {
	this->SetBody(body);
}

void liboai::netimpl::Session::SetBody(const components::Body& body) {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[2]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	this->hasBody = true;
	e[0] = curl_easy_setopt(this->curl_, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(body.str().length()));
	e[1] = curl_easy_setopt(this->curl_, CURLOPT_POSTFIELDS, body.c_str());

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set CURLOPT_POSTFIELDSIZE_LARGE and CURLOPT_POSTFIELDS for Session (0x%p) to %lld and \"%s\".\n",
			__func__, this, static_cast<curl_off_t>(body.str().length()), body.c_str()
		);
	#endif

	ErrorCheck(e, 2, "liboai::netimpl::Session::SetBody()");
}

void liboai::netimpl::Session::SetOption(components::Body&& body) {
	this->SetBody(std::move(body));
}

void liboai::netimpl::Session::SetBody(components::Body&& body) {
	// holds error codes - all init to OK to prevent errors
	// when checking unset values
	CURLcode e[2]; memset(e, CURLcode::CURLE_OK, sizeof(e));

	this->hasBody = true;
	e[0] = curl_easy_setopt(this->curl_, CURLOPT_POSTFIELDSIZE_LARGE, static_cast<curl_off_t>(body.str().length()));
	e[1] = curl_easy_setopt(this->curl_, CURLOPT_COPYPOSTFIELDS, body.c_str());

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set CURLOPT_POSTFIELDSIZE_LARGE and CURLOPT_COPYPOSTFIELDS for Session (0x%p) to %lld and \"%s\".\n",
			__func__, this, static_cast<curl_off_t>(body.str().length()), body.c_str()
		);
	#endif
		
	ErrorCheck(e, 2, "liboai::netimpl::Session::SetBody()");
}

void liboai::netimpl::Session::SetOption(const components::Multipart& multipart) {
	this->SetMultipart(multipart);
}

void liboai::netimpl::Session::SetMultipart(const components::Multipart& multipart) {
	#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
		CURLFORMcode fe[2]; memset(fe, CURLFORMcode::CURL_FORMADD_OK, sizeof(fe));
		CURLcode e;
			
		curl_httppost* lastptr = nullptr;

		for (const auto& part : multipart.parts) {
			std::vector<curl_forms> formdata;
			if (!part.content_type.empty()) {
				formdata.push_back({CURLFORM_CONTENTTYPE, part.content_type.c_str()});
			}
			if (part.is_file) {
				CURLFORMcode f;
				for (const auto& file : part.files) {
					formdata.push_back({CURLFORM_COPYNAME, part.name.c_str()});
					formdata.push_back({CURLFORM_FILE, file.filepath.c_str()});
					if (file.hasOverridedFilename()) {
						formdata.push_back({CURLFORM_FILENAME, file.overrided_filename.c_str()});
					}
					formdata.push_back({CURLFORM_END, nullptr});
					f = curl_formadd(&this->form, &lastptr, CURLFORM_ARRAY, formdata.data(), CURLFORM_END);

					// check each file
					ErrorCheck(f, "liboai::netimpl::Session::SetMultipart() @ is_file[formadd]");
					
					formdata.clear();
				}
			} else if (part.is_buffer) {
				fe[0] = curl_formadd(&this->form, &lastptr, CURLFORM_COPYNAME, part.name.c_str(), CURLFORM_BUFFER, part.value.c_str(), CURLFORM_BUFFERPTR, part.data, CURLFORM_BUFFERLENGTH, part.datalen, CURLFORM_END);
			} else {
				formdata.push_back({CURLFORM_COPYNAME, part.name.c_str()});
				formdata.push_back({CURLFORM_COPYCONTENTS, part.value.c_str()});
				formdata.push_back({CURLFORM_END, nullptr});
				fe[1] = curl_formadd(&this->form, &lastptr, CURLFORM_ARRAY, formdata.data(), CURLFORM_END);
			}
		}
		e = curl_easy_setopt(this->curl_, CURLOPT_HTTPPOST, this->form);

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set multipart for Session (0x%p) using curl_formadd() and CURLOPT_HTTPPOST.\n",
				__func__, this
			);
		#endif
	
		ErrorCheck(fe, 2, "liboai::netimpl::Session::SetMultipart()");
		ErrorCheck(e, "liboai::netimpl::Session::SetMultipart()");

		this->hasBody = true;
	#endif

	#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
		CURLcode e[6]; memset(e, CURLcode::CURLE_OK, sizeof(e));

		this->mime = curl_mime_init(this->curl_);
		if (!this->mime) {
			throw liboai::exception::OpenAIException(
				"curl_mime_init() failed",
				liboai::exception::EType::E_CURLERROR,
				"liboai::netimpl::Session::SetMultipart()"
			);
		}

		for (const auto& part : multipart.parts) {
			std::vector<curl_mimepart*> mimedata;
			if (!part.content_type.empty()) {
				mimedata.push_back(curl_mime_addpart(this->mime));
				e[0] = curl_mime_type(mimedata.back(), part.content_type.c_str());
			}
			if (part.is_file) {
				CURLcode fe[3]; memset(fe, CURLcode::CURLE_OK, sizeof(fe));
				for (const auto& file : part.files) {
					mimedata.push_back(curl_mime_addpart(this->mime));
					fe[0] = curl_mime_name(mimedata.back(), part.name.c_str());
					fe[1] = curl_mime_filedata(mimedata.back(), file.filepath.c_str());
					if (file.hasOverridedFilename()) {
						fe[2] = curl_mime_filename(mimedata.back(), file.overrided_filename.c_str());
					}

					// check each file
					ErrorCheck(fe, 3, "liboai::netimpl::Session::SetMultipart() @ is_file[mime]");
				}
			}
			else if (part.is_buffer) {
				mimedata.push_back(curl_mime_addpart(this->mime));
				e[1] = curl_mime_name(mimedata.back(), part.name.c_str());
				e[2] = curl_mime_filename(mimedata.back(), part.value.c_str());
				e[3] = curl_mime_data(mimedata.back(), reinterpret_cast<const char*>(part.data), part.datalen);
			}
			else {
				mimedata.push_back(curl_mime_addpart(this->mime));
				e[3] = curl_mime_name(mimedata.back(), part.name.c_str());
				e[4] = curl_mime_data(mimedata.back(), part.value.c_str(), CURL_ZERO_TERMINATED);
			}
		}
		e[5] = curl_easy_setopt(this->curl_, CURLOPT_MIMEPOST, this->mime);

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set multipart for Session (0x%p) using curl_mime_addpart() and CURLOPT_MIMEPOST.\n",
				__func__, this
			);
		#endif

		ErrorCheck(e, 6, "liboai::netimpl::Session::SetMultipart()");

		this->hasBody = true;
	#endif
}

void liboai::netimpl::Session::SetOption(components::Multipart&& multipart) {
	this->SetMultipart(std::move(multipart));
}

void liboai::netimpl::Session::SetMultipart(components::Multipart&& multipart) {
	#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
		CURLFORMcode fe[2]; memset(fe, CURLFORMcode::CURL_FORMADD_OK, sizeof(fe));
		CURLcode e;

		curl_httppost* lastptr = nullptr;

		for (const auto& part : multipart.parts) {
			std::vector<curl_forms> formdata;
			if (!part.content_type.empty()) {
				formdata.push_back({ CURLFORM_CONTENTTYPE, part.content_type.c_str() });
			}
			if (part.is_file) {
				CURLFORMcode f;
				for (const auto& file : part.files) {
					formdata.push_back({ CURLFORM_COPYNAME, part.name.c_str() });
					formdata.push_back({ CURLFORM_FILE, file.filepath.c_str() });
					if (file.hasOverridedFilename()) {
						formdata.push_back({ CURLFORM_FILENAME, file.overrided_filename.c_str() });
					}
					formdata.push_back({ CURLFORM_END, nullptr });
					f = curl_formadd(&this->form, &lastptr, CURLFORM_ARRAY, formdata.data(), CURLFORM_END);
					
					// check each file
					ErrorCheck(f, "liboai::netimpl::Session::SetMultipart() @ is_file[formadd]");
					
					formdata.clear();
				}
			}
			else if (part.is_buffer) {
				fe[0] = curl_formadd(&this->form, &lastptr, CURLFORM_COPYNAME, part.name.c_str(), CURLFORM_BUFFER, part.value.c_str(), CURLFORM_BUFFERPTR, part.data, CURLFORM_BUFFERLENGTH, part.datalen, CURLFORM_END);
			}
			else {
				formdata.push_back({ CURLFORM_COPYNAME, part.name.c_str() });
				formdata.push_back({ CURLFORM_COPYCONTENTS, part.value.c_str() });
				formdata.push_back({ CURLFORM_END, nullptr });
				fe[1] = curl_formadd(&this->form, &lastptr, CURLFORM_ARRAY, formdata.data(), CURLFORM_END);
			}
		}
		e = curl_easy_setopt(this->curl_, CURLOPT_HTTPPOST, this->form);

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set multipart for Session (0x%p) using curl_formadd() and CURLOPT_HTTPPOST.\n",
				__func__, this
			);
		#endif

		ErrorCheck(fe, 2, "liboai::netimpl::Session::SetMultipart()");
		ErrorCheck(e, "liboai::netimpl::Session::SetMultipart()");

		this->hasBody = true;
	#endif

	#if LIBCURL_VERSION_MAJOR > 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR >= 56)
		CURLcode e[6]; memset(e, CURLcode::CURLE_OK, sizeof(e));
		curl_mimepart* _part = nullptr;

		this->mime = curl_mime_init(this->curl_);
		if (!this->mime) {
			throw liboai::exception::OpenAIException(
				"curl_mime_init() failed",
				liboai::exception::EType::E_CURLERROR,
				"liboai::netimpl::Session::SetMultipart()"
			);
		}

		for (const auto& part : multipart.parts) {
			std::vector<curl_mimepart*> mimedata;
			if (!part.content_type.empty()) {
				mimedata.push_back(curl_mime_addpart(this->mime));
				e[0] = curl_mime_type(mimedata.back(), part.content_type.c_str());
			}
			if (part.is_file) {
				CURLcode fe[3]; memset(fe, CURLcode::CURLE_OK, sizeof(fe));
				for (const auto& file : part.files) {
					mimedata.push_back(curl_mime_addpart(this->mime));
					fe[0] = curl_mime_name(mimedata.back(), part.name.c_str());
					fe[1] = curl_mime_filedata(mimedata.back(), file.filepath.c_str());
					if (file.hasOverridedFilename()) {
						fe[2] = curl_mime_filename(mimedata.back(), file.overrided_filename.c_str());
					}

					// check each file
					ErrorCheck(fe, 3, "liboai::netimpl::Session::SetMultipart() @ is_file[mime]");
				}
			}
			else if (part.is_buffer) {
				mimedata.push_back(curl_mime_addpart(this->mime));
				e[1] = curl_mime_name(mimedata.back(), part.name.c_str());
				e[2] = curl_mime_filename(mimedata.back(), part.value.c_str());
				e[3] = curl_mime_data(mimedata.back(), reinterpret_cast<const char*>(part.data), part.datalen);
			}
			else {
				mimedata.push_back(curl_mime_addpart(this->mime));
				e[3] = curl_mime_name(mimedata.back(), part.name.c_str());
				e[4] = curl_mime_data(mimedata.back(), part.value.c_str(), CURL_ZERO_TERMINATED);
			}
		}
		e[5] = curl_easy_setopt(this->curl_, CURLOPT_MIMEPOST, this->mime);

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set multipart for Session (0x%p) using curl_mime_addpart() and CURLOPT_MIMEPOST.\n",
				__func__, this
			);
		#endif

		ErrorCheck(e, 6, "liboai::netimpl::Session::SetMultipart()");

		this->hasBody = true;
	#endif
}

std::string liboai::netimpl::CurlHolder::urlEncode(const std::string& s) {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] URL-encode string \"%s\".\n",
			__func__, s.c_str()
		);
	#endif
	
	char* output = curl_easy_escape(this->curl_, s.c_str(), static_cast<int>(s.length()));
	if (output) {
		std::string result = output;
		curl_free(output);
		return result;
	}
	return "";
}

std::string liboai::netimpl::CurlHolder::urlDecode(const std::string& s) {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] URL-decode string \"%s\".\n",
			__func__, s.c_str()
		);
	#endif
	
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
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called with %zu bytes.\n",
			__func__, size * nmemb
		);
	#endif
		
	size *= nmemb;
	return (*write)({ ptr, size }) ? size : 0;
}

size_t liboai::netimpl::components::writeFunction(char* ptr, size_t size, size_t nmemb, std::string* data) {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called with %zu bytes.\n",
			__func__, size * nmemb
		);
	#endif

	size *= nmemb;
	data->append(ptr, size);
	return size;
}

size_t liboai::netimpl::components::writeFileFunction(char* ptr, size_t size, size_t nmemb, std::ofstream* file) {
	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Called with %zu bytes.\n",
			__func__, size * nmemb
		);
	#endif
		
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

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Added parameter \"%s\" with value \"%s\".\n",
				__func__, parameter.key.c_str(), parameter.value.c_str()
			);
		#endif
	}
}

void liboai::netimpl::components::Parameters::Add(const Parameter& parameter) {
	this->parameters_.emplace_back(parameter.key, parameter.value);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Added parameter \"%s\" with value \"%s\".\n",
			__func__, parameter.key.c_str(), parameter.value.c_str()
		);
	#endif
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

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Built parameter string \"%s\".\n",
			__func__, parameter_string.c_str()
		);
	#endif

	return parameter_string;
}

void liboai::netimpl::Session::SetOption(const components::Header& header) {
	this->SetHeader(header);
}

void liboai::netimpl::Session::SetHeader(const components::Header& header) {
	CURLcode e;
	
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

	curl_slist* temp;
//  Causes cURL error for simple GET requests
//    curl_slist* temp = curl_slist_append(this->headers, "Transfer-Encoding: chunked");
//    if (temp) {
//		this->headers = temp;
//    }

	// remove preset curl headers for files >1MB
    temp = curl_slist_append(this->headers, "Expect:");
    if (temp) {
		this->headers = temp;
    }

    e = curl_easy_setopt(this->curl_, CURLOPT_HTTPHEADER, this->headers);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set headers.\n",
			__func__
		);
	#endif

	ErrorCheck(e, "liboai::netimpl::Session::SetHeader()");
}

void liboai::netimpl::Session::SetOption(const components::Parameters& parameters) {
	this->SetParameters(parameters);
}

void liboai::netimpl::Session::SetParameters(const components::Parameters& parameters) {
	if (!parameters.Empty()) {
		this->parameter_string_ = parameters.BuildParameterString();

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set parameters.\n",
				__func__
			);
		#endif
	}
}

void liboai::netimpl::Session::SetOption(components::Parameters&& parameters) {
	this->SetParameters(std::move(parameters));
}

void liboai::netimpl::Session::SetParameters(components::Parameters&& parameters) {
	if (!parameters.Empty()) {
		this->parameter_string_ = parameters.BuildParameterString();

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set parameters.\n",
				__func__
			);
		#endif
	}
}

void liboai::netimpl::Session::SetOption(const components::Timeout& timeout) {
	this->SetTimeout(timeout);
}

void liboai::netimpl::Session::SetTimeout(const components::Timeout& timeout) {
	CURLcode e = curl_easy_setopt(this->curl_, CURLOPT_TIMEOUT_MS, timeout.Milliseconds());

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set timeout to %ld milliseconds\n",
			__func__, timeout.Milliseconds()
		);
	#endif
		
	ErrorCheck(e, "liboai::netimpl::Session::SetTimeout()");
}

void liboai::netimpl::Session::SetOption(const components::Proxies& proxies) {
	this->SetProxies(proxies);
}

void liboai::netimpl::Session::SetProxies(const components::Proxies& proxies) {
	this->proxies_ = proxies;

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set proxies.\n",
			__func__
		);
	#endif
}

void liboai::netimpl::Session::SetOption(components::Proxies&& proxies) {
	this->SetProxies(std::move(proxies));
}

void liboai::netimpl::Session::SetProxies(components::Proxies&& proxies) {
	this->proxies_ = std::move(proxies);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set proxies.\n",
			__func__
		);
	#endif
}

void liboai::netimpl::Session::SetOption(const components::ProxyAuthentication& proxy_auth) {
	this->SetProxyAuthentication(proxy_auth);
}

void liboai::netimpl::Session::SetProxyAuthentication(const components::ProxyAuthentication& proxy_auth) {
	this->proxyAuth_ = proxy_auth;

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set proxy authentication.\n",
			__func__
		);
	#endif
}

void liboai::netimpl::Session::SetOption(components::ProxyAuthentication&& proxy_auth) {
	this->SetProxyAuthentication(std::move(proxy_auth));
}

void liboai::netimpl::Session::SetProxyAuthentication(components::ProxyAuthentication&& proxy_auth) {
	this->proxyAuth_ = std::move(proxy_auth);

	#if defined(LIBOAI_DEBUG)
		_liboai_dbg(
			"[dbg] [@%s] Set proxy authentication.\n",
			__func__
		);
	#endif
}

void liboai::netimpl::Session::SetOption(const components::WriteCallback& write) {
	this->SetWriteCallback(write);
}

void liboai::netimpl::Session::SetWriteCallback(const components::WriteCallback& write) {
	if (write.callback) {
		CURLcode e[2]; memset(e, CURLcode::CURLE_OK, sizeof(e));

		e[0] = curl_easy_setopt(this->curl_, CURLOPT_WRITEFUNCTION, components::writeUserFunction);
		this->write_ = write;
		e[1] = curl_easy_setopt(this->curl_, CURLOPT_WRITEDATA, &this->write_);

		#if defined(LIBOAI_DEBUG)
				_liboai_dbg(
					"[dbg] [@%s] Set user supplied write callback.\n",
					__func__
				);
		#endif

		ErrorCheck(e, 2, "liboai::netimpl::Session::SetWriteCallback()");
	}
}

void liboai::netimpl::Session::SetOption(components::WriteCallback&& write) {
	this->SetWriteCallback(std::move(write));
}

void liboai::netimpl::Session::SetWriteCallback(components::WriteCallback&& write) {
	if (write.callback) {
		CURLcode e[2]; memset(e, CURLcode::CURLE_OK, sizeof(e));

		e[0] = curl_easy_setopt(this->curl_, CURLOPT_WRITEFUNCTION, components::writeUserFunction);
		this->write_ = std::move(write);
		e[1] = curl_easy_setopt(this->curl_, CURLOPT_WRITEDATA, &this->write_);

		#if defined(LIBOAI_DEBUG)
			_liboai_dbg(
				"[dbg] [@%s] Set user supplied write callback.\n",
				__func__
			);
		#endif

		ErrorCheck(e, 2, "liboai::netimpl::Session::SetWriteCallback()");
	}
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

void liboai::netimpl::ErrorCheck(CURLcode* ecodes, size_t size, std::string_view where) {
	if (ecodes) {
		for (size_t i = 0; i < size; ++i) {
			if (ecodes[i] != CURLE_OK) {
				throw liboai::exception::OpenAIException(
					curl_easy_strerror(ecodes[i]),
					liboai::exception::EType::E_CURLERROR,
					where
				);
			}
		}
	}
}

void liboai::netimpl::ErrorCheck(CURLcode ecode, std::string_view where) {
	if (ecode != CURLE_OK) {
		throw liboai::exception::OpenAIException(
			curl_easy_strerror(ecode),
			liboai::exception::EType::E_CURLERROR,
			where
		);
	}
}

#if LIBCURL_VERSION_MAJOR < 7 || (LIBCURL_VERSION_MAJOR == 7 && LIBCURL_VERSION_MINOR < 56)
	void liboai::netimpl::ErrorCheck(CURLFORMcode* ecodes, size_t size, std::string_view where) {
		if (ecodes) {
			for (size_t i = 0; i < size; ++i) {
				if (ecodes[i] != CURL_FORMADD_OK) {
					throw liboai::exception::OpenAIException(
						"curl_formadd() failed.",
						liboai::exception::EType::E_CURLERROR,
						where
					);
				}
			}
		}
	}

	void liboai::netimpl::ErrorCheck(CURLFORMcode ecode, std::string_view where) {
		if (ecode != CURL_FORMADD_OK) {
			throw liboai::exception::OpenAIException(
				"curl_formadd() failed.",
				liboai::exception::EType::E_CURLERROR,
				where
			);
		}
	}
#endif
