#include "../include/components/embeddings.h"

liboai::Response liboai::Embeddings::create(const std::string& model_id, const std::optional<std::string>& input, const std::optional<std::string>& user) const& {
	nlohmann::json json_data = {
		{ "model", model_id }
	};
	if (input.has_value()) { json_data["input"] = input.value(); }
	if (user.has_value()) { json_data["user"] = user.value(); }

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/embeddings", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			json_data.dump()
		},
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}