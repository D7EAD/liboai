#include "../include/components/images.h"

liboai::Response liboai::Images::create(const std::string& prompt, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("prompt", prompt);
	jcon.push_back("n", std::move(n));
	jcon.push_back("size", std::move(size));
	jcon.push_back("response_format", std::move(response_format));
	jcon.push_back("user", std::move(user));

	Response res;
	res = this->Request(
		Method::HTTP_POST, "/images/generations", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		netimpl::components::Body {
			jcon.dump()
		},
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth()
	);

	return res;
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
				netimpl::components::Body {
					jcon.dump()
				},
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth()
			);
		}
	);
}

liboai::Response liboai::Images::create_edit(const std::filesystem::path& image, const std::string& prompt, std::optional<std::filesystem::path> mask, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	if (!this->Validate(image)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Images::create_edit(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "prompt", prompt },
		{ "image", netimpl::components::File{image.generic_string()} }
	};
	
	if (mask) {
		if (!this->Validate(mask.value())) {
			throw liboai::exception::OpenAIException(
				"File path provided is non-existent, is not a file, or is empty.",
				liboai::exception::EType::E_FILEERROR,
				"liboai::Images::create_edit(...)"
			);
		}
		form.parts.push_back({ "mask", netimpl::components::File{mask.value().generic_string()} });
	}
	if (n) { form.parts.push_back({ "n", n.value() }); }
	if (size) { form.parts.push_back({ "size", size.value() }); }
	if (response_format) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (user) { form.parts.push_back({ "user", user.value() }); }
	
	Response res;
	res = this->Request(
		Method::HTTP_POST, "/images/edits", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth()
	);

	return res;
}

liboai::FutureResponse liboai::Images::create_edit_async(const std::filesystem::path& image, const std::string& prompt, std::optional<std::filesystem::path> mask, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	if (!this->Validate(image)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Images::create_edit_async(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "prompt", prompt },
		{ "image", netimpl::components::File{image.generic_string()} }
	};
	
	if (mask) {
		if (!this->Validate(mask.value())) {
			throw liboai::exception::OpenAIException(
				"File path provided is non-existent, is not a file, or is empty.",
				liboai::exception::EType::E_FILEERROR,
				"liboai::Images::create_edit_async(...)"
			);
		}
		form.parts.push_back({ "mask", netimpl::components::File{mask.value().generic_string()} });
	}
	if (n) { form.parts.push_back({ "n", n.value() }); }
	if (size) { form.parts.push_back({ "size", size.value() }); }
	if (response_format) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (user) { form.parts.push_back({ "user", user.value() }); }

	return std::async(
		std::launch::async, [&, form]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/images/edits", "multipart/form-data",
				this->auth_.GetAuthorizationHeaders(),
				std::move(form),
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth()
			);
		}
	);
}

liboai::Response liboai::Images::create_variation(const std::filesystem::path& image, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	if (!this->Validate(image)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Images::create_variation(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "image", netimpl::components::File{image.generic_string()} }
	};
	
	if (n) { form.parts.push_back({ "n", n.value() }); }
	if (size) { form.parts.push_back({ "size", size.value() }); }
	if (response_format) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (user) { form.parts.push_back({ "user", user.value() }); }
	
	Response res;
	res = this->Request(
		Method::HTTP_POST, "/images/variations", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth()
	);

	return res;
}

liboai::FutureResponse liboai::Images::create_variation_async(const std::filesystem::path& image, std::optional<uint8_t> n, std::optional<std::string> size, std::optional<std::string> response_format, std::optional<std::string> user) const & noexcept(false) {
	if (!this->Validate(image)) {
		throw liboai::exception::OpenAIException(
			"File path provided is non-existent, is not a file, or is empty.",
			liboai::exception::EType::E_FILEERROR,
			"liboai::Images::create_variation_async(...)"
		);
	}

	netimpl::components::Multipart form = {
		{ "image", netimpl::components::File{image.generic_string()} }
	};
	
	if (n) { form.parts.push_back({ "n", n.value() }); }
	if (size) { form.parts.push_back({ "size", size.value() }); }
	if (response_format) { form.parts.push_back({ "response_format", response_format.value() }); }
	if (user) { form.parts.push_back({ "user", user.value() }); }

	return std::async(
		std::launch::async, [&, form]() -> liboai::Response {
			return this->Request(
				Method::HTTP_POST, "/images/variations", "multipart/form-data",
				this->auth_.GetAuthorizationHeaders(),
				std::move(form),
				this->auth_.GetProxies(),
				this->auth_.GetProxyAuth()
			);
		}
	);
}