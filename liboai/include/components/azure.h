#pragma once

/*
	azure.h : Azure component class for OpenAI.
		Azure provides their own API for access to the OpenAI API.
		This class provides methods that, provided that the proper
		Azure authentication information has been set, allows users
		to access the OpenAI API through Azure.
*/

#include "../core/authorization.h"
#include "../core/response.h"
#include "chat.h"

namespace liboai {
	class Azure final : private Network {
		public:
			Azure() = default;
			~Azure() = default;
			Azure(const Azure&) = delete;
			Azure(Azure&&) = delete;

			Azure& operator=(const Azure&) = delete;
			Azure& operator=(Azure&&) = delete;

			/*
				@brief Given a prompt, the model will return one or more
					predicted completions, and can also return the
					probabilities of alternative tokens at each position.

				@param *resource_name    The name of your Azure OpenAI Resource.
				@param *deployment_id    The deployment name you chose when you deployed the model.
				@param *api_version      The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param Refer to liboai::Completions::create for more information on the remaining parameters.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response create_completion(
				const std::string& resource_name,
				const std::string& deployment_id,
				const std::string& api_version,
				std::optional<std::string> prompt = std::nullopt,
				std::optional<std::string> suffix = std::nullopt,
				std::optional<uint16_t> max_tokens = std::nullopt,
				std::optional<float> temperature = std::nullopt,
				std::optional<float> top_p = std::nullopt,
				std::optional<uint16_t> n = std::nullopt,
				std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
				std::optional<uint8_t> logprobs = std::nullopt,
				std::optional<bool> echo = std::nullopt,
				std::optional<std::vector<std::string>> stop = std::nullopt,
				std::optional<float> presence_penalty = std::nullopt,
				std::optional<float> frequency_penalty = std::nullopt,
				std::optional<uint16_t> best_of = std::nullopt,
				std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);
			
			/*
				@brief Given a prompt, the model will asynchronously return
				    one or more	predicted completions, and can also return the
					probabilities of alternative tokens at each position.

				@param *resource_name    The name of your Azure OpenAI Resource.
				@param *deployment_id    The deployment name you chose when you deployed the model.
				@param *api_version      The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param Refer to liboai::Completions::create for more information on the remaining parameters.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse create_completion_async(
				const std::string& resource_name,
				const std::string& deployment_id,
				const std::string& api_version,
				std::optional<std::string> prompt = std::nullopt,
				std::optional<std::string> suffix = std::nullopt,
				std::optional<uint16_t> max_tokens = std::nullopt,
				std::optional<float> temperature = std::nullopt,
				std::optional<float> top_p = std::nullopt,
				std::optional<uint16_t> n = std::nullopt,
				std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
				std::optional<uint8_t> logprobs = std::nullopt,
				std::optional<bool> echo = std::nullopt,
				std::optional<std::vector<std::string>> stop = std::nullopt,
				std::optional<float> presence_penalty = std::nullopt,
				std::optional<float> frequency_penalty = std::nullopt,
				std::optional<uint16_t> best_of = std::nullopt,
				std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);

			/*
				@brief Creates an embedding vector representing the input text.

				@param *resource_name    The name of your Azure OpenAI Resource.
				@param *deployment_id    The deployment name you chose when you deployed the model.
				@param *api_version      The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *input            Input text to get embeddings for, encoded as a string. The number of input tokens
										 varies depending on what model you are using. 
				@param Refer to liboai::Embeddings::create for more information on the remaining parameters.

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response create_embedding(
				const std::string& resource_name,
				const std::string& deployment_id,
				const std::string& api_version,
				const std::string& input,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);

			/*
				@brief Asynchronously creates an embedding vector representing the input text.

				@param *resource_name    The name of your Azure OpenAI Resource.
				@param *deployment_id    The deployment name you chose when you deployed the model.
				@param *api_version      The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *input            Input text to get embeddings for, encoded as a string. The number of input tokens
										 varies depending on what model you are using.
				@param Refer to liboai::Embeddings::create for more information on the remaining parameters.

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse create_embedding_async(
				const std::string& resource_name,
				const std::string& deployment_id,
				const std::string& api_version,
				const std::string& input,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);

			/*
				@brief Creates a completion for the chat message.

				@param *resource_name    The name of your Azure OpenAI Resource.
				@param *deployment_id    The deployment name you chose when you deployed the model.
				@param *api_version      The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *conversation     A Conversation object containing the conversation data.
				@param Refer to liboai::Chat::create for more information on the remaining parameters.

				@returns A liboai::Response object containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response create_chat_completion(
				const std::string& resource_name,
				const std::string& deployment_id,
				const std::string& api_version,
				const Conversation& conversation,
				std::optional<float> temperature = std::nullopt,
				std::optional<uint16_t> n = std::nullopt,
				std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
				std::optional<std::vector<std::string>> stop = std::nullopt,
				std::optional<uint16_t> max_tokens = std::nullopt,
				std::optional<float> presence_penalty = std::nullopt,
				std::optional<float> frequency_penalty = std::nullopt,
				std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);

			/*
				@brief Asynchronously creates a completion for the chat message.

				@param *resource_name    The name of your Azure OpenAI Resource.
				@param *deployment_id    The deployment name you chose when you deployed the model.
				@param *api_version      The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *conversation     A Conversation object containing the conversation data.
				@param Refer to liboai::Chat::create for more information on the remaining parameters.

				@returns A liboai::Response object containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse create_chat_completion_async(
				const std::string& resource_name,
				const std::string& deployment_id,
				const std::string& api_version,
				const Conversation& conversation,
				std::optional<float> temperature = std::nullopt,
				std::optional<uint16_t> n = std::nullopt,
				std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
				std::optional<std::vector<std::string>> stop = std::nullopt,
				std::optional<uint16_t> max_tokens = std::nullopt,
				std::optional<float> presence_penalty = std::nullopt,
				std::optional<float> frequency_penalty = std::nullopt,
				std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);

			/*
				@brief Generate a batch of images from a text caption.
					Image generation is currently only available with api-version=2023-06-01-preview.

				@param *resource_name  The name of your Azure OpenAI Resource.
				@param *api_version    The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *prompt         The text to create an image from.
				@param n               The number of images to create.
				@param size            The size of the image to create.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response request_image_generation(
				const std::string& resource_name,
				const std::string& api_version,
				const std::string& prompt,
				std::optional<uint8_t> n = std::nullopt,
				std::optional<std::string> size = std::nullopt
			) const & noexcept(false);

			/*
				@brief Asynchronously generate a batch of images from a text caption.
					Image generation is currently only available with api-version=2023-06-01-preview.

				@param *resource_name  The name of your Azure OpenAI Resource.
				@param *api_version    The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *prompt         The text to create an image from.
				@param n               The number of images to create.
				@param size            The size of the image to create.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse request_image_generation_async(
				const std::string& resource_name,
				const std::string& api_version,
				const std::string& prompt,
				std::optional<uint8_t> n = std::nullopt,
				std::optional<std::string> size = std::nullopt
			) const & noexcept(false);

			/*
				@brief Retrieve the results (URL) of a previously called image generation operation.

				@param *resource_name  The name of your Azure OpenAI Resource.
				@param *api_version    The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *operation_id   The GUID that identifies the original image generation request.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response get_generated_image(
				const std::string& resource_name,
				const std::string& api_version,
				const std::string& operation_id
			) const & noexcept(false);

			/*
				@brief Asynchronously retrieve the results (URL) of a previously called image generation operation.

				@param *resource_name  The name of your Azure OpenAI Resource.
				@param *api_version    The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *operation_id   The GUID that identifies the original image generation request.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse get_generated_image_async(
				const std::string& resource_name,
				const std::string& api_version,
				const std::string& operation_id
			) const & noexcept(false);

			/*
				@brief Deletes the corresponding image from the Azure server.

				@param *resource_name  The name of your Azure OpenAI Resource.
				@param *api_version    The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *operation_id   The GUID that identifies the original image generation request.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response delete_generated_image(
				const std::string& resource_name,
				const std::string& api_version,
				const std::string& operation_id
			) const & noexcept(false);

			/*
				@brief Asynchronously deletes the corresponding image from the Azure server.

				@param *resource_name  The name of your Azure OpenAI Resource.
				@param *api_version    The API version to use for this operation. This follows the YYYY-MM-DD format.
				@param *operation_id   The GUID that identifies the original image generation request.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse delete_generated_image_async(
				const std::string& resource_name,
				const std::string& api_version,
				const std::string& operation_id
			) const & noexcept(false);

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}