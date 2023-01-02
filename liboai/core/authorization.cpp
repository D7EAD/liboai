#include "../include/core/authorization.h"

bool liboai::Authorization::SetKey(const std::string& key) {
	if (!key.empty()) {
		this->key_ = key;
		if (this->auth_headers_.count("Authorization") > 0) {
			this->auth_headers_.erase("Authorization");
		}
		this->auth_headers_["Authorization"] = ("Bearer " + this->key_);
		return true;
	}
	return false;
}

bool liboai::Authorization::SetKey(std::string&& key) {
	if (!key.empty()) {
		this->key_ = std::move(key);
		if (this->auth_headers_.count("Authorization") > 0) {
			this->auth_headers_.erase("Authorization");
		}
		this->auth_headers_["Authorization"] = ("Bearer " + this->key_);
		return true;
	}
	return false;
}

bool liboai::Authorization::SetKeyFile(const std::filesystem::path& path) {
	if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path) && std::filesystem::file_size(path) > 0) {
		std::ifstream file(path);
		if (file.is_open()) {
			std::getline(file, this->key_);
			if (this->auth_headers_.count("Authorization") > 0) {
				this->auth_headers_.erase("Authorization");
			}
			this->auth_headers_["Authorization"] = ("Bearer " + this->key_);
			return true;
		}
	}
	return false;
}

bool liboai::Authorization::SetKeyEnv(const std::string& var) {
	if (!var.empty()) {
		const char* key = std::getenv(var.c_str());
		if (key != nullptr) {
			this->key_ = key;
			if (this->auth_headers_.count("Authorization") > 0) {
				this->auth_headers_.erase("Authorization");
			}
			this->auth_headers_["Authorization"] = ("Bearer " + this->key_);
			return true;
		}
		return false;
	}
	return false;
}

bool liboai::Authorization::SetOrganization(const std::string& org) {
	if (!org.empty()) {
		this->org_ = org;
		if (this->auth_headers_.count("OpenAI-Organization") > 0) {
			this->auth_headers_.erase("OpenAI-Organization");
		}
		this->auth_headers_["OpenAI-Organization"] = this->org_;
		return true;
	}
	return false;
}

bool liboai::Authorization::SetOrganization(std::string&& org) {
	if (!org.empty()) {
		this->org_ = std::move(org);
		if (this->auth_headers_.count("OpenAI-Organization") > 0) {
			this->auth_headers_.erase("OpenAI-Organization");
		}
		this->auth_headers_["OpenAI-Organization"] = this->org_;
		return true;
	}
	return false;
}

bool liboai::Authorization::SetOrganizationFile(const std::filesystem::path& path) {
	if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path) && std::filesystem::file_size(path) > 0) {
		std::ifstream file(path);
		if (file.is_open()) {
			std::getline(file, this->key_);
			if (this->auth_headers_.count("OpenAI-Organization") > 0) {
				this->auth_headers_.erase("OpenAI-Organization");
			}
			this->auth_headers_["OpenAI-Organization"] = this->org_;
			return true;
		}
	}
	return false;
}

bool liboai::Authorization::SetOrganizationEnv(const std::string& var) {
	if (!var.empty()) {
		const char* org = std::getenv(var.c_str());
		if (org != nullptr) {
			this->org_ = org;
			if (this->auth_headers_.count("OpenAI-Organization") > 0) {
				this->auth_headers_.erase("OpenAI-Organization");
			}
			this->auth_headers_["OpenAI-Organization"] = this->org_;
			return true;
		}
		return false;
	}
	return false;
}

void liboai::Authorization::SetProxies(const std::initializer_list<std::pair<const std::string, std::string>>& hosts) {
	this->proxies_ = cpr::Proxies(hosts);
}

void liboai::Authorization::SetProxies(std::initializer_list<std::pair<const std::string, std::string>>&& hosts) {
	this->proxies_ = cpr::Proxies(std::move(hosts));
}

void liboai::Authorization::SetProxies(const std::map<std::string, std::string>& hosts) {	
	this->proxies_ = cpr::Proxies(hosts);
}

void liboai::Authorization::SetProxies(std::map<std::string, std::string>&& hosts) {
	this->proxies_ = cpr::Proxies(std::move(hosts));
}