#include "../include/components/chat.h"

liboai::Conversation::Conversation() {
	this->_conversation["messages"] = nlohmann::json::array();
}

liboai::Conversation::Conversation(const Conversation& other) {
	this->_conversation = other._conversation;
	this->_functions = other._functions;
	this->_last_resp_is_fc = other._last_resp_is_fc;
}

liboai::Conversation::Conversation(Conversation&& old) noexcept {
	this->_conversation = std::move(old._conversation);
	this->_functions = std::move(old._functions);
	this->_last_resp_is_fc = old._last_resp_is_fc;

	old._conversation = nlohmann::json::object();
	old._functions = nlohmann::json::object();
}

liboai::Conversation::Conversation(std::string_view system_data) {
	this->_conversation["messages"] = nlohmann::json::array();
	auto result = this->SetSystemData(system_data);
}

liboai::Conversation::Conversation(std::string_view system_data, std::string_view user_data) {
	this->_conversation["messages"] = nlohmann::json::array();
	auto result = this->SetSystemData(system_data);
	result = this->AddUserData(user_data);
}

liboai::Conversation::Conversation(std::string_view system_data, std::initializer_list<std::string_view> user_data) {
	this->_conversation["messages"] = nlohmann::json::array();
	auto result = this->SetSystemData(system_data);
	
	for (auto& data : user_data) {
		auto result = this->AddUserData(data);
	}
}

liboai::Conversation::Conversation(std::initializer_list<std::string_view> user_data) {
	this->_conversation["messages"] = nlohmann::json::array();

	for (auto& data : user_data) {
		auto result = this->AddUserData(data);
	}
}

liboai::Conversation::Conversation(const std::vector<std::string>& user_data) {
	this->_conversation["messages"] = nlohmann::json::array();
	
	for (auto& data : user_data) {
		auto result = this->AddUserData(data);
	}
}

liboai::Conversation& liboai::Conversation::operator=(const Conversation& other) {
	this->_conversation = other._conversation;
	this->_functions = other._functions;
	this->_last_resp_is_fc = other._last_resp_is_fc;
	return *this;
}

liboai::Conversation& liboai::Conversation::operator=(Conversation&& old) noexcept {
	this->_conversation = std::move(old._conversation);
	this->_functions = std::move(old._functions);
	this->_last_resp_is_fc = old._last_resp_is_fc;

	old._conversation = nlohmann::json::object();
	old._functions = nlohmann::json::object();

	return *this;
}

bool liboai::Conversation::ChangeFirstSystemMessage(std::string_view new_data) & noexcept(false) {
	if (!new_data.empty() && !this->_conversation["messages"].empty()) {
		if (this->_conversation["messages"][0]["role"].get<std::string>() == "system") {
			this->_conversation["messages"][0]["content"] = new_data;
			return true; // System message changed successfuly
		}
		return false; // First message is not a system message
	}
	return false; // New data is empty or conversation is empty
}

