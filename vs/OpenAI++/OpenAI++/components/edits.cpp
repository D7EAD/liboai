#include "../include/components/edits.h"

liboai::Response liboai::Edits::create(const std::string& model_id, const std::optional<std::string>& input, const std::optional<std::string>& instruction, const std::optional<uint16_t>& n, const std::optional<float>& temperature, const std::optional<float>& top_p) const& {
	nlohmann::json json_data = {
		{ "model", model_id }
	};
	if (input.has_value()) { json_data["input"] = input.value(); }
	if (instruction.has_value()) { json_data["instruction"] = instruction.value(); }
	if (n.has_value()) { json_data["n"] = n.value(); }
	if (temperature.has_value()) { json_data["temperature"] = temperature.value(); }
	if (top_p.has_value()) { json_data["top_p"] = top_p.value(); }

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/edits", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			json_data.dump()
		},
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}