#include "../include/components/moderations.h"

liboai::Response liboai::Moderations::create(const std::string& input, std::optional<std::string> model) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("input", input);
	jcon.push_back("model", std::move(model));
	
	Response res;
	res = this->Request(
		Method::HTTP_POST, "/moderations", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		netimpl::components::Body {
			jcon.dump()
		},
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
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
				netimpl::components::Body {
					jcon.dump()
				},
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}