bool liboai::Conversation::SetSystemData(std::string_view data) & noexcept(false) {
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

bool liboai::Conversation::PopSystemData() & noexcept(false) {
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

bool liboai::Conversation::AddUserData(std::string_view data) & noexcept(false) {
	// if data provided is non-empty
	if (!data.empty()) {
		this->_conversation["messages"].push_back({ { "role", "user" }, {"content", data} });
		return true; // user data added successfully
	}
	return false; // data is empty
}

bool liboai::Conversation::AddUserData(std::string_view data, std::string_view name) & noexcept(false) {
	// if data provided is non-empty
	if (!data.empty()) {
		this->_conversation["messages"].push_back(
			{
				{"role", "user"},
				{"content", data},
				{"name", name}
			}
		);
		return true; // user data added successfully
	}
	return false; // data is empty
}

bool liboai::Conversation::PopUserData() & noexcept(false) {
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

std::string liboai::Conversation::GetLastResponse() const & noexcept {
	// if conversation is not empty
	if (!this->_conversation["messages"].empty()) {
		// if last message is from assistant
		if (this->_conversation["messages"].back()["role"].get<std::string>() == "assistant") {
			return this->_conversation["messages"].back()["content"].get<std::string>();
		}
	}
	return ""; // no response found
}

bool liboai::Conversation::LastResponseIsFunctionCall() const & noexcept {
	return this->_last_resp_is_fc;
}

std::string liboai::Conversation::GetLastFunctionCallName() const & noexcept(false) {
	if (this->_conversation.contains("function_call")) {
		if (this->_conversation["function_call"].contains("name")) {
			return this->_conversation["function_call"]["name"].get<std::string>();
		}
	}

	return "";
}

std::string liboai::Conversation::GetLastFunctionCallArguments() const & noexcept(false) {
	if (this->_conversation.contains("function_call")) {
		if (this->_conversation["function_call"].contains("arguments")) {
			return this->_conversation["function_call"]["arguments"].get<std::string>();
		}
	}
	
	return "";
}

bool liboai::Conversation::PopLastResponse() & noexcept(false) {
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

bool liboai::Conversation::Update(std::string_view response) & noexcept(false) {
	// reset "last response is function call" flag
	if (this->_last_resp_is_fc) {
		if (this->_conversation.contains("function_call")) {
			this->_conversation.erase("function_call");
		}
		this->_last_resp_is_fc = false;
	}
	
	// if response is non-empty
	if (!response.empty()) {
		nlohmann::json j = nlohmann::json::parse(response);
		if (j.contains("choices")) { // top level, several messages
			for (auto& choice : j["choices"].items()) {
				if (choice.value().contains("message")) {
					if (choice.value()["message"].contains("role") && choice.value()["message"].contains("content")) {
						if (!choice.value()["message"]["content"].is_null()) {
							this->_conversation["messages"].push_back(
								{
									{ "role",    choice.value()["message"]["role"]    },
									{ "content", choice.value()["message"]["content"] }
								}
							);
						}
						else {
							this->_conversation["messages"].push_back(
								{
									{ "role",    choice.value()["message"]["role"]    },
									{ "content", "" }
								}
							);
						}
						
						if (choice.value()["message"].contains("function_call")) {
							// if a function_call is present in the response, the
							// conversation is not updated as there is no assistant
							// response to be added. However, we do add the function
							// information
							
							this->_conversation["function_call"] = nlohmann::json::object();
							if (choice.value()["message"]["function_call"].contains("name")) {
								this->_conversation["function_call"]["name"] = choice.value()["message"]["function_call"]["name"];
							}
							if (choice.value()["message"]["function_call"].contains("arguments")) {
								this->_conversation["function_call"]["arguments"] = choice.value()["message"]["function_call"]["arguments"];
							}
							
							this->_last_resp_is_fc = true;
						}

						return true; // conversation updated successfully
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
				if (j["message"]["content"].is_null()) {
					this->_conversation["messages"].push_back(
						{
							{ "role",    j["message"]["role"]    },
							{ "content", j["message"]["content"] }
						}
					);
				}
				else {
					this->_conversation["messages"].push_back(
						{
							{ "role",    j["message"]["role"] },
							{ "content", "" }
						}
					);
				}

				if (j["message"].contains("function_call")) {
					// if a function_call is present in the response, the
					// conversation is not updated as there is no assistant
					// response to be added. However, we do add the function
					// information
					
					this->_conversation["function_call"] = nlohmann::json::object();
					if (j["message"]["function_call"].contains("name")) {
						this->_conversation["function_call"]["name"] = j["message"]["function_call"]["name"];
					}
					if (j["message"]["function_call"].contains("arguments")) {
						this->_conversation["function_call"]["arguments"] = j["message"]["function_call"]["arguments"];
					}

					this->_last_resp_is_fc = true;
				}

				return true; // conversation updated successfully
			}
			else {
				return false; // response is not valid
			}
		}
		else if (j.contains("role") && j.contains("content")) { // low level, single message
			if (j["message"]["content"].is_null()) {
				this->_conversation["messages"].push_back(
					{
						{ "role",    j["message"]["role"]    },
						{ "content", j["message"]["content"] }
					}
				);
			}
			else {
				this->_conversation["messages"].push_back(
					{
						{ "role",    j["message"]["role"] },
						{ "content", "" }
					}
				);
			}
			
			if (j["message"].contains("function_call")) {
				// if a function_call is present in the response, the
				// conversation is not updated as there is no assistant
				// response to be added. However, we do add the function
				// information
				this->_conversation["function_call"] = nlohmann::json::object();
				if (j["message"]["function_call"].contains("name")) {
					this->_conversation["function_call"]["name"] = j["message"]["function_call"]["name"];
				}
				if (j["message"]["function_call"].contains("arguments")) {
					this->_conversation["function_call"]["arguments"] = j["message"]["function_call"]["arguments"];
				}

				this->_last_resp_is_fc = true;
			}

			return true; // conversation updated successfully
		}
		else {
			return false; // invalid response
		}
	}
	return false; // response is empty
}

bool liboai::Conversation::Update(const Response& response) & noexcept(false) {
	return this->Update(response.content);
}

std::string liboai::Conversation::Export() const & noexcept(false) {
	nlohmann::json j;
	
	if (!this->_conversation.empty()) {
		j["messages"] = this->_conversation["messages"];

		if (this->_functions) {
			j["functions"] = this->_functions.value()["functions"];
		}

		return j.dump(4); // conversation exported successfully
	}

	return ""; // conversation is empty
}

bool liboai::Conversation::Import(std::string_view json) & noexcept(false) {
	if (!json.empty()) {
		nlohmann::json j = nlohmann::json::parse(json);

		if (j.contains("messages")) {
			this->_conversation["messages"] = j["messages"];

			if (j.contains("functions")) {
				this->_functions = nlohmann::json();
				this->_functions.value()["functions"] = j["functions"];
			}

			return true; // conversation imported successfully
		}
		
		return false; // no messages found
	}
	
	return false; // json is empty
}

bool liboai::Conversation::AppendStreamData(std::string data) & noexcept(false) {
	if (!data.empty()) {
		std::string delta;
		bool completed = false;
		return this->ParseStreamData(data, delta, completed);
	}
	
	return false; // data is empty
}

bool liboai::Conversation::AppendStreamData(std::string data, std::string& delta, bool& completed) & noexcept(false){
	if (!data.empty()) {
		return this->ParseStreamData(data, delta, completed);
	}
	
	return false;
}


bool liboai::Conversation::SetFunctions(Functions functions) & noexcept(false) {
	nlohmann::json j = functions.GetJSON();
	
	if (!j.empty() && j.contains("functions") && j["functions"].size() > 0) {
		this->_functions = std::move(j);
		return true; // functions set successfully
	}
	
	return false; // functions are empty
}

void liboai::Conversation::PopFunctions() & noexcept(false) {
	this->_functions = std::nullopt;
}

std::string liboai::Conversation::GetRawConversation() const & noexcept {
	return this->_conversation.dump(4);
}

const nlohmann::json& liboai::Conversation::GetJSON() const & noexcept {
	return this->_conversation;
}

std::string liboai::Conversation::GetRawFunctions() const & noexcept {
	return this->HasFunctions() ? this->_functions.value().dump(4) : "";
}

const nlohmann::json& liboai::Conversation::GetFunctionsJSON() const & noexcept {
	return this->_functions.value();
}

std::vector<std::string> liboai::Conversation::SplitStreamedData(std::string data) const noexcept(false) {
	// remove all instances of the string "data: " from the string
	this->RemoveStrings(data, "data: ");

	/*
		Splits the streamed data into a vector of strings
		via delimiter of two newlines.

		For instance, a string of "Hello\n\nWorld" would
		be split into a vector of {"Hello", "World"}, and
		a string of "Hello World" would be split into
		a vector of {"Hello World"}.
	*/
	if (!data.empty()) {
		std::vector<std::string> split_data;
		std::string temp;
		std::istringstream iss(data);
		while (std::getline(iss, temp)) {
			if (temp.empty()) {
				split_data.push_back(temp);
			}
			else {
				split_data.push_back(temp);
			}
		}

		// remove empty strings from the vector
		split_data.erase(std::remove_if(split_data.begin(), split_data.end(), [](const std::string& s) { return s.empty(); }), split_data.end());

		return split_data;
	}

	return {};
}

void liboai::Conversation::RemoveStrings(std::string& s, std::string_view p) const noexcept(false) {
	std::string::size_type i = s.find(p);
	while (i != std::string::npos) {
		s.erase(i, p.length());
		i = s.find(p, i);
	}
}

std::vector<std::string> liboai::Conversation::SplitFullStreamedData(std::string data) const noexcept(false) {
	if (data.empty()) {
		return {};
	}
	
	std::vector<std::string> split_data;
	std::string temp;
	std::istringstream iss(data);
	while (std::getline(iss, temp)) {
		if (temp.empty()) {
			split_data.push_back(temp);
		}
		else {
			split_data.push_back(temp);
		}
	}

	// remove empty strings from the vector
	split_data.erase(std::remove_if(split_data.begin(), split_data.end(), [](const std::string& s) { return s.empty(); }), split_data.end());

	return split_data;
}

bool liboai::Conversation::ParseStreamData(std::string data, std::string& delta_content, bool& completed){
	if (!_last_incomplete_buffer.empty()) {
		data = _last_incomplete_buffer + data;
		_last_incomplete_buffer.clear();
	}

	std::vector<std::string> data_lines = SplitFullStreamedData(data);

	if (data_lines.empty()){
		return false;
	}
	
	// create an empty message at the end of the conversation,
	// marked as "pending" to indicate that the response is
	// still being processed. This flag will be removed once
	// the response is processed. If the marking already
	// exists, keep appending to the same message.
	if (this->_conversation["messages"].empty() || !this->_conversation["messages"].back().contains("pending")) {
		this->_conversation["messages"].push_back(
			{
				{ "role",    "" },
				{ "content", "" },
				{ "pending", true }
			}
		);
	}

	for (auto& line : data_lines){
		if (line.find("data: [DONE]") == std::string::npos) {
			/*
				j should have content in the form of:
					{"id":"chatcmpl-7SKOck29emvbBbDS6cHg5xwnRrsLO","object":"chat.completion.chunk","created":1686985942,"model":"gpt-3.5-turbo-0613","choices":[{"index":0,"delta":{"content":"."},"finish_reason":null}]}
				where "delta" may be empty
			*/
			this->RemoveStrings(line, "data: ");

			nlohmann::json j;
			try {
				j = nlohmann::json::parse(line);
			} catch (const std::exception& e) {
				_last_incomplete_buffer = line;
				continue;
			}

			if (j.contains("choices")) {
				if (j["choices"][0].contains("delta")) {
					if (!j["choices"][0]["delta"].empty() && !j["choices"][0]["delta"].is_null()) {
						if (j["choices"][0]["delta"].contains("role")) {
							this->_conversation["messages"].back()["role"] = j["choices"][0]["delta"]["role"];
						}

						if (j["choices"][0]["delta"].contains("content")) {
							if (!j["choices"][0]["delta"]["content"].empty() && !j["choices"][0]["delta"]["content"].is_null()) {
								std::string stream_content =  j["choices"][0]["delta"]["content"].get<std::string>();
								this->_conversation["messages"].back()["content"] = this->_conversation["messages"].back()["content"].get<std::string>() + stream_content;
								delta_content += stream_content;
							}

							// function calls do not have a content field,
							// set _last_resp_is_fc to false and remove any
							// previously set function_call field in the
							// conversation
							if (this->_last_resp_is_fc) {
								if (this->_conversation.contains("function_call")) {
									this->_conversation.erase("function_call");
								}
								this->_last_resp_is_fc = false;
							}
						}

						if (j["choices"][0]["delta"].contains("function_call")) {
							if (!j["choices"][0]["delta"]["function_call"].empty() && !j["choices"][0]["delta"]["function_call"].is_null()) {
								if (j["choices"][0]["delta"]["function_call"].contains("name")) {
									if (!j["choices"][0]["delta"]["function_call"]["name"].empty() && !j["choices"][0]["delta"]["function_call"]["name"].is_null()) {
										if (!this->_conversation["messages"].back().contains("function_call")) {
											this->_conversation["function_call"] = { { "name", j["choices"][0]["delta"]["function_call"]["name"] } };
											this->_last_resp_is_fc = true;
										}
									}
								}
								else if (j["choices"][0]["delta"]["function_call"].contains("arguments")) {
									if (!j["choices"][0]["delta"]["function_call"]["arguments"].empty() && !j["choices"][0]["delta"]["function_call"]["arguments"].is_null()) {
										if (!this->_conversation["function_call"].contains("arguments")) {
											this->_conversation["function_call"].push_back({ "arguments", j["choices"][0]["delta"]["function_call"]["arguments"] });
										}
										else {
											this->_conversation["function_call"]["arguments"] = this->_conversation["function_call"]["arguments"].get<std::string>() + j["choices"][0]["delta"]["function_call"]["arguments"].get<std::string>();
										}
									}
								}
							}
						}
					}
				}
			} else {
				return false; // no "choices" found - invalid
			}
		} else {
			// the response is complete, erase the "pending" flag
			this->_conversation["messages"].back().erase("pending");
			completed = true;
		}
	}

	return true; // last message received
}



liboai::Response liboai::ChatCompletion::create(const std::string& model, Conversation& conversation, std::optional<std::string> function_call, std::optional<float> temperature, std::optional<float> top_p, std::optional<uint16_t> n, std::optional<ChatStreamCallback> stream, std::optional<std::vector<std::string>> stop, std::optional<uint16_t> max_tokens, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const& noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("model", model);
	jcon.push_back("temperature", std::move(temperature));
	jcon.push_back("top_p", std::move(top_p));
	jcon.push_back("n", std::move(n));
	jcon.push_back("stop", std::move(stop));
	jcon.push_back("max_tokens", std::move(max_tokens));
	jcon.push_back("presence_penalty", std::move(presence_penalty));
	jcon.push_back("frequency_penalty", std::move(frequency_penalty));
	jcon.push_back("logit_bias", std::move(logit_bias));
	jcon.push_back("user", std::move(user));

	if (function_call) {
		if (function_call.value() == "none" || function_call.value() == "auto") {
			nlohmann::json j; j["function_call"] = function_call.value();
			jcon.push_back("function_call", j["function_call"]);
		}
		else {
			nlohmann::json j; j["function_call"] = { {"name", function_call.value()} };
			jcon.push_back("function_call", j["function_call"]);
		}
	}

	StrippedStreamCallback _sscb = nullptr;
	if (stream) {
		_sscb = [stream, &conversation](std::string data, intptr_t userdata) -> bool {
			ChatStreamCallback _stream = stream.value();
			return _stream(data, userdata, conversation);
		};

		jcon.push_back("stream", _sscb);
	}

	if (conversation.GetJSON().contains("messages")) {
		jcon.push_back("messages", conversation.GetJSON()["messages"]);
	}
	
	if (conversation.HasFunctions()) {
		jcon.push_back("functions", conversation.GetFunctionsJSON()["functions"]);
	}

	Response res;
	res = this->Request(
		Method::HTTP_POST, this->openai_root_, "/chat/completions", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		netimpl::components::Body {
			jcon.dump()
		},
		_sscb ? netimpl::components::WriteCallback{std::move(_sscb)} : netimpl::components::WriteCallback{},
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::ChatCompletion::create_async(const std::string& model, Conversation& conversation, std::optional<std::string> function_call, std::optional<float> temperature, std::optional<float> top_p, std::optional<uint16_t> n, std::optional<ChatStreamCallback> stream, std::optional<std::vector<std::string>> stop, std::optional<uint16_t> max_tokens, std::optional<float> presence_penalty, std::optional<float> frequency_penalty, std::optional<std::unordered_map<std::string, int8_t>> logit_bias, std::optional<std::string> user) const& noexcept(false) {
	return std::async(std::launch::async, &liboai::ChatCompletion::create, this, model, std::ref(conversation), function_call, temperature, top_p, n, stream, stop, max_tokens, presence_penalty, frequency_penalty, logit_bias, user);
}

namespace liboai {

std::ostream& operator<<(std::ostream& os, const Conversation& conv) {
	os << conv.GetRawConversation() << std::endl << (conv.HasFunctions() ? conv.GetRawFunctions() : "");

	return os;
}

}

liboai::Functions::Functions() {
	this->_functions["functions"] = nlohmann::json::array();
}

liboai::Functions::Functions(const Functions& other) {
	this->_functions = other._functions;
}

liboai::Functions::Functions(Functions&& old) noexcept {
	this->_functions = std::move(old._functions);
	old._functions = nlohmann::json::object();
}

liboai::Functions& liboai::Functions::operator=(const Functions& other) {
	this->_functions = other._functions;
	return *this;
}

liboai::Functions& liboai::Functions::operator=(Functions&& old) noexcept {
	this->_functions = std::move(old._functions);
	old._functions = nlohmann::json::object();
	return *this;
}

bool liboai::Functions::AddFunction(std::string_view function_name) & noexcept(false) {
	if (this->GetFunctionIndex(function_name) == -1) {
		this->_functions["functions"].push_back({ {"name", function_name} });
		return true; // function added
	}
	return false; // function already exists
}

bool liboai::Functions::AddFunctions(std::initializer_list<std::string_view> function_names) & noexcept(false) {
	if (function_names.size() > 0) {
		for (auto& function_name : function_names) {
			if (this->GetFunctionIndex(function_name) == -1) {
				this->_functions["functions"].push_back({ {"name", function_name} });
			}
		}
		return true; // functions added
	}
	return false; // functions not added (size 0)
}

bool liboai::Functions::AddFunctions(std::vector<std::string> function_names) & noexcept(false) {
	if (function_names.size() > 0) {
		for (auto& function_name : function_names) {
			if (this->GetFunctionIndex(function_name) == -1) {
				this->_functions["functions"].push_back({ {"name", std::move(function_name)} });
			}
		}
		return true; // functions added
	}
	return false; // functions not added (size 0)
}

bool liboai::Functions::PopFunction(std::string_view function_name) & noexcept(false) {
	auto index = this->GetFunctionIndex(function_name);

	if (index != -1) {
		this->_functions["functions"].erase(this->_functions["functions"].begin() + index);
		return true; // function removed
	}

	return false; // function not removed
}

bool liboai::Functions::PopFunctions(std::initializer_list<std::string_view> function_names) & noexcept(false) {
	if (function_names.size() > 0) {
		for (auto& function_name : function_names) {
			auto index = this->GetFunctionIndex(function_name);

			if (index != -1) {
				this->_functions["functions"].erase(this->_functions["functions"].begin() + index);
			}
		}
		
		return true; // functions removed
	}
	
	return false; // functions not removed (size 0)
}

bool liboai::Functions::PopFunctions(std::vector<std::string> function_names) & noexcept(false) {
	if (function_names.size() > 0) {
		for (auto& function_name : function_names) {
			auto index = this->GetFunctionIndex(function_name);

			if (index != -1) {
				this->_functions["functions"].erase(this->_functions["functions"].begin() + index);
			}
		}
		return true; // functions removed
	}

	return false; // functions not removed (size 0)
}

bool liboai::Functions::SetDescription(std::string_view target, std::string_view description) & noexcept(false) {
	index i = this->GetFunctionIndex(target);
	
	if (i != -1) {
		if (!this->_functions["functions"][i].contains("description")) {
			this->_functions["functions"][i]["description"] = description;
			return true; // description set successfully
		}
		return false; // already has a description
	}
	
	return false; // function does not exist
}

bool liboai::Functions::PopDescription(std::string_view target) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("description")) {
			this->_functions["functions"][i].erase("description");
			return true; // description removed successfully
		}
		return false; // does not have a description
	}
	
	return false; // function does not exist
}

bool liboai::Functions::SetRequired(std::string_view target, std::initializer_list<std::string_view> params) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1 && params.size() > 0) {
		if (this->_functions["functions"][i].contains("parameters")) {
			for (auto& parameter : params) {
				this->_functions["functions"][i]["parameters"]["required"] = std::move(params);
				return true; // required parameters set successfully
			}
		}
	}

	return false; // required parameters not set
}

bool liboai::Functions::SetRequired(std::string_view target, std::vector<std::string> params) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1 && params.size() > 0) {
		if (this->_functions["functions"][i].contains("parameters")) {
			for (auto& parameter : params) {
				this->_functions["functions"][i]["parameters"]["required"] = std::move(params);
				return true; // required parameters set successfully
			}
		}
	}

	return false; // required parameters not set
}

bool liboai::Functions::PopRequired(std::string_view target) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("parameters")) {
			if (this->_functions["functions"][i]["parameters"].contains("required")) {
				this->_functions["functions"][i]["parameters"].erase("required");
				return true; // required parameters removed successfully
			}
		}
	}

	return false; // required parameters not removed
}

