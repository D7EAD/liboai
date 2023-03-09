#include "../include/components/audio.h"

liboai::Response liboai::Audio::transcribe(const std::filesystem::path& file, const std::string& model, std::optional<std::string> prompt, std::optional<std::string> response_format, std::optional<float> temperature, std::optional<std::string> language) const& noexcept(false) {
	cpr::Multipart form = {
		{ "file", cpr::File{file.generic_string()} },
		{ "model", model }
	};
	
	if (prompt) { form.parts.push_back({ "prompt", prompt.value() }); }
	if (response_format) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (temperature) { form.parts.push_back({ "temperature", std::to_string(temperature.value()) }); }
	if (language) { form.parts.push_back({ "language", language.value() }); }

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/audio/transcriptions", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::Response liboai::Audio::translate(const std::filesystem::path& file, const std::string& model, std::optional<std::string> prompt, std::optional<std::string> response_format, std::optional<float> temperature) const& noexcept(false) {
	cpr::Multipart form = {
		{ "file", cpr::File{file.generic_string()} },
		{ "model", model }
	};
	
	if (prompt) { form.parts.push_back({ "prompt", std::move(prompt.value()) }); }
	if (response_format) { form.parts.push_back({ "response_format", std::move(response_format.value()) }); }
	if (temperature) { form.parts.push_back({ "temperature", std::to_string(temperature.value()) }); }

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/audio/translations", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}