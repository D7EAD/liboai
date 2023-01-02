#include "../include/components/images.h"

liboai::Response liboai::Images::create(const std::string& prompt, const std::optional<uint8_t>& n, const std::optional<std::string>& size, const std::optional<std::string>& response_format, const std::optional<std::string>& user) const & {
	nlohmann::json json_data = {
		{ "prompt", prompt }
	};
	if (n.has_value()) { json_data["n"] = n.value(); }
	if (size.has_value()) { json_data["size"] = size.value(); }
	if (response_format.has_value()) { json_data["response_format"] = response_format.value(); }
	if (user.has_value()) { json_data["user"] = user.value(); }

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/images/generations", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			json_data.dump()
		},
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::Images::create_edit(const std::filesystem::path& image, const std::string& prompt, const std::optional<std::filesystem::path>& mask, const std::optional<uint8_t>& n, const std::optional<std::string>& size, const std::optional<std::string>& response_format, const std::optional<std::string>& user) const & {
	cpr::Multipart form = {
		{ "prompt", prompt },
		{ "image", cpr::File{image.generic_string()} }
	};
	if (mask.has_value()) { form.parts.push_back({ "mask", cpr::File{mask.value().generic_string()} }); }
	if (n.has_value()) { form.parts.push_back({ "n", n.value() }); }
	if (size.has_value()) { form.parts.push_back({ "size", size.value() }); }
	if (response_format.has_value()) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (user.has_value()) { form.parts.push_back({ "user", user.value() }); }
	
	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/images/edits", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::Images::create_variation(const std::filesystem::path& image, const std::optional<uint8_t>& n, const std::optional<std::string>& size, const std::optional<std::string>& response_format, const std::optional<std::string>& user) const & {
	cpr::Multipart form = {
		{ "image", cpr::File{image.generic_string()} }
	};
	if (n.has_value()) { form.parts.push_back({ "n", n.value() }); }
	if (size.has_value()) { form.parts.push_back({ "size", size.value() }); }
	if (response_format.has_value()) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (user.has_value()) { form.parts.push_back({ "user", user.value() }); }
	
	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/images/variations", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}