bool liboai::Functions::AppendRequired(std::string_view target, std::string_view param) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("parameters")) {
			if (this->_functions["functions"][i]["parameters"].contains("required")) {
				this->_functions["functions"][i]["parameters"]["required"].push_back(param);
				return true; // required parameter appended successfully
			}
		}
	}

	return false; // required parameter not appended
}

bool liboai::Functions::AppendRequired(std::string_view target, std::initializer_list<std::string_view> params) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1 && params.size() > 0) {
		if (this->_functions["functions"][i].contains("parameters")) {
			if (this->_functions["functions"][i]["parameters"].contains("required")) {
				for (auto& param : params) {
					this->_functions["functions"][i]["parameters"]["required"].push_back(param);
				}
				
				return true; // required parameters appended successfully
			}
		}
	}

	return false; // required parameters not appended
}

bool liboai::Functions::AppendRequired(std::string_view target, std::vector<std::string> params) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1 && params.size() > 0) {
		if (this->_functions["functions"][i].contains("parameters")) {
			if (this->_functions["functions"][i]["parameters"].contains("required")) {
				for (auto& param : params) {
					this->_functions["functions"][i]["parameters"]["required"].push_back(std::move(param));
				}

				return true; // required parameters appended successfully
			}
		}
	}

	return false; // required parameters not appended
}

