#include "../include/components/azure.h"

liboai::Response liboai::Azure::create_completion(const std::string& resource_name, const std::string& deployment_id, const std::string& api_version, std::optional<std::string> prompt, std::optional<std::string> suffix, std::optional<uint16_t> max_tokens, std::optional<float> temperature, std::optional<float> top_p, std::optional<uint16_t> n, std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<uint8_t> logprobs, std::optional<bool> echo, std::optional<std::vector<std::string>> stop, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<uint16_t> best_of, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("prompt", std::move(prompt));
	jcon.push_back("suffix", std::move(suffix));
	jcon.push_back("max_tokens", std::move(max_tokens));
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));
	jcon.push_back("n", std::move(n));
	jcon.push_back("stream", stream);
	jcon.push_back("logprobs", std::move(logprobs));
	jcon.push_back("echo", std::move(echo));
	jcon.push_back("stop", std::move(stop));
	jcon.push_back("presence_penalty", std::move(presence_penalty));
	jcon.push_back("frequency_penalty", std::move(frequency_penalty));
	jcon.push_back("best_of", std::move(best_of));
	jcon.push_back("logit_bias", std::move(logit_bias));
	jcon.push_back("user", std::move(user));

	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	Response res;
	res = this->Request(
		Method::HTTP_POST, ("https://" + resource_name + this->azure_root_ + "/deployments/" + deployment_id), "/completions", "application/json",
		this->auth_.GetAzureAuthorizationHeaders(),
		netimpl::components::Body {
			jcon.dump()
		},
		std::move(params),
		stream ? netimpl::components::WriteCallback{std::move(stream.value())} : netimpl::components::WriteCallback{},
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Azure::create_completion_async(const std::string& resource_name, const std::string& deployment_id, const std::string& api_version, std::optional<std::string> prompt, std::optional<std::string> suffix, std::optional<uint16_t> max_tokens, std::optional<float> temperature, std::optional<float> top_p, std::optional<uint16_t> n, std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<uint8_t> logprobs, std::optional<bool> echo, std::optional<std::vector<std::string>> stop, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<uint16_t> best_of, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("prompt", std::move(prompt));
	jcon.push_back("suffix", std::move(suffix));
	jcon.push_back("max_tokens", std::move(max_tokens));
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));
	jcon.push_back("n", std::move(n));
	jcon.push_back("stream", stream);
	jcon.push_back("logprobs", std::move(logprobs));
	jcon.push_back("echo", std::move(echo));
	jcon.push_back("stop", std::move(stop));
	jcon.push_back("presence_penalty", std::move(presence_penalty));
	jcon.push_back("frequency_penalty", std::move(frequency_penalty));
	jcon.push_back("best_of", std::move(best_of));
	jcon.push_back("logit_bias", std::move(logit_bias));
	jcon.push_back("user", std::move(user));

	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	return std::async(
		std::launch::async, [&, jcon, params, stream]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, ("https://" + resource_name + this->azure_root_ + "/deployments/" + deployment_id), "/completions", "application/json",
				this->auth_.GetAzureAuthorizationHeaders(),
				netimpl::components::Body {
					jcon.dump()
				},
				std::move(params),
				stream ? netimpl::components::WriteCallback{std::move(stream.value())} : netimpl::components::WriteCallback{},
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}

