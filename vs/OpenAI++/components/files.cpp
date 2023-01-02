#include "../include/components/files.h"

liboai::Response liboai::Files::list() const & {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_GET, "/files", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::Files::create(const std::filesystem::path& file, const std::string& purpose) const & {
	cpr::Multipart form = {
		{ "purpose", purpose },
		{ "file", cpr::File{file.generic_string()} }
	};
	
	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/files", "multipart/form-data",
		this->auth_.GetAuthorizationHeaders(),
		std::move(form),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::Files::remove(const std::string& file_id) const & {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_DELETE, "/files/" + file_id, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::Files::retrieve(const std::string& file_id) const & {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_GET, "/files/" + file_id, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

bool liboai::Files::download(const std::string& file_id, const std::string& save_to) const & {
	return Network::Download(
		save_to, ("https://api.openai.com/v1/files/" + file_id + "/content")
	);
}