bool liboai::Functions::SetParameter(std::string_view target, FunctionParameter parameter) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (!this->_functions["functions"][i].contains("parameters")) {
			this->_functions["functions"][i]["parameters"] = nlohmann::json::object();
			this->_functions["functions"][i]["parameters"]["properties"] = nlohmann::json::object();
			this->_functions["functions"][i]["parameters"]["type"] = "object";
			
			this->_functions["functions"][i]["parameters"]["properties"].push_back(
				{ parameter.name, {
					{ "type", std::move(parameter.type) },
					{ "description", std::move(parameter.description) }
				}}
			);

			if (parameter.enumeration) {
				this->_functions["functions"][i]["parameters"]["properties"][parameter.name]["enum"] = std::move(parameter.enumeration.value());
			}

			return true; // parameter set successfully
		}
	}

	return false; // function non-existent, or parameters already set (use AppendParameter(s))
}

bool liboai::Functions::SetParameters(std::string_view target, std::initializer_list<FunctionParameter> parameters) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (!this->_functions["functions"][i].contains("parameters") && parameters.size() > 0) {
			this->_functions["functions"][i]["parameters"] = nlohmann::json::object();
			this->_functions["functions"][i]["parameters"]["properties"] = nlohmann::json::object();
			this->_functions["functions"][i]["parameters"]["type"] = "object";

			for (auto& parameter : parameters) {
				if (!this->_functions["functions"][i]["parameters"]["properties"].contains(parameter.name)) {
					this->_functions["functions"][i]["parameters"]["properties"].push_back(
						{ parameter.name, {
							{ "type", parameter.type },
							{ "description", parameter.description }
						} }
					);

					if (parameter.enumeration) {
						this->_functions["functions"][i]["parameters"]["properties"][parameter.name]["enum"] = parameter.enumeration.value();
					}
				}
			}

			return true; // parameter set successfully
		}
	}

	return false; // function non-existent, or parameters already set (use AppendParameter(s))
}

