#include "../include/core/response.h"

liboai::Response::Response(const liboai::Response& other) noexcept
	: status_code(other.status_code), elapsed(other.elapsed), status_line(other.status_line),
	content(other.content), url(other.url), reason(other.reason), raw_json(other.raw_json) {}

liboai::Response::Response(liboai::Response&& other) noexcept
	: status_code(other.status_code), elapsed(other.elapsed), status_line(std::move(other.status_line)),
	content(std::move(other.content)), url(std::move(other.url)), reason(std::move(other.reason)), raw_json(std::move(other.raw_json)) {}

liboai::Response::Response(std::string&& url, std::string&& content, std::string&& status_line, std::string&& reason, long status_code, double elapsed) noexcept(false) 
	: status_code(status_code), elapsed(elapsed), status_line(std::move(status_line)),
	content(std::move(content)), url(url), reason(std::move(reason))
{
	try {
		if (!this->content.empty()) {
			if (this->content[0] == '{') {
				this->raw_json = nlohmann::json::parse(this->content);
			}
			else {
				this->raw_json = nlohmann::json();
			}
		}
		else {
			this->raw_json = nlohmann::json();
		}
	}
	catch (nlohmann::json::parse_error& e) {
		throw liboai::exception::OpenAIException(
			e.what(),
			liboai::exception::EType::E_FAILURETOPARSE,
			"liboai::Response::Response(std::string&&, std::string&&, ...)"
		);
	}

	// check the response for errors -- nothrow on success
	this->CheckResponse();
}

liboai::Response& liboai::Response::operator=(const liboai::Response& other) noexcept {
	this->status_code = other.status_code;
	this->elapsed = other.elapsed;
	this->status_line = other.status_line;
	this->content = other.content;
	this->url = other.url;
	this->reason = other.reason;
	this->raw_json = other.raw_json;

	return *this;
}

liboai::Response& liboai::Response::operator=(liboai::Response&& other) noexcept {
	this->status_code = other.status_code;
	this->elapsed = other.elapsed;
	this->status_line = std::move(other.status_line);
	this->content = std::move(other.content);
	this->url = std::move(other.url);
	this->reason = std::move(other.reason);
	this->raw_json = std::move(other.raw_json);

	return *this;
}

std::ostream& liboai::operator<<(std::ostream& os, const Response& r) {
	!r.raw_json.empty() ? os << r.raw_json.dump(4) : os << "null";
	return os;
}

void liboai::Response::CheckResponse() const noexcept(false) {
	if (this->status_code == 429) {
		throw liboai::exception::OpenAIRateLimited(
			!this->reason.empty() ? this->reason : "Rate limited",
			liboai::exception::EType::E_RATELIMIT,
			"liboai::Response::CheckResponse()"
		);
	}
	else if (this->status_code == 0) {
		throw liboai::exception::OpenAIException(
			"A connection error occurred",
			liboai::exception::EType::E_CONNECTIONERROR,
			"liboai::Response::CheckResponse()"
		);
	}
	else if (this->status_code < 200 || this->status_code >= 300) {
		if (this->raw_json.contains("error")) {
			try {
				throw liboai::exception::OpenAIException(
					this->raw_json["error"]["message"].get<std::string>(),
					liboai::exception::EType::E_APIERROR,
					"liboai::Response::CheckResponse()"
				);
			}
			catch (nlohmann::json::parse_error& e) {
				throw liboai::exception::OpenAIException(
					e.what(),
					liboai::exception::EType::E_FAILURETOPARSE,
					"liboai::Response::CheckResponse()"
				);
			}
		}
		else {
			throw liboai::exception::OpenAIException(
				!this->reason.empty() ? this->reason : "An unknown error occurred",
				liboai::exception::EType::E_BADREQUEST,
				"liboai::Response::CheckResponse()"
			);
		}
	}
}
