#pragma once

/*
	chat.h : Chat component header file
		This class contains all the methods for the Chat component
		of the OpenAI API. This class provides access to 'Chat'
		endpoints on the OpenAI API and should be accessed via the
		liboai.h header file through an instantiated liboai::OpenAI
		object after setting necessary authentication information
		through the liboai::Authorization::Authorizer() singleton
		object.
*/

#include "../core/authorization.h"
#include "../core/response.h"

namespace liboai {
	class ChatCompletion final : private Network {
		public:
			ChatCompletion() = default;
			~ChatCompletion() = default;
			ChatCompletion(const ChatCompletion&) = delete;
			ChatCompletion(ChatCompletion&&) = delete;

			ChatCompletion& operator=(const ChatCompletion&) = delete;
			ChatCompletion& operator=(ChatCompletion&&) = delete;

			/*
				@brief Class containing, and used for keeping track of, the chat history.
					An object of this class should be created, set with system and user data,
					and provided to ChatCompletion::create (system is optional).

					The general usage of this class is as follows:
						1. Create a ChatCompletion::Conversation object.
						2. Optinally set the system data to guide how the assistant responds.
						3. Set the user data, which is the user's input - such as
						   a question or a command.
						4. Provide the ChatCompletion::Conversation object to
						   ChatCompletion::create.
						5. Update the ChatCompletion::Conversation object with
						   the response from the API - either the object or the
						   response content can be used to update the object.
						6. Retrieve the assistant's response from the
						   ChatCompletion::Conversation object; repeat steps 3, 4, and 5
						   until the conversation is complete.
					
					After providing the object to ChatCompletion::create, the object will
					be updated with the 'assistant' response - this response is the
					assistant's response to the user's input. A developer could then
					retrieve this response and display it to the user, and then set the
					next user input in the object and pass it back to ChatCompletion::create,
					if desired.
			*/
			class Conversation final {
				public:
					Conversation();
					~Conversation() = default;
					Conversation(const Conversation& other);
					Conversation(Conversation&& old) noexcept;
					
					Conversation(std::string_view system_data);
					Conversation(std::string_view system_data, std::string_view user_data);
					Conversation(std::string_view system_data, std::initializer_list<std::string_view> user_data);
					Conversation(std::initializer_list<std::string_view> user_data);
				
					Conversation& operator=(const Conversation& other);
					Conversation& operator=(Conversation&& old) noexcept;

					friend std::ostream& operator<<(std::ostream& os, const Conversation& conv);

					/*
						@brief Sets the system data for the conversation.
							This method sets the system data for the conversation.
							The system data is the data that helps set the behavior
							of the assistant so it knows how to respond.

							@param *data      The system data to set.
					*/
					LIBOAI_EXPORT bool SetSystemData(std::string_view data) & noexcept(false);

					/*
						@brief Removes the set system data from the top of the conversation.
							The system data must be the first data set, if used,
							in order to be removed. If the system data is not
							the first data set, this method will return false.
					*/
					LIBOAI_EXPORT bool PopSystemData() & noexcept(false);

					/*
						@brief Adds user input to the conversation.
							This method adds user input to the conversation.
							The user input is the user's input - such as a question
							or a command.

							If using a system prompt, the user input should be
							provided after the system prompt is set - i.e. after
							SetSystemData() is called.

							@param *data      The user input to add.
					*/
					LIBOAI_EXPORT bool AddUserData(std::string_view data) & noexcept(false);
					
					/*
						@brief Removes the last added user data.
					*/
					LIBOAI_EXPORT bool PopUserData() & noexcept(false);

					/*
						@brief Gets the last response from the assistant.
							This method gets the last response from the assistant.
							The response is the assistant's response to the user's
							input.
					*/
					LIBOAI_EXPORT std::string GetLastResponse() const & noexcept;

					/*
						@brief Removes the last assistant response.
					*/
					LIBOAI_EXPORT bool PopLastResponse() & noexcept(false);

					/*
						@brief Updates the conversation given JSON data.
							This method updates the conversation given JSON data.
							The JSON data should be the JSON 'messages' data returned
							from the OpenAI API.

							@param *history      The JSON data to update the conversation with.
									     This should be the 'messages' array of data returned
									     from a call to ChatCompletion::create.
					*/
					LIBOAI_EXPORT bool Update(std::string_view history) & noexcept(false);

					/*
						@brief Updates the conversation given a Response object.
							This method updates the conversation given a Response object.

							@param *response     The Response to update the conversation with.
								             This should be the Response returned from a call
									     to ChatCompletion::create.
					*/
					LIBOAI_EXPORT bool Update(const Response& response) & noexcept(false);