bool liboai::Functions::SetParameters(std::string_view target, std::vector<FunctionParameter> parameters) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (!this->_functions["functions"][i].contains("parameters") && parameters.size() > 0) {
			this->_functions["functions"][i]["parameters"] = nlohmann::json::object();
			this->_functions["functions"][i]["parameters"]["properties"] = nlohmann::json::object();
			this->_functions["functions"][i]["parameters"]["type"] = "object";

			for (auto& parameter : parameters) {
				if (!this->_functions["functions"][i]["parameters"]["properties"].contains(parameter.name)) {
					this->_functions["functions"][i]["parameters"]["properties"].push_back(
						{ parameter.name, {
							{ "type", std::move(parameter.type) },
							{ "description", std::move(parameter.description) }
						} }
					);

					if (parameter.enumeration) {
						this->_functions["functions"][i]["parameters"]["properties"][parameter.name]["enum"] = std::move(parameter.enumeration.value());
					}
				}
			}

			return true; // parameter set successfully
		}
	}

	return false; // function non-existent, or parameters already set (use AppendParameter(s))
}

bool liboai::Functions::PopParameters(std::string_view target) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("parameters")) {
			this->_functions["functions"][i].erase("parameters");
			return true; // parameters removed successfully
		}
	}

	return false; // parameters not removed
}

