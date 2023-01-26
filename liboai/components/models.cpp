#include "../include/components/models.h"

liboai::Response liboai::Models::list() const & noexcept(false) {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_GET, "/models", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Models::list_async() const & noexcept(false) {
	return std::async(
		std::launch::async, [&]() -> liboai::Response {
			return this->Request(
				Method::HTTP_GET, "/models", "application/json",
				this->auth_.GetAuthorizationHeaders(),
				this->auth_.GetProxies()
			);
		}
	);
}

liboai::Response liboai::Models::retrieve(const std::string& model) const & noexcept(false) {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_GET, "/models/" + model, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Models::retrieve_async(const std::string& model) const & noexcept(false) {
	return std::async(
		std::launch::async, [&]() -> liboai::Response {
			return this->Request(
				Method::HTTP_GET, "/models/" + model, "application/json",
				this->auth_.GetAuthorizationHeaders(),
				this->auth_.GetProxies()
			);
		}
	);
}

liboai::Response liboai::Models::remove(const std::string& model) const & noexcept(false) {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_DELETE, "/models/" + model, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return liboai::Response(std::move(res));
}

liboai::FutureResponse liboai::Models::remove_async(const std::string& model) const & noexcept(false) {
	return std::async(
		std::launch::async, [&]() -> liboai::Response {
			return this->Request(
				Method::HTTP_DELETE, "/models/" + model, "application/json",
				this->auth_.GetAuthorizationHeaders(),
				this->auth_.GetProxies()
			);
		}
	);
}