					/*
						@brief Returns the raw JSON dump of the internal conversation object
							in string format.
					*/
					LIBOAI_EXPORT std::string GetRawConversation() const & noexcept;

					/*
						@brief Returns the JSON object of the internal conversation.
					*/
					LIBOAI_EXPORT nlohmann::json GetJSON() const & noexcept;
					
				private:
					nlohmann::json _conversation;
			};

			/*
				@brief Creates a completion for the chat message.

				@param *model            ID of the model to use. Currently,
				                         only gpt-3.5-turbo and gpt-3.5-turbo-0301 
							 are supported.
				@param *conversation     A Conversation object containing the
							 conversation data.
				@param temperature       What sampling temperature to use,
				                         between 0 and 2. Higher values like 0.8 will
							 make the output more random, while lower values
							 like 0.2 will make it more focused and deterministic.
				@param top_p             An alternative to sampling with temperature, called
				                         nucleus sampling, where the model considers the results
							 of the tokens with top_p probability mass. So 0.1 means
							 only the tokens comprising the top 10% probability mass
							 are considered.
				@param n                 How many chat completion choices to generate for each
				                         input message.
				@param stream            If set, partial message deltas will be sent, like in
				                         ChatGPT. Tokens will be sent as data-only server-sent
							 events as they become available, with the stream terminated
							 by a data: [DONE] message.
				@param stop              Up to 4 sequences where the API will stop generating further
				                         tokens.
				@param max_tokens        The maximum number of tokens allowed for the generated answer.
				                         By default, the number of tokens the model can return will be
						         (4096 - prompt tokens).
				@param presence_penalty  Number between -2.0 and 2.0. Positive values penalize new tokens
				                         based on whether they appear in the text so far, increasing the
							 model's likelihood to talk about new topics.
				@param frequency_penalty Number between -2.0 and 2.0. Positive values penalize new tokens
							 based on their existing frequency in the text so far, decreasing
							 the model's likelihood to repeat the same line verbatim.
				@param logit_bias        Modify the likelihood of specified tokens appearing in the completion.
				@param user              The user ID to associate with the request. This is used to
							 prevent abuse of the API.

				@returns A liboai::Response object containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response create(
				const std::string& model,
				const Conversation& conversation,
				std::optional<float> temperature = std::nullopt,
				std::optional<float> top_p = std::nullopt,
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
				@param *model            ID of the model to use. Currently,
				                         only gpt-3.5-turbo and gpt-3.5-turbo-0301 
							 are supported.
				@param *conversation     A Conversation object containing the
							 conversation data.
				@param temperature       What sampling temperature to use,
				                         between 0 and 2. Higher values like 0.8 will
							 make the output more random, while lower values
							 like 0.2 will make it more focused and deterministic.
				@param top_p             An alternative to sampling with temperature, called
				                         nucleus sampling, where the model considers the results
							 of the tokens with top_p probability mass. So 0.1 means
							 only the tokens comprising the top 10% probability mass
							 are considered.
				@param n                 How many chat completion choices to generate for each
				                         input message.
				@param stream            If set, partial message deltas will be sent, like in
				                         ChatGPT. Tokens will be sent as data-only server-sent
							 events as they become available, with the stream terminated
							 by a data: [DONE] message.
				@param stop              Up to 4 sequences where the API will stop generating further
				                         tokens.
				@param max_tokens        The maximum number of tokens allowed for the generated answer.
				                         By default, the number of tokens the model can return will be
						         (4096 - prompt tokens).
				@param presence_penalty  Number between -2.0 and 2.0. Positive values penalize new tokens
				                         based on whether they appear in the text so far, increasing the
							 model's likelihood to talk about new topics.
				@param frequency_penalty Number between -2.0 and 2.0. Positive values penalize new tokens
							 based on their existing frequency in the text so far, decreasing
							 the model's likelihood to repeat the same line verbatim.
				@param logit_bias        Modify the likelihood of specified tokens appearing in the completion.
				@param user              The user ID to associate with the request. This is used to
							 prevent abuse of the API.

				@returns A liboai::Response future containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse create_async(
				const std::string& model,
				const Conversation& conversation,
				std::optional<float> temperature = std::nullopt,
				std::optional<float> top_p = std::nullopt,
				std::optional<uint16_t> n = std::nullopt,
				std::optional<std::function<bool(std::string, intptr_t)>> stream = std::nullopt,
				std::optional<std::vector<std::string>> stop = std::nullopt,
				std::optional<uint16_t> max_tokens = std::nullopt,
				std::optional<float> presence_penalty = std::nullopt,
				std::optional<float> frequency_penalty = std::nullopt,
				std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
				std::optional<std::string> user = std::nullopt
			) const& noexcept(false);

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
	using Conversation = ChatCompletion::Conversation;
}
