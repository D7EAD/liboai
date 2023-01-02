#include "../include/components/models.h"

liboai::Response liboai::Models::list() const & {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_GET, "/models", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::Models::retrieve(const std::string& model) const& {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_GET, "/models/" + model, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::Models::remove(const std::string& model) const& {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_DELETE, "/models/" + model, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}