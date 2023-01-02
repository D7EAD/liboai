#include "../include/components/completions.h"

liboai::Response liboai::Completions::create(const std::string& model_id, const std::optional<std::string>& prompt, const std::optional<std::string>& suffix, const std::optional<uint16_t>& max_tokens, const std::optional<float>& temperature, const std::optional<float>& top_p, const std::optional<uint16_t>& n, const std::optional<std::function<bool(std::string, intptr_t)>>& stream, const std::optional<uint8_t>& logprobs, const std::optional<bool>& echo, const std::optional<std::vector<std::string>>& stop, const std::optional<float>& presence_penalty, const std::optional<float>& frequency_penalty, const std::optional<uint16_t>& best_of, const std::optional<std::unordered_map<std::string, int8_t>>& logit_bias, const std::optional<std::string>& user) const& {
	nlohmann::json json_data = {
		{ "model", model_id }
	};
	if (prompt.has_value()) { json_data["prompt"] = prompt.value(); }
	if (suffix.has_value()) { json_data["suffix"] = suffix.value(); }
	if (max_tokens.has_value()) { json_data["max_tokens"] = max_tokens.value(); }
	if (temperature.has_value()) { json_data["temperature"] = temperature.value(); }
	if (top_p.has_value()) { json_data["top_p"] = top_p.value(); }
	if (n.has_value()) { json_data["n"] = n.value(); }
	if (logprobs.has_value()) { json_data["logprobs"] = logprobs.value(); }
	if (echo.has_value()) { json_data["echo"] = echo.value(); }
	if (stop.has_value()) { json_data["stop"] = stop.value(); }
	if (presence_penalty.has_value()) { json_data["presence_penalty"] = presence_penalty.value(); }
	if (frequency_penalty.has_value()) { json_data["frequency_penalty"] = frequency_penalty.value(); }
	if (best_of.has_value()) { json_data["best_of"] = best_of.value(); }
	if (logit_bias.has_value()) { json_data["logit_bias"] = logit_bias.value(); }
	if (user.has_value()) { json_data["user"] = user.value(); }

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/completions", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			json_data.dump()
		},
		stream.has_value() ? cpr::WriteCallback{stream.value()} : cpr::WriteCallback{},
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}