bool liboai::Functions::PopParameters(std::string_view target, std::initializer_list<std::string_view> param_names) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("parameters")) {
			for (auto& param_name : param_names) {
				if (this->_functions["functions"][i]["parameters"]["properties"].contains(param_name)) {
					this->_functions["functions"][i]["parameters"]["properties"].erase(param_name);
				}
			}

			return true; // parameters removed successfully
		}
	}

	return false; // parameters not removed
}

bool liboai::Functions::PopParameters(std::string_view target, std::vector<std::string> param_names) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("parameters")) {
			for (auto& param_name : param_names) {
				if (this->_functions["functions"][i]["parameters"]["properties"].contains(param_name)) {
					this->_functions["functions"][i]["parameters"]["properties"].erase(param_name);
				}
			}

			return true; // parameters removed successfully
		}
	}
	
	return false; // parameters not removed
}

bool liboai::Functions::AppendParameter(std::string_view target, FunctionParameter parameter) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("parameters")) {
			if (!this->_functions["functions"][i]["parameters"]["properties"].contains(parameter.name)) {
				this->_functions["functions"][i]["parameters"]["properties"].push_back(
					{ parameter.name, {
						{ "type", std::move(parameter.type) },
						{ "description", std::move(parameter.description) }
					}}
				);

				if (parameter.enumeration) {
					this->_functions["functions"][i]["parameters"]["properties"][parameter.name]["enum"] = std::move(parameter.enumeration.value());
				}

				return true; // parameter appended successfully
			}
		}
	}

	return false; // parameter not appended
}

