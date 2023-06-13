#include "../include/components/models.h"

liboai::Response liboai::Models::list() const & noexcept(false) {
	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/models", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Models::list_async() const & noexcept(false) {
	auto _fn = [this]() -> liboai::Response {
		return this->Request(
			Method::HTTP_GET, this->openai_root_, "/models", "application/json",
			this->auth_.GetAuthorizationHeaders(),
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};
	
	return std::async(std::launch::async, _fn);
}

liboai::Response liboai::Models::retrieve(const std::string& model) const & noexcept(false) {
	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/models/" + model, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::Models::retrieve_async(const std::string& model) const & noexcept(false) {
	auto _fn = [this](const std::string& model) -> liboai::Response {
		return this->Request(
			Method::HTTP_GET, this->openai_root_, "/models/" + model, "application/json",
			this->auth_.GetAuthorizationHeaders(),
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};

	return std::async(std::launch::async, _fn, model);
}