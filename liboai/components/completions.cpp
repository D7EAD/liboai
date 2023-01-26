#include "../include/components/completions.h"

liboai::Response liboai::Completions::create(const std::string& model_id, std::optional<std::string> prompt, std::optional<std::string> suffix, std::optional<uint16_t> max_tokens, std::optional<float> temperature, std::optional<float> top_p, std::optional<uint16_t> n, std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<uint8_t> logprobs, std::optional<bool> echo, std::optional<std::vector<std::string>> stop, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<uint16_t> best_of, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model_id);
	jcon.push_back("prompt", std::move(prompt));
	jcon.push_back("suffix", std::move(suffix));
	jcon.push_back("max_tokens", std::move(max_tokens));
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));
	jcon.push_back("n", std::move(n));
	jcon.push_back("logprobs", std::move(logprobs));
	jcon.push_back("echo", std::move(echo));
	jcon.push_back("stop", std::move(stop));
	jcon.push_back("presence_penalty", std::move(presence_penalty));
	jcon.push_back("frequency_penalty", std::move(frequency_penalty));
	jcon.push_back("best_of", std::move(best_of));
	jcon.push_back("logit_bias", std::move(logit_bias));
	jcon.push_back("user", std::move(user));

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/completions", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			jcon.dump()
		},
		stream.has_value() ? cpr::WriteCallback{stream.value()} : cpr::WriteCallback{},
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Completions::create_async(const std::string& model_id, std::optional<std::string> prompt, std::optional<std::string> suffix, std::optional<uint16_t> max_tokens, std::optional<float> temperature, std::optional<float> top_p, std::optional<uint16_t> n, std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<uint8_t> logprobs, std::optional<bool> echo, std::optional<std::vector<std::string>> stop, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<uint16_t> best_of, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model_id);
	jcon.push_back("prompt", std::move(prompt));
	jcon.push_back("suffix", std::move(suffix));
	jcon.push_back("max_tokens", std::move(max_tokens));
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));
	jcon.push_back("n", std::move(n));
	jcon.push_back("logprobs", std::move(logprobs));
	jcon.push_back("echo", std::move(echo));
	jcon.push_back("stop", std::move(stop));
	jcon.push_back("presence_penalty", std::move(presence_penalty));
	jcon.push_back("frequency_penalty", std::move(frequency_penalty));
	jcon.push_back("best_of", std::move(best_of));
	jcon.push_back("logit_bias", std::move(logit_bias));
	jcon.push_back("user", std::move(user));

	return std::async(
		std::launch::async, [&, jcon]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/completions", "application/json",
				this->auth_.GetAuthorizationHeaders(),
				cpr::Body{
					jcon.dump()
				},
				stream.has_value() ? cpr::WriteCallback{ stream.value() } : cpr::WriteCallback{},
				this->auth_.GetProxies()
			);
		}
	);
}