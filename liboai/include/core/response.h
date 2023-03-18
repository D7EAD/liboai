#pragma once

/*
	respone.h : liboai response container implementation.
		This header file provides declarations for the liboai Response
		implementation. Each component class will include this header
		and use the Response class to return data to the user.
		
		For instance, making a call to liboai::Image::Create(...) will
		return a liboai::Response object. The user can then check the
		object and retrieve the data found in the response as needed.

		This class will construct itself from the output of
		liboai::Network::Request(...) (cpr::Response) and parse it
		into a usable format for the user to access via this class.
*/

#ifdef __linux__
	#define LIBOAI_EXPORT
#else
	#define LIBOAI_EXPORT __declspec(dllexport)
#endif

#include <iostream>
#include <optional>
#include <future>
#include <nlohmann/json.hpp>
#include "exception.h"

namespace liboai {
	class JsonConstructor final {
		public:
			JsonConstructor() {}
			JsonConstructor(const JsonConstructor& other) noexcept : _json(other._json) {}
			JsonConstructor(JsonConstructor&& old) noexcept : _json(std::move(old._json)) {}

			template <class _Ty>
			void push_back(std::string_view key, const _Ty& value) {
				if constexpr (std::is_same_v<_Ty, std::optional<std::function<bool(std::string, intptr_t)>>>) {
					if (value) {
						this->_json[key.data()] = true;
					}
				}
				else {
					this->_json[key.data()] = value;
				}
			}

			template <class _Ty>
			void push_back(std::string_view key, std::optional<_Ty>&& value) {
				if (value) {
					this->_json[key.data()] = std::forward<_Ty>(value.value());
				}
			}

			std::string dump() const {
				return this->_json.dump(4);
			}

		private:
			nlohmann::json _json;
	};

	class Response final {
		public:
			Response() = default;
			Response(const liboai::Response& other) noexcept;
			Response(liboai::Response&& old) noexcept;
			Response(
				std::string&& url,
				std::string&& content,
				std::string&& status_line,
				std::string&& reason,
				long status_code,
				double elapsed
			) noexcept(false);
			
			Response& operator=(const liboai::Response& other) noexcept;
			Response& operator=(liboai::Response&& old) noexcept;
			
			/*
				@brief Transparent operator[] wrapper to nlohmann::json to
					access the Response object as if it were a json object.
			*/
			template <class _Ty>
			nlohmann::json::const_reference operator[](const _Ty& key) const noexcept {
				return this->raw_json[key];
			}
			
			/*
				@brief std::ostream operator<< overload to allow for
					pretty printing of the Response object.
			*/
			LIBOAI_EXPORT friend std::ostream& operator<<(std::ostream& os, const Response& r);
			
		public:
			long status_code = 0; double elapsed = 0.0;
			std::string status_line{}, content{}, url{}, reason{};
			nlohmann::json raw_json{};

		private:
			/*
				@brief Used internally during construction to check the response
					for errors and throw exceptions if necessary.
			*/
			LIBOAI_EXPORT void CheckResponse() const noexcept(false);
	};
	using FutureResponse = std::future<liboai::Response>;
}