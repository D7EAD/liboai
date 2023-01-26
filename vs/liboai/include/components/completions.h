#pragma once

/*
	completions.h : Completions component class for OpenAI.
		This class contains all the methods for the Completions component
		of the OpenAI API. This class provides access to 'Completions' 
		endpoints on the OpenAI API and should be accessed via the
		liboai.h header file through an instantiated liboai::OpenAI
		object after setting necessary authentication information
		through the liboai::Authorization::Authorizer() singleton
		object.
*/

#include "../core/authorization.h"
#include "../core/response.h"

namespace liboai {
	class Completions final : private Network {
		public:
			Completions() = default;
			~Completions() = default;
			Completions(const Completions&) = delete;
			Completions(Completions&&) = delete;
			
			Completions& operator=(const Completions&) = delete;
			Completions& operator=(Completions&&) = delete;

			/*
				@brief Given a prompt, the model will return one or more
					predicted completions, and can also return the
					probabilities of alternative tokens at each position.

				@param *model             The model to use for completion.
				@param prompt             The prompt(s) to generate completions for.
				@param suffix             The suffix that comes after a completion of inserted text.
				@param max_tokens         The maximum number of tokens to generate in a completion.
				@param temperature        The temperature for the model. Higher values will result in more
											creative completions, while lower values will result in more
											repetitive completions.
				@param top_p              The top_p for the model. This is the probability mass that the
											model will consider when making predictions. Lower values will
										    result in more creative completions, while higher values will
										    result in more repetitive completions.
				@param n                  The number of completions to generate.
				@param stream             Stream partial progress back to the client. A callback function
											that is called each time new data is received from the API. If
											no callback is supplied, this parameter is disabled and the
											API will wait until the completion is finished before returning
											the response.
											
				@param logprobs           The number of log probabilities to return for each token.
				@param echo               Whether to include the prompt in the returned completion.
				@param stop               A list of tokens that the model will stop generating completions
											at. This can be a single token or a list of tokens.
				@param presence_penalty   The presence penalty for the model. This is a number between
											-2.0 and 2.0. Positive values penalize new tokens based on
											whether they appear in the text so far, increasing the model's
											likelihood to talk about new topics.
				@param frequency_penalty  The frequency penalty for the model. This is a number between
											-2.0 and 2.0. Positive values penalize new tokens based on
											their existing frequency in the text so far, decreasing the
											model's likelihood to repeat the same line verbatim.
				@param best_of            Generates best_of completions server-side and returns the "best" 
											one. When used with n, best_of controls the number of candidate
											completions and n specifies how many to return – best_of must be
											greater than n

											Because this parameter generates many completions, it can quickly
											consume your token quota. Use carefully and ensure that you have
											reasonable settings for max_tokens and stop.
				@param logit_bias         Modify the likelihood of specified tokens appearing in the completion.
											Accepts a json object that maps tokens (specified by their token ID
											in the GPT tokenizer) to an associated bias value from -100 to 100. 
				@param user               A unique identifier representing your end-user.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response create(
				const std::string& model_id,
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
				@brief Given a prompt, the model will return one or more
					predicted completions asynchronously, and can also
					return the probabilities of alternative tokens at each
					position.

				@param *model             The model to use for completion.
				@param prompt             The prompt(s) to generate completions for.
				@param suffix             The suffix that comes after a completion of inserted text.
				@param max_tokens         The maximum number of tokens to generate in a completion.
				@param temperature        The temperature for the model. Higher values will result in more
											creative completions, while lower values will result in more
											repetitive completions.
				@param top_p              The top_p for the model. This is the probability mass that the
											model will consider when making predictions. Lower values will
										    result in more creative completions, while higher values will
										    result in more repetitive completions.
				@param n                  The number of completions to generate.
				@param stream             Stream partial progress back to the client. A callback function
											that is called each time new data is received from the API. If
											no callback is supplied, this parameter is disabled and the
											API will wait until the completion is finished before returning
											the response.
											
				@param logprobs           The number of log probabilities to return for each token.
				@param echo               Whether to include the prompt in the returned completion.
				@param stop               A list of tokens that the model will stop generating completions
											at. This can be a single token or a list of tokens.
				@param presence_penalty   The presence penalty for the model. This is a number between
											-2.0 and 2.0. Positive values penalize new tokens based on
											whether they appear in the text so far, increasing the model's
											likelihood to talk about new topics.
				@param frequency_penalty  The frequency penalty for the model. This is a number between
											-2.0 and 2.0. Positive values penalize new tokens based on
											their existing frequency in the text so far, decreasing the
											model's likelihood to repeat the same line verbatim.
				@param best_of            Generates best_of completions server-side and returns the "best" 
											one. When used with n, best_of controls the number of candidate
											completions and n specifies how many to return – best_of must be
											greater than n

											Because this parameter generates many completions, it can quickly
											consume your token quota. Use carefully and ensure that you have
											reasonable settings for max_tokens and stop.
				@param logit_bias         Modify the likelihood of specified tokens appearing in the completion.
											Accepts a json object that maps tokens (specified by their token ID
											in the GPT tokenizer) to an associated bias value from -100 to 100. 
				@param user               A unique identifier representing your end-user.

				@returns A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse create_async(
				const std::string& model_id,
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

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}