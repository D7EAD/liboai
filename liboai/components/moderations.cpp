#include "../include/components/moderations.h"

liboai::Response liboai::Moderations::create(const std::string& input, std::optional<std::string> model) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("input", input);
	jcon.push_back("model", std::move(model));
	
	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/moderations", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			jcon.dump()
		},
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Moderations::create_async(const std::string& input, std::optional<std::string> model) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("input", input);
	jcon.push_back("model", std::move(model));
	
	return std::async(
		std::launch::async, [&, jcon]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/moderations", "application/json",
				this->auth_.GetAuthorizationHeaders(),
				cpr::Body {
					jcon.dump()
				},
				this->auth_.GetProxies()
			);
		}
	);
}