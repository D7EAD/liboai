#include "../include/components/images.h"

liboai::Response liboai::Images::create(const std::string& prompt, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("prompt", prompt);
	jcon.push_back("n", std::move(n));
	jcon.push_back("size", std::move(size));
	jcon.push_back("response_format", std::move(response_format));
	jcon.push_back("user", std::move(user));

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/images/generations", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			jcon.dump()
		},
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Images::create_async(const std::string& prompt, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("prompt", prompt);
	jcon.push_back("n", std::move(n));
	jcon.push_back("size", std::move(size));
	jcon.push_back("response_format", std::move(response_format));
	jcon.push_back("user", std::move(user));

	return std::async(
		std::launch::async, [&, jcon]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/images/generations", "application/json",
				this->auth_.GetAuthorizationHeaders(),
				cpr::Body {
					jcon.dump()
				},
				this->auth_.GetProxies()
			);
		}
	);
}

liboai::Response liboai::Images::create_edit(const std::filesystem::path& image, const std::string& prompt, std::optional<std::filesystem::path> mask, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
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

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Images::create_edit_async(const std::filesystem::path& image, const std::string& prompt, std::optional<std::filesystem::path> mask, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	cpr::Multipart form = {
		{ "prompt", prompt },
		{ "image", cpr::File{image.generic_string()} }
	};
	if (mask.has_value()) { form.parts.push_back({ "mask", cpr::File{mask.value().generic_string()} }); }
	if (n.has_value()) { form.parts.push_back({ "n", n.value() }); }
	if (size.has_value()) { form.parts.push_back({ "size", size.value() }); }
	if (response_format.has_value()) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (user.has_value()) { form.parts.push_back({ "user", user.value() }); }

	return std::async(
		std::launch::async, [&, form]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/images/edits", "multipart/form-data",
				this->auth_.GetAuthorizationHeaders(),
				std::move(form),
				this->auth_.GetProxies()
			);
		}
	);
}

liboai::Response liboai::Images::create_variation(const std::filesystem::path& image, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
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

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Images::create_variation_async(const std::filesystem::path& image, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	cpr::Multipart form = {
		{ "image", cpr::File{image.generic_string()} }
	};
	if (n.has_value()) { form.parts.push_back({ "n", n.value() }); }
	if (size.has_value()) { form.parts.push_back({ "size", size.value() }); }
	if (response_format.has_value()) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (user.has_value()) { form.parts.push_back({ "user", user.value() }); }

	return std::async(
		std::launch::async, [&, form]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/images/variations", "multipart/form-data",
				this->auth_.GetAuthorizationHeaders(),
				std::move(form),
				this->auth_.GetProxies()
			);
		}
	);
}