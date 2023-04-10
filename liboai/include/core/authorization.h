#pragma once

/*
	authorization.h : liboai authorization header.
		This header file provides declarations for authorization
		directives for authorizing requests with the OpenAI API.
		Each component class makes use of a single object accessed
		via liboai::Authorization::Authorizer() to retrieve and use
		user-set authorization information to successfully complete
		component API requests.
*/

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include "network.h"

namespace liboai {
	class Authorization final {
		public: // cons/des, operator deletions
			Authorization() = default;
			~Authorization() = default;

			// non-copyable, singleton paradigm
			Authorization(Authorization const&) = delete;
			void operator=(Authorization const&) = delete;

			// non-movable, singleton paradigm
			Authorization(Authorization&&) = delete;
			void operator=(Authorization&&) = delete;

		public:	// member methods
			/*
				@brief Singleton paradigm access method.
				@return A reference to the singleton instance of this class
					to be used in all component classes.
			*/
			static Authorization& Authorizer() noexcept {
				static Authorization instance;
				return instance;
			}
			
			/*
				@brief Sets the authorization key for the OpenAI API
					as the passed string.
				@param key : The authorization key to use in component calls.
				@returns True if the key was set successfully, false otherwise.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetKey(std::string_view key) noexcept;

			/*
				@brief Sets the authorization key for the OpenAI API
					as the first line present in the file at the passed path.
				@param path : The path to the file containing the authorization key.
				@returns True if the key was set successfully, false otherwise.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetKeyFile(const std::filesystem::path& path) noexcept;

			/*
				@brief Sets the authorization key for the OpenAI API
					as the value stored in the environment variable with
					the passed name.
				@param var : The name of the environment variable to
					retrieve the authorization key from.
				@returns True if the key was set successfully, false otherwise.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetKeyEnv(std::string_view var) noexcept;
			
			/*
				@brief Sets the organization identifier as the passed
					string for use in component calls.
				@param org : The organization identifier to use in
					component calls.
				@returns True if the ID was set successfully, false otherwise.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetOrganization(std::string_view org) noexcept;

			/*
				@brief Sets the organization identifier as the first
					line present in the file at the passed path for use
					in component calls.
				@param path : The path to the file containing the
					organization identifier.
				@returns True if the ID was set successfully, false otherwise.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetOrganizationFile(const std::filesystem::path& path) noexcept;

			/*
				@brief Sets the organization identifier as the value
					stored in the environment variable with the passed
					name for use in component calls.
				@param var : The name of the environment variable to
					retrieve the organization identifier from.
				@returns True if the ID was set successfully, false otherwise.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetOrganizationEnv(std::string_view var) noexcept;
			
			/*
				@brief Sets proxies to use for component calls.
				@param hosts : The hosts to use as proxies in 
					paired { "protocol", "host" } format.
			*/
			LIBOAI_EXPORT void SetProxies(const std::initializer_list<std::pair<const std::string, std::string>>& hosts) noexcept;
			
			/*
				@brief Sets proxies to use for component calls.
				@param hosts : The hosts to use as proxies in
					paired { "protocol", "host" } format.
			*/
			LIBOAI_EXPORT void SetProxies(std::initializer_list<std::pair<const std::string, std::string>>&& hosts) noexcept;

			/*
				@brief Sets proxies to use for component calls.
				@param hosts : The hosts to use as proxies in
					paired { "protocol", "host" } format.
			*/
			LIBOAI_EXPORT void SetProxies(const std::map<std::string, std::string>& hosts) noexcept;
			
			/*
				@brief Sets proxies to use for component calls.
				@param hosts : The hosts to use as proxies in
					paired { "protocol", "host" } format.
			*/
			LIBOAI_EXPORT void SetProxies(std::map<std::string, std::string>&& hosts) noexcept;
			
			/*
				@brief Sets authentication information for proxies per-protocol.
				
				@param proto_up : A {protocol, {uname, passwd}} map to use for
					authentication with proxies on a per-protocol basis.
			*/
			LIBOAI_EXPORT void SetProxyAuth(const std::map<std::string, netimpl::components::EncodedAuthentication>& proto_up) noexcept;

			/*			
				@brief Returns currently the set authorization key.
			*/
			constexpr const std::string& GetKey() const noexcept { return this->key_; }

			/*
				@brief Returns the currently set organization identifier.
			*/
			constexpr const std::string& GetOrganization() const noexcept { return this->org_; }
			
			/*
				@returns The currently set proxies.
			*/
			netimpl::components::Proxies GetProxies() const noexcept { return this->proxies_; }
			
			/*
				@returns The currently set proxy authentication information.
			*/
			netimpl::components::ProxyAuthentication GetProxyAuth() const noexcept { return this->proxyAuth_; }

			/*
				@returns An authorization header with the
					currently set authorization information for use
					in component calls.
			*/
			constexpr const netimpl::components::Header& GetAuthorizationHeaders() const noexcept { return this->auth_headers_; }

		private: // member variables
			std::string key_, org_;
			netimpl::components::Header auth_headers_;
			netimpl::components::Proxies proxies_;
			netimpl::components::ProxyAuthentication proxyAuth_;
	};
}