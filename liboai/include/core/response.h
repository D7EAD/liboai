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
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include "exception.h"

namespace liboai {
	class Response final {
		public:
			Response() = delete;
			Response(const Response&) = delete;
			Response(Response&&) = delete;
			
			Response(const cpr::Response& toParse) noexcept(false)
				: status_code(toParse.status_code), elapsed(toParse.elapsed), status_line(toParse.status_line),
				content(toParse.text), url(toParse.url.str()), reason(toParse.reason) 
			{
				try {
					if (!this->content.empty()) {
						this->raw_json = nlohmann::json::parse(this->content);
					}
					else {
						this->raw_json = nlohmann::json();
					}
				}
				catch (const nlohmann::json::parse_error& e) {
					throw liboai::exception::OpenAIException(
						e.what(),
						liboai::exception::EType::E_FAILURETOPARSE,
						"liboai::Response::Response(const cpr::Response&)"
					);
				}
				
				// check the response for errors -- nothrow on success
				this->CheckResponse();
			}
			
			Response(cpr::Response&& toParse) noexcept(false)
				: status_code(toParse.status_code), elapsed(toParse.elapsed), status_line(std::move(toParse.status_line)),
				content(std::move(toParse.text)), url(toParse.url.str()), reason(std::move(toParse.reason))
			{
				try {
					if (!this->content.empty()) {
						this->raw_json = nlohmann::json::parse(this->content);
					}
					else {
						this->raw_json = nlohmann::json();
					}
				}
				catch (nlohmann::json::parse_error& e) {
					throw liboai::exception::OpenAIException(
						e.what(),
						liboai::exception::EType::E_FAILURETOPARSE,
						"liboai::Response::Response(cpr::Response&&)"
					);
				}

				// check the response for errors -- nothrow on success
				this->CheckResponse();
			}
			
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
			friend std::ostream& operator<<(std::ostream& os, const Response& r);
			
		public:
			long status_code; double elapsed;
			std::string status_line, content, url, reason;
			nlohmann::json raw_json;

		private:
			/*
				@brief Used internally during construction to check the response
					for errors and throw exceptions if necessary.
			*/
			LIBOAI_EXPORT void CheckResponse() const noexcept(false);
	};
}
