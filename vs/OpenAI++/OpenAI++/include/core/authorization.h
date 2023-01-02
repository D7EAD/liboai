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
	class Authorization {
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
			static Authorization& Authorizer() {
				static Authorization instance;
				return instance;
			}
			
			/*
				@brief Sets the authorization key for the OpenAI API
					as the passed string.
				@param key : The authorization key to use in component calls.
				@returns True if the key was set successfully, false otherwise.
			*/
			LIBOAI_EXPORT bool SetKey(const std::string& key);
			
			/*
				@brief Sets the authorization key for the OpenAI API
					as the passed string.
				@param key : The authorization key to use in component calls.
				@returns True if the key was set successfully, false otherwise.
			*/
			LIBOAI_EXPORT bool SetKey(std::string&& key);

			/*
				@brief Sets the authorization key for the OpenAI API
					as the first line present in the file at the passed path.
				@param path : The path to the file containing the authorization key.
				@returns True if the key was set successfully, false otherwise.
			*/
			LIBOAI_EXPORT bool SetKeyFile(const std::filesystem::path& path);

			/*
				@brief Sets the authorization key for the OpenAI API
					as the value stored in the environment variable with
					the passed name.
				@param var : The name of the environment variable to
					retrieve the authorization key from.
				@returns True if the key was set successfully, false otherwise.
			*/
			LIBOAI_EXPORT bool SetKeyEnv(const std::string& var);
			
			/*
				@brief Sets the organization identifier as the passed
					string for use in component calls.
				@param org : The organization identifier to use in
					component calls.
				@returns True if the ID was set successfully, false otherwise.
			*/
			LIBOAI_EXPORT bool SetOrganization(const std::string& org);

			/*
				@brief Sets the organization identifier as the passed
					string for use in component calls.
				@param org : The organization identifier to use in
					component calls.
				@returns True if the ID was set successfully, false otherwise.
			*/
			LIBOAI_EXPORT bool SetOrganization(std::string&& org);

			/*
				@brief Sets the organization identifier as the first
					line present in the file at the passed path for use
					in component calls.
				@param path : The path to the file containing the
					organization identifier.
				@returns True if the ID was set successfully, false otherwise.
			*/
			LIBOAI_EXPORT bool SetOrganizationFile(const std::filesystem::path& path);

			/*
				@brief Sets the organization identifier as the value
					stored in the environment variable with the passed
					name for use in component calls.
				@param var : The name of the environment variable to
					retrieve the organization identifier from.
				@returns True if the ID was set successfully, false otherwise.
			*/
			LIBOAI_EXPORT bool SetOrganizationEnv(const std::string& var);
			
			/*
				@brief Sets proxies to use for component calls.
				@param hosts : The hosts to use as proxies in 
				paired { "protocol", "host" } format.
			*/
			LIBOAI_EXPORT void SetProxies(const std::initializer_list<std::pair<const std::string, std::string>>& hosts);
			
			/*
				@brief Sets proxies to use for component calls.
				@param hosts : The hosts to use as proxies in
				paired { "protocol", "host" } format.
			*/
			LIBOAI_EXPORT void SetProxies(std::initializer_list<std::pair<const std::string, std::string>>&& hosts);

			/*
				@brief Sets proxies to use for component calls.
				@param hosts : The hosts to use as proxies in
				paired { "protocol", "host" } format.
			*/
			LIBOAI_EXPORT void SetProxies(const std::map<std::string, std::string>& hosts);
			
			/*
				@brief Sets proxies to use for component calls.
				@param hosts : The hosts to use as proxies in
				paired { "protocol", "host" } format.
			*/
			LIBOAI_EXPORT void SetProxies(std::map<std::string, std::string>&& hosts);

			/*			
				@brief Returns currently the set authorization key.
			*/
			constexpr const std::string& GetKey() const { return this->key_; }

			/*
				@brief Returns the currently set organization identifier.
			*/
			constexpr const std::string& GetOrganization() const { return this->org_; }
			
			/*
				@returns The currently set proxies.
			*/
			cpr::Proxies GetProxies() { return this->proxies_; }
			
			/*
				@returns An authorization header with the
					currently set authorization information for use
					in component calls.
			*/
			constexpr const cpr::Header& GetAuthorizationHeaders() const { return this->auth_headers_; }

		private: // member variables
			std::string key_, org_;
			cpr::Header auth_headers_;
			cpr::Proxies proxies_;
	};
}