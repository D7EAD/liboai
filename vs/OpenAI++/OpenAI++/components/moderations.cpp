#include "../include/components/moderations.h"

liboai::Response liboai::Moderations::create(const std::string& input, const std::optional<std::string>& model) const & {
	nlohmann::json json_data = {
		{ "input", input }
	};
	if (model.has_value()) { json_data["model"] = model.value(); }
	
	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/moderations", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			json_data.dump()
		},
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}