liboai::Response liboai::Azure::create_embedding(const std::string& resource_name, const std::string& deployment_id, const std::string& api_version, const std::string& input, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("input", input);
	jcon.push_back("user", std::move(user));

	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	Response res;
	res = this->Request(
		Method::HTTP_POST, ("https://" + resource_name + this->azure_root_ + "/deployments/" + deployment_id), "/embeddings", "application/json",
		this->auth_.GetAzureAuthorizationHeaders(),
		netimpl::components::Body {
			jcon.dump()
		},
		std::move(params),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Azure::create_embedding_async(const std::string& resource_name, const std::string& deployment_id, const std::string& api_version, const std::string& input, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("input", input);
	jcon.push_back("user", std::move(user));

	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	return std::async(
		std::launch::async, [&, jcon, params]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, ("https://" + resource_name + this->azure_root_ + "/deployments/" + deployment_id), "/embeddings", "application/json",
				this->auth_.GetAzureAuthorizationHeaders(),
				netimpl::components::Body {
					jcon.dump()
				},
				std::move(params),
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}

liboai::Response liboai::Azure::create_chat_completion(const std::string& resource_name, const std::string& deployment_id, const std::string& api_version, const Conversation& conversation, std::optional<float> temperature, std::optional<uint16_t> n, std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<std::vector<std::string>> stop, std::optional<uint16_t> max_tokens, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("n", std::move(n));
	jcon.push_back("stream", stream);
	jcon.push_back("stop", std::move(stop));
	jcon.push_back("max_tokens", std::move(max_tokens));
	jcon.push_back("presence_penalty", std::move(presence_penalty));
	jcon.push_back("frequency_penalty", std::move(frequency_penalty));
	jcon.push_back("logit_bias", std::move(logit_bias));
	jcon.push_back("user", std::move(user));

	if (conversation.GetJSON().contains("messages")) {
		jcon.push_back("messages", conversation.GetJSON()["messages"]);
	}

	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	Response res;
	res = this->Request(
		Method::HTTP_POST, ("https://" + resource_name + this->azure_root_ + "/deployments/" + deployment_id), "/chat/completions", "application/json",
		this->auth_.GetAzureAuthorizationHeaders(),
		netimpl::components::Body {
			jcon.dump()
		},
		std::move(params),
		stream ? netimpl::components::WriteCallback{std::move(stream.value())} : netimpl::components::WriteCallback{},
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Azure::create_chat_completion_async(const std::string& resource_name, const std::string& deployment_id, const std::string& api_version, const Conversation& conversation, std::optional<float> temperature, std::optional<uint16_t> n, std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<std::vector<std::string>> stop, std::optional<uint16_t> max_tokens, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("n", std::move(n));
	jcon.push_back("stream", stream);
	jcon.push_back("stop", std::move(stop));
	jcon.push_back("max_tokens", std::move(max_tokens));
	jcon.push_back("presence_penalty", std::move(presence_penalty));
	jcon.push_back("frequency_penalty", std::move(frequency_penalty));
	jcon.push_back("logit_bias", std::move(logit_bias));
	jcon.push_back("user", std::move(user));

	if (conversation.GetJSON().contains("messages")) {
		jcon.push_back("messages", conversation.GetJSON()["messages"]);
	}

	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	return std::async(
		std::launch::async, [&, jcon, params, stream]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, ("https://" + resource_name + this->azure_root_ + "/deployments/" + deployment_id), "/chat/completions", "application/json",
				this->auth_.GetAzureAuthorizationHeaders(),
				netimpl::components::Body {
					jcon.dump()
				},
				std::move(params),
				stream ? netimpl::components::WriteCallback{std::move(stream.value())} : netimpl::components::WriteCallback{},
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}

liboai::Response liboai::Azure::request_image_generation(const std::string& resource_name, const std::string& api_version, const std::string& prompt, std::optional<uint8_t> n, std::optional<std::string> size) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("prompt", prompt);
	jcon.push_back("n", std::move(n));
	jcon.push_back("size", std::move(size));

	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	Response res;
	res = this->Request(
		Method::HTTP_POST, ("https://" + resource_name + this->azure_root_), "/images/generations:submit", "application/json",
		this->auth_.GetAzureAuthorizationHeaders(),
		netimpl::components::Body {
			jcon.dump()
		},
		std::move(params),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Azure::request_image_generation_async(const std::string& resource_name, const std::string& api_version, const std::string& prompt, std::optional<uint8_t> n, std::optional<std::string> size) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("prompt", prompt);
	jcon.push_back("n", std::move(n));
	jcon.push_back("size", std::move(size));

	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	return std::async(
		std::launch::async, [&, jcon, params]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, ("https://" + resource_name + this->azure_root_), "/images/generations:submit", "application/json",
				this->auth_.GetAzureAuthorizationHeaders(),
				netimpl::components::Body {
					jcon.dump()
				},
				std::move(params),
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}

liboai::Response liboai::Azure::get_generated_image(const std::string& resource_name, const std::string& api_version, const std::string& operation_id) const & noexcept(false) {
	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	Response res;
	res = this->Request(
		Method::HTTP_GET, ("https://" + resource_name + this->azure_root_), "/operations/images/" + operation_id, "application/json",
		this->auth_.GetAzureAuthorizationHeaders(),
		std::move(params),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Azure::get_generated_image_async(const std::string& resource_name, const std::string& api_version, const std::string& operation_id) const & noexcept(false) {
	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	return std::async(
		std::launch::async, [&, params]() -> liboai::Response {
			return this->Request(
				Method::HTTP_GET, ("https://" + resource_name + this->azure_root_), "/operations/images/" + operation_id, "application/json",
				this->auth_.GetAzureAuthorizationHeaders(),
				std::move(params),
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}

liboai::Response liboai::Azure::delete_generated_image(const std::string& resource_name, const std::string& api_version, const std::string& operation_id) const & noexcept(false) {
	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	Response res;
	res = this->Request(
		Method::HTTP_DELETE, ("https://" + resource_name + this->azure_root_), "/operations/images/" + operation_id, "application/json",
		this->auth_.GetAzureAuthorizationHeaders(),
		std::move(params),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Azure::delete_generated_image_async(const std::string& resource_name, const std::string& api_version, const std::string& operation_id) const & noexcept(false) {
	netimpl::components::Parameters params;
	params.Add({ "api-version", api_version });

	return std::async(
		std::launch::async, [&, params]() -> liboai::Response {
			return this->Request(
				Method::HTTP_DELETE, ("https://" + resource_name + this->azure_root_), "/operations/images/" + operation_id, "application/json",
				this->auth_.GetAzureAuthorizationHeaders(),
				std::move(params),
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}