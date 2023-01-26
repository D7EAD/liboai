#include "../include/components/embeddings.h"

liboai::Response liboai::Embeddings::create(const std::string& model_id, std::optional<std::string> input, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model_id);
	jcon.push_back("input", std::move(input));
	jcon.push_back("user", std::move(user));

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/embeddings", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			jcon.dump()
		},
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Embeddings::create_async(const std::string& model_id, std::optional<std::string> input, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model_id);
	jcon.push_back("input", std::move(input));
	jcon.push_back("user", std::move(user));

	return std::async(
		std::launch::async, [&, jcon]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/embeddings", "application/json",
				this->auth_.GetAuthorizationHeaders(),
				cpr::Body {
					jcon.dump()
				},
				this->auth_.GetProxies()
			);
		}
	);
}