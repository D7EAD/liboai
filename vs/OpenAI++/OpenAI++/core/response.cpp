#include "../include/core/response.h"

void liboai::Response::CheckResponse() const noexcept(false) {
	if (this->status_code == 429) {
		throw liboai::exception::OpenAIRateLimited(
			this->reason,
			liboai::exception::EType::E_RATELIMIT,
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
				this->reason,
				liboai::exception::EType::E_BADRESPONSE,
				"liboai::Response::CheckResponse()"
			);
		}
	}
}

std::ostream& liboai::operator<<(std::ostream& os, const Response& r) {
	!r.raw_json.empty() ? os << r.raw_json.dump(4) : os << "null";
	return os;
}