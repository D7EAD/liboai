#pragma once

/*
	network.h : liboai network implementation.
		This header file provides declarations for the abstracted liboai
		Network implementation. Each component class will inherit from
		this class to make use of the network functionality provided by
		it.

		For instance, making a call to liboai::Image::Create(...) will
		make use of both this class to send the request to the OpenAI API
		and liboai::Authorization to provide the user's authorization
		information to successfully complete the request.
*/

#include <optional>
#include <future>
#include "netimpl.h"

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
			static inline bool Download(
				const std::string& to,
				const std::string& from,
				netimpl::components::Header authorization
			) noexcept(false) {
				std::ofstream file(to, std::ios::binary);
				Response res;
				res = netimpl::Download(
					file,
					netimpl::components::Url{ from },
					std::move(authorization)
				);

				return res.status_code == 200;
			}

			/*
				@brief Function to asynchronously download a
					file at 'from' to file path 'to.' Useful
					for downloading images from the OpenAI API
					given a URL to 'from.'

					This function is not to be confused with
					liboai::File::download(...) which is used
					to download .jsonl files from the OpenAI API.

				@param *to     The path and filename to download the file to.
				@param *from   Where to download the file data from
					(such as a URL).

				@returns Future bool indicating success or failure.
			*/
			static inline std::future<bool> DownloadAsync(
				const std::string& to,
				const std::string& from,
				netimpl::components::Header authorization
			) noexcept(false) {
				return std::async(
					std::launch::async, [&]() -> bool {
						std::ofstream file(to, std::ios::binary);
						Response res;
						res = netimpl::Download(
							file,
							netimpl::components::Url{ from },
							std::move(authorization)
						);

						return res.status_code == 200;
					}
				);
			}
			
		protected:
			enum class Method : uint8_t {
				HTTP_GET,     // GET
				HTTP_POST,    // POST
				HTTP_DELETE   // DELETE
			};

			template <class... _Params,
				std::enable_if_t<std::conjunction_v<std::negation<std::is_lvalue_reference<_Params>>...>, int> = 0>
			inline Response Request(
				const Method& http_method,
				const std::string& root,
				const std::string& endpoint,
				const std::string& content_type,
				std::optional<netimpl::components::Header> headers = std::nullopt,
				_Params&&... parameters
			) const {
				netimpl::components::Header _headers = { { "Content-Type", content_type } };
				if (headers) {
					if (headers.value().size() != 0) {
						for (auto& i : headers.value()) {
							_headers.insert(std::move(i));
						}
					}
				}
				
				Response res;
				if constexpr (sizeof...(parameters) > 0) {
					res = Network::MethodSchema<netimpl::components::Header&&, _Params&&...>::_method[static_cast<uint8_t>(http_method)](
						netimpl::components::Url { root + endpoint },
						std::move(_headers),
						std::forward<_Params>(parameters)...
					);
				}
				else {
					res = Network::MethodSchema<netimpl::components::Header&&>::_method[static_cast<uint8_t>(http_method)](
						netimpl::components::Url { root + endpoint },
						std::move(_headers)
					);
				}			

				return res;
			}

			/*
				@brief Function to validate the existence and validity of
					a file located at a provided file path. This is used
					in functions that take a file path as a parameter
					to ensure that the file exists and is valid.
			*/
			bool Validate(const std::filesystem::path& path) const {
				// checks if the file exists, is a regular file, and is not empty
				if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
					return std::filesystem::file_size(path) > 0;
				}
				return false;
			}
			
			const std::string openai_root_ = "https://api.openai.com/v1";
			const std::string azure_root_ = ".openai.azure.com/openai";

		private:
			template <class... T> struct MethodSchema {
				inline static std::function<Response(netimpl::components::Url&&, T...)> _method[3] = {
					netimpl::Get    <netimpl::components::Url&&, T...>,
					netimpl::Post   <netimpl::components::Url&&, T...>,
					netimpl::Delete <netimpl::components::Url&&, T...>
				};
			};
	};
}