bool liboai::Functions::AppendParameters(std::string_view target, std::initializer_list<FunctionParameter> parameters) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("parameters")) {
			for (auto& parameter : parameters) {
				if (!this->_functions["functions"][i]["parameters"]["properties"].contains(parameter.name)) {
					this->_functions["functions"][i]["parameters"]["properties"].push_back(
						{ parameter.name, {
							{ "type", parameter.type },
							{ "description", parameter.description }
						} }
					);

					if (parameter.enumeration) {
						this->_functions["functions"][i]["parameters"]["properties"][parameter.name]["enum"] = parameter.enumeration.value();
					}
				}
			}

			return true; // parameters appended successfully
		}
	}

	return false; // parameters not appended
}

bool liboai::Functions::AppendParameters(std::string_view target, std::vector<FunctionParameter> parameters) & noexcept(false) {
	index i = this->GetFunctionIndex(target);

	if (i != -1) {
		if (this->_functions["functions"][i].contains("parameters")) {
			for (auto& parameter : parameters) {
				if (!this->_functions["functions"][i]["parameters"]["properties"].contains(parameter.name)) {
					this->_functions["functions"][i]["parameters"]["properties"].push_back(
						{ parameter.name, {
							{ "type", std::move(parameter.type) },
							{ "description", std::move(parameter.description) }
						} }
					);

					if (parameter.enumeration) {
						this->_functions["functions"][i]["parameters"]["properties"][parameter.name]["enum"] = std::move(parameter.enumeration.value());
					}
				}
			}

			return true; // parameters appended successfully
		}
	}

	return false; // parameters not appended
}

const nlohmann::json& liboai::Functions::GetJSON() const & noexcept {
	return this->_functions;
}

liboai::Functions::index liboai::Functions::GetFunctionIndex(std::string_view function_name) const & noexcept(false) {
	index i = 0;

	if (!this->_functions.empty()) {
		for (auto& [key, value] : this->_functions["functions"].items()) {
			if (value.contains("name")) {
				if (value["name"].get<std::string>() == function_name) {
					return i;
				}
			}
			i++;
		}
	}
	
	return -1;
}
