#include "../include/components/edits.h"

liboai::Response liboai::Edits::create(const std::string& model_id, std::optional<std::string> input, std::optional<std::string> instruction, std::optional<uint16_t> n, std::optional<float> temperature, std::optional<float> top_p) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model_id);
	jcon.push_back("input", std::move(input));
	jcon.push_back("instruction", std::move(instruction));
	jcon.push_back("n", std::move(n));
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));

	Response res;
	res = this->Request(
		Method::HTTP_POST, this->openai_root_, "/edits", "application/json",
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

liboai::FutureResponse liboai::Edits::create_async(const std::string& model_id, std::optional<std::string> input, std::optional<std::string> instruction, std::optional<uint16_t> n, std::optional<float> temperature, std::optional<float> top_p) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model_id);
	jcon.push_back("input", std::move(input));
	jcon.push_back("instruction", std::move(instruction));
	jcon.push_back("n", std::move(n));
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));
	
	auto _fn = [this](liboai::JsonConstructor&& jcon) -> liboai::Response {
		return this->Request(
			Method::HTTP_POST, this->openai_root_, "/edits", "application/json",
			this->auth_.GetAuthorizationHeaders(),
			netimpl::components::Body {
				jcon.dump()
			},
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};

	return std::async(std::launch::async, _fn, std::move(jcon));
}