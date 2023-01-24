#pragma once

/*
	network.h : liboai network implementation.
		This header file provides declarations for the liboai Network
		implementation. Each component class will inherit from this
		class to make use of the network functionality provided by
		it.

		For instance, making a call to liboai::Image::Create(...) will
		make use of both this class to send the request to the OpenAI API
		and liboai::Authorization to provide the user's authorization
		information to successfully complete the request.
*/

#ifdef __linux__
	#define LIBOAI_EXPORT
#else
	#define LIBOAI_EXPORT __declspec(dllexport)
#endif

#include <optional>
#include <future>
#include <cpr/cpr.h>

namespace liboai {
	class Network {			
		public:
			Network() noexcept = default;
			Network(Network&&) = delete;
			Network(const Network&) = delete;
		
			/*
				@brief Function to download a file at 'from'
					to file path 'to.' Useful for downloading
					images from the OpenAI API given a URL to
					'from.'

					This function is not to be confused with
					liboai::File::download(...) which is used
					to download .jsonl files from the OpenAI API.

				@param *to     The path and filename to download the file to.
				@param *from   Where to download the file data from
					(such as a URL).

				@returns Bool indicating success or failure.
			*/
			static inline bool Download(const std::string& to, const std::string& from) noexcept(false) {
				std::ofstream file(to, std::ios::binary);
				cpr::Response res;
				res = cpr::Download(file, cpr::Url{ from });

				return res.status_code == 200;
			}

		protected:
			enum class Method : uint8_t {
				HTTP_GET,     // GET
				HTTP_POST,    // POST
				HTTP_DELETE   // DELETE
			};

			template <class... _Params,
				std::enable_if_t<std::conjunction_v<std::negation<std::is_lvalue_reference<_Params>>...>, int> = 0>
			inline cpr::Response Request(
				const Method& http_method,
				const std::string& endpoint,
				const std::string& content_type,
				std::optional<cpr::Header> headers = std::nullopt,
				_Params&&... parameters
			) const {
				cpr::Header _headers = { { "Content-Type", content_type } };
				if (headers) {
					if (headers.value().size() != 0) {
						for (auto& i : headers.value()) {
							_headers.insert(std::move(i));
						}
					}
				}
				
				cpr::Response res;
				if constexpr (sizeof...(parameters) > 0) {
					res = Network::MethodSchema<cpr::Timeout&&, cpr::Header&&, _Params&&...>::_method[static_cast<uint8_t>(http_method)](
						cpr::Url { this->root_ + endpoint },
						cpr::Timeout{ 30000 },
						std::move(_headers),
						std::forward<_Params>(parameters)...
					);
				}
				else {
					res = Network::MethodSchema<cpr::Timeout&&, cpr::Header&&>::_method[static_cast<uint8_t>(http_method)](
						cpr::Url { this->root_ + endpoint },
						cpr::Timeout{ 30000 },
						std::move(_headers)
					);
				}			

				return res;
			}

		private:
			template <class... T> struct MethodSchema {
				inline static std::function<cpr::Response(cpr::Url&&, T...)> _method[3] = {
					cpr::Get    <cpr::Url&&, T...>,
					cpr::Post   <cpr::Url&&, T...>,
					cpr::Delete <cpr::Url&&, T...>
				};
			};
			const std::string root_ = "https://api.openai.com/v1";
	};
}