#include "../include/components/chat.h"

liboai::ChatCompletion::Conversation::Conversation() {
	this->_conversation["messages"] = nlohmann::json::array();
}

liboai::ChatCompletion::Conversation::Conversation(const Conversation& other) {
	this->_conversation = other._conversation;
}

liboai::ChatCompletion::Conversation::Conversation(Conversation&& old) noexcept {
	this->_conversation = std::move(old._conversation);
	old._conversation = nlohmann::json::object();
}

liboai::ChatCompletion::Conversation::Conversation(std::string_view system_data) {
	this->_conversation["messages"] = nlohmann::json::array();
	this->SetSystemData(system_data);
}

liboai::ChatCompletion::Conversation::Conversation(std::string_view system_data, std::string_view user_data) {
	this->_conversation["messages"] = nlohmann::json::array();
	this->SetSystemData(system_data);
	this->AddUserData(user_data);
}

liboai::ChatCompletion::Conversation::Conversation(std::string_view system_data, std::initializer_list<std::string_view> user_data) {
	this->_conversation["messages"] = nlohmann::json::array();
	this->SetSystemData(system_data);
	
	for (auto& data : user_data) {
		this->AddUserData(data);
	}
}

liboai::ChatCompletion::Conversation::Conversation(std::initializer_list<std::string_view> user_data) {
	this->_conversation["messages"] = nlohmann::json::array();

	for (auto& data : user_data) {
		this->AddUserData(data);
	}
}

liboai::ChatCompletion::Conversation& liboai::ChatCompletion::Conversation::operator=(const Conversation& other) {
	this->_conversation = other._conversation;
	return *this;
}

liboai::ChatCompletion::Conversation& liboai::ChatCompletion::Conversation::operator=(Conversation&& old) noexcept {
	this->_conversation = std::move(old._conversation);
	old._conversation = nlohmann::json::object();
	return *this;
}

bool liboai::ChatCompletion::Conversation::SetSystemData(std::string_view data) & noexcept(false) {
	// if data provided is non-empty
	if (!data.empty()) {
		// if system is not set already - only one system message shall exist in any
		// conversation
		for (auto& message : this->_conversation["messages"].items()) {
			if (message.value()["role"].get<std::string>() == "system") {
				return false; // system already set
			}
		}
		this->_conversation["messages"].push_back({ { "role", "system" }, {"content", data} });
		return true; // system set successfully
	}
	return false; // data is empty
}

bool liboai::ChatCompletion::Conversation::PopSystemData() & noexcept(false) {
	// if conversation is non-empty
	if (!this->_conversation["messages"].empty()) {
		// if first message is system
		if (this->_conversation["messages"][0]["role"].get<std::string>() == "system") {
			this->_conversation["messages"].erase(0);
			return true; // system message popped successfully
		}
		return false; // first message is not system
	}
	return false; // conversation is empty
}

bool liboai::ChatCompletion::Conversation::AddUserData(std::string_view data) & noexcept(false) {
	// if data provided is non-empty
	if (!data.empty()) {
		this->_conversation["messages"].push_back({ { "role", "user" }, {"content", data} });
		return true; // user data added successfully
	}
	return false; // data is empty
}

bool liboai::ChatCompletion::Conversation::PopUserData() & noexcept(false) {
	// if conversation is not empty
	if (!this->_conversation["messages"].empty()) {
		// if last message is user message
		if (this->_conversation["messages"].back()["role"].get<std::string>() == "user") {
			this->_conversation["messages"].erase(this->_conversation["messages"].end() - 1);
			return true; // user data popped successfully
		}
		return false; // last message is not user message
	}
	return false; // conversation is empty
}

std::string liboai::ChatCompletion::Conversation::GetLastResponse() const & noexcept {
	// if conversation is not empty
	if (!this->_conversation["messages"].empty()) {
		// if last message is from system
		if (this->_conversation["messages"].back()["role"].get<std::string>() == "assistant") {
			return this->_conversation["messages"].back()["content"].get<std::string>();
		}
	}
	return ""; // no response found
}

bool liboai::ChatCompletion::Conversation::PopLastResponse() & noexcept(false) {
	// if conversation is not empty
	if (!this->_conversation["messages"].empty()) {
		// if last message is assistant message
		if (this->_conversation["messages"].back()["role"].get<std::string>() == "assistant") {
			this->_conversation["messages"].erase(this->_conversation["messages"].end() - 1);
			return true; // assistant data popped successfully
		}
		return false; // last message is not assistant message
	}
	return false; // conversation is empty
}

bool liboai::ChatCompletion::Conversation::Update(std::string_view response) & noexcept(false) {
	// if response is non-empty
	if (!response.empty()) {
		nlohmann::json j = nlohmann::json::parse(response);
		if (j.contains("choices")) { // top level, several messages
			for (auto& choice : j["choices"].items()) {
				if (choice.value().contains("message")) {
					if (choice.value()["message"].contains("role") && choice.value()["message"].contains("content")) {
						this->_conversation["messages"].push_back(
							{
								{ "role",    choice.value()["message"]["role"]    },
								{ "content", choice.value()["message"]["content"] }
							}
						);
					}
					else {
						return false; // response is not valid
					}
				}
				else {
					return false; // no response found
				}
			}
		}
		else if (j.contains("message")) { // mid level, single message
			if (j["message"].contains("role") && j["message"].contains("content")) {
				this->_conversation["messages"].push_back(
					{
						{ "role",    j["message"]["role"]    },
						{ "content", j["message"]["content"] }
					}
				);
			}
			else {
				return false; // response is not valid
			}
		}
		else if (j.contains("role") && j.contains("content")) { // low level, single message
			this->_conversation["messages"].push_back(
				{
					{ "role",    j["role"]    },
					{ "content", j["content"] }
				}
			);
		}
		else {
			return false; // invalid response
		}
		return true; // response updated successfully
	}
	return false; // response is empty
}

bool liboai::ChatCompletion::Conversation::Update(const Response& response) & noexcept(false) {
	return this->Update(response.content);
}

std::string liboai::ChatCompletion::Conversation::GetRawConversation() const & noexcept {
	return this->_conversation.dump(4);
}

const nlohmann::json& liboai::ChatCompletion::Conversation::GetJSON() const & noexcept {
	return this->_conversation;
}

liboai::Response liboai::ChatCompletion::create(const std::string& model, const Conversation& conversation, std::optional<float> temperature, std::optional<float> top_p, std::optional<uint16_t> n, std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<std::vector<std::string>> stop, std::optional<uint16_t> max_tokens, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const& noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model);
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));
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

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/chat/completions", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			jcon.dump()
		},
		stream ? cpr::WriteCallback{std::move(stream.value())} : cpr::WriteCallback{},
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::ChatCompletion::create_async(const std::string& model, const Conversation& conversation, std::optional<float> temperature, std::optional<float> top_p, std::optional<uint16_t> n, std::optional<std::function<bool(std::string, intptr_t)>> stream, std::optional<std::vector<std::string>> stop, std::optional<uint16_t> max_tokens, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const& noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model);
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));
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

	return std::async(
		std::launch::async, [&, jcon]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/chat/completions", "application/json",
				this->auth_.GetAuthorizationHeaders(),
				cpr::Body {
					jcon.dump()
				},
				stream ? cpr::WriteCallback{ std::move(stream.value()) } : cpr::WriteCallback{},
				this->auth_.GetProxies()
			);
		}
	);
}

std::ostream& liboai::operator<<(std::ostream& os, const Conversation& conv) {
	os << conv.GetRawConversation();
	return os;
}