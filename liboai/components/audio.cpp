#include "../include/components/audio.h"

liboai::Response liboai::Audio::transcribe(const std::filesystem::path& file, const std::string& model, std::optional<std::string> prompt, std::optional<std::string> response_format, std::optional<float> temperature, std::optional<std::string> language) const& noexcept(false) {
	if (!this->Validate(file)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Audio::transcribe(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "file", netimpl::components::File{file.generic_string()} },
		{ "model", model }
	};
	
	if (prompt) { form.parts.push_back({ "prompt", prompt.value() }); }
	if (response_format) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (temperature) { form.parts.push_back({ "temperature", std::to_string(temperature.value()) }); }
	if (language) { form.parts.push_back({ "language", language.value() }); }

	Response res;
	res = this->Request(
		Method::HTTP_POST, "/audio/transcriptions", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Audio::transcribe_async(const std::filesystem::path& file, const std::string& model, std::optional<std::string> prompt, std::optional<std::string> response_format, std::optional<float> temperature, std::optional<std::string> language) const& noexcept(false) {
	if (!this->Validate(file)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Audio::transcribe(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "file", netimpl::components::File{file.generic_string()} },
		{ "model", model }
	};

	if (prompt) { form.parts.push_back({ "prompt", prompt.value() }); }
	if (response_format) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (temperature) { form.parts.push_back({ "temperature", std::to_string(temperature.value()) }); }
	if (language) { form.parts.push_back({ "language", language.value() }); }

	return std::async(
		std::launch::async, [&, form]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/audio/transcriptions", "multipart/form-data",
				this->auth_.GetAuthorizationHeaders(),
				std::move(form),
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}

liboai::Response liboai::Audio::translate(const std::filesystem::path& file, const std::string& model, std::optional<std::string> prompt, std::optional<std::string> response_format, std::optional<float> temperature) const& noexcept(false) {
	if (!this->Validate(file)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Audio::translate(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "file", netimpl::components::File{file.generic_string()} },
		{ "model", model }
	};
	
	if (prompt) { form.parts.push_back({ "prompt", std::move(prompt.value()) }); }
	if (response_format) { form.parts.push_back({ "response_format", std::move(response_format.value()) }); }
	if (temperature) { form.parts.push_back({ "temperature", std::to_string(temperature.value()) }); }

	Response res;
	res = this->Request(
		Method::HTTP_POST, "/audio/translations", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Audio::translate_async(const std::filesystem::path& file, const std::string& model, std::optional<std::string> prompt, std::optional<std::string> response_format, std::optional<float> temperature) const& noexcept(false) {
	if (!this->Validate(file)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Audio::translate(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "file", netimpl::components::File{file.generic_string()} },
		{ "model", model }
	};

	if (prompt) { form.parts.push_back({ "prompt", std::move(prompt.value()) }); }
	if (response_format) { form.parts.push_back({ "response_format", std::move(response_format.value()) }); }
	if (temperature) { form.parts.push_back({ "temperature", std::to_string(temperature.value()) }); }

	return std::async(
		std::launch::async, [&, form]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/audio/translations", "multipart/form-data",
				this->auth_.GetAuthorizationHeaders(),
				std::move(form),
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth(),
				this->auth_.GetMaxTimeout()
			);
		}
	);
}