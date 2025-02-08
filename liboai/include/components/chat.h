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

#include <limits>

namespace liboai {
	/*
		@brief Class containing methods for building Function objects to supply
			to the OpenAI ChatCompletions component class via the associated
			Conversation class.
	*/
	class Functions final {
		public:
			Functions();
			Functions(const Functions& other);
			Functions(Functions&& old) noexcept;
			template <class... _Fname,
				std::enable_if_t<std::conjunction_v<std::is_convertible<_Fname, std::string_view>...>, int> = 0>
			Functions(_Fname... function_names) { auto result = this->AddFunctions(function_names...); }
			~Functions() = default;

			Functions& operator=(const Functions& other);
			Functions& operator=(Functions&& old) noexcept;
			
			/*
				@brief Denotes a parameter of a function, which includes
				the parameter's name, type, description, and an optional
				enumeration.

				@param name         The name of the parameter.
				@param type         The type of the parameter.
				@param description  The description of the parameter.
				@param enumeration  An optional enumeration of possible
									values for the parameter.
			*/
			struct FunctionParameter {
				FunctionParameter() = default;
				FunctionParameter(
					std::string_view name,
					std::string_view type,
					std::string_view description,
					std::optional<std::vector<std::string>> enumeration = std::nullopt
				) : name(name), type(type), description(description), enumeration(enumeration) {}
				FunctionParameter(const FunctionParameter& other) = default;
				FunctionParameter(FunctionParameter&& old) noexcept = default;
				~FunctionParameter() = default;

				FunctionParameter& operator=(const FunctionParameter& other) = default;
				FunctionParameter& operator=(FunctionParameter&& old) noexcept = default;

				std::string name;
				std::string type;
				std::string description;
				std::optional<std::vector<std::string>> enumeration;
			};

			/*
				@brief Adds a function named 'function_name' to the list of
					functions. This function, once added, can then be 
					referenced in subsequent 'Functions' class method calls
					by the name provided here.

				@param *function_name  The name of the function to add.

				@returns True/False denoting whether the function was added
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AddFunction(std::string_view function_name) & noexcept(false);

			/*
				@brief Same as AddFunction, but allows for adding multiple
					functions at once.

				@param *function_names  The name of the function to add.

				@returns True/False denoting whether the functions were added
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AddFunctions(std::initializer_list<std::string_view> function_names) & noexcept(false);

			/*
				@brief Same as AddFunction, but allows for adding multiple
					functions at once.

				@param *function_names  The name of the function to add.

				@returns True/False denoting whether the functions were added
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AddFunctions(std::vector<std::string> function_names) & noexcept(false);

			/*
				@brief Same as AddFunction, but allows for adding multiple
					functions at once.

				@param *function_names  The name of the function to add.

				@returns True/False denoting whether the functions were added
					successfully.
			*/
			template <class... _Fnames,
				std::enable_if_t<std::conjunction_v<std::is_convertible<_Fnames, std::string_view>...>, int> = 0>
			[[nodiscard]] bool AddFunctions(_Fnames... function_names) & noexcept(false) {
				return this->AddFunctions({ function_names... });
			}

			/*
				@brief Pops the specified function from the list of functions.
					This will also remove any associated name, description,
					parameters, and so on as it involves removing the entire
					'function_name' key from the JSON object.

				@param *function_name  The name of the function to pop.

				@returns True/False denoting whether the function was popped
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopFunction(std::string_view function_name) & noexcept(false);

			/*
				@brief Same as PopFunction, but allows for popping multiple
					functions at once.

				@param *function_names  The name of the function to pop.

				@returns True/False denoting whether the functions were popped
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopFunctions(std::initializer_list<std::string_view> function_names) & noexcept(false);
			
			/*
				@brief Same as PopFunction, but allows for popping multiple
					functions at once.

				@param *function_names  The name of the function to pop.

				@returns True/False denoting whether the functions were popped
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopFunctions(std::vector<std::string> function_names) & noexcept(false);
			
			/*
				@brief Same as PopFunction, but allows for popping multiple
					functions at once.

				@param *function_names  The name of the function to pop.

				@returns True/False denoting whether the functions were popped
					successfully.
			*/
			template <class... _Fnames,
				std::enable_if_t<std::conjunction_v<std::is_convertible<_Fnames, std::string_view>...>, int> = 0>
			[[nodiscard]] bool PopFunctions(_Fnames... function_names) & noexcept(false) {
				return this->PopFunctions({ function_names... });
			}

			/*
				@brief Sets a previously added function's description.
				
				@param *target       The name of the function to set the description of.
				@param *description  The description to set for the function.

				@returns True/False denoting whether the description was set
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetDescription(std::string_view target, std::string_view description) & noexcept(false);

			/*
				@brief Pops a previously added function's description.

				@param *target       The name of the function to pop the description of.

				@returns True/False denoting whether the description was popped
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopDescription(std::string_view target) & noexcept(false);

			/*
				@brief Sets which set function parameters are required.

				@param *target   The name of the function to set the required parameters of.
				@param *params   A series of parameter names to set as required.

				@returns True/False denoting whether the required parameters were set
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetRequired(std::string_view target, std::initializer_list<std::string_view> params) & noexcept(false);

			/*
				@brief Sets which set function parameters are required.

				@param *target   The name of the function to set the required parameters of.
				@param *params   A series of parameter names to set as required.

				@returns True/False denoting whether the required parameters were set
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetRequired(std::string_view target, std::vector<std::string> params) & noexcept(false);

			/*
				@brief Sets which set function parameters are required.

				@param *target   The name of the function to set the required parameters of.
				@param *params   A series of parameter names to set as required.

				@returns True/False denoting whether the required parameters were set
					successfully.
			*/
			template <class... _Rp,
				std::enable_if_t<std::conjunction_v<std::is_convertible<_Rp, std::string_view>...>, int> = 0>
			[[nodiscard]] bool SetRequired(std::string_view target, _Rp... params) & noexcept(false) {
				return SetRequired(target, { params... });
			}

			/*
				@brief Pops previously set required function parameters.

				@param *target   The name of the function to pop the required parameters of.

				@returns True/False denoting whether the required parameters were popped
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopRequired(std::string_view target) & noexcept(false);

			/*
				@brief Appends a parameter to a previously set series of required function
					parameters. This function should only be called if required parameters
					have already been set for 'target' via SetRequired().
				
				@param *target   The name of the function to append the required parameter to.
				@param *param    The name of the parameter to append to the required parameters.

				@returns True/False denoting whether the required parameter was appended
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AppendRequired(std::string_view target, std::string_view param) & noexcept(false);

			/*
				@brief Appends multiple parameters to a previously set series of required function
					parameters. This function should only be called if required parameters have
					already been set for 'target' via SetRequired().

				@param *target   The name of the function to append the required parameter to.
				@param *params   The name of the parameters to append to the required parameters.

				@returns True/False denoting whether the required parameter was appended
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AppendRequired(std::string_view target, std::initializer_list<std::string_view> params) & noexcept(false);

			/*
				@brief Appends multiple parameters to a previously set series of required function
					parameters. This function should only be called if required parameters have
					already been set for 'target' via SetRequired().

				@param *target   The name of the function to append the required parameter to.
				@param *params   The name of the parameters to append to the required parameters.

				@returns True/False denoting whether the required parameter was appended
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AppendRequired(std::string_view target, std::vector<std::string> params) & noexcept(false);

			/*
				@brief Appends multiple parameters to a previously set series of required function
					parameters. This function should only be called if required parameters have
					already been set for 'target' via SetRequired().

				@param *target   The name of the function to append the required parameter to.
				@param *params   The name of the parameters to append to the required parameters.

				@returns True/False denoting whether the required parameter was appended
					successfully.
			*/
			template <class... _Rp,
				std::enable_if_t<std::conjunction_v<std::is_convertible<_Rp, std::string_view>...>, int> = 0>
			[[nodiscard]] bool AppendRequired(std::string_view target, _Rp... params) & noexcept(false) {
				return AppendRequired(target, { params... });
			}

			/*
				@brief Adds a single parameter to an added function.

				@param *target       The name of the function to add the parameter to.
				@param *parameter    The parameter to add to the function.

				@returns True/False denoting whether the parameter was added
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetParameter(std::string_view target, FunctionParameter parameter) & noexcept(false);

			/*
				@brief Adds a series of parameters to an added function.

				@param *target       The name of the function to add the parameters to.
				@param *parameters   The parameters to add to the function.

				@returns True/False denoting whether the parameters were added
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetParameters(std::string_view target, std::initializer_list<FunctionParameter> parameters) & noexcept(false);

			/*
				@brief Adds a series of parameters to an added function.

				@param *target       The name of the function to add the parameters to.
				@param *parameters   The parameters to add to the function.

				@returns True/False denoting whether the parameters were added
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetParameters(std::string_view target, std::vector<FunctionParameter> parameters) & noexcept(false);

			/*
				@brief Adds a series of parameters to an added function.

				@param *target       The name of the function to add the parameters to.
				@param *parameters   The parameters to add to the function.

				@returns True/False denoting whether the parameters were added
					successfully.
			*/
			template <class... _Fp,
				std::enable_if_t<std::conjunction_v<std::is_same<_Fp, FunctionParameter>...>, int> = 0>
			[[nodiscard]] bool SetParameters(std::string_view target, _Fp... parameters) & noexcept(false) {
				return SetParameters(target, { parameters... });
			}

			/*
				@brief Pops all of a function's set parameters.
					This function removes set 'required' values and anything
					else that falls under the category of 'parameters' as a
					result of removing the entire 'parameters' section.

				@param *target   The name of the function to pop the parameters of.

				@returns True/False denoting whether the parameters were popped
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopParameters(std::string_view target) & noexcept(false);

			/*
				@brief Pops one or more of a function's set parameters.

				@param *target   The name of the function to pop the parameters of.
				@param *params   The names of the parameters to pop.

				@returns True/False denoting whether the parameters were popped
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopParameters(std::string_view target, std::initializer_list<std::string_view> param_names) & noexcept(false);

			/*
				@brief Pops one or more of a function's set parameters.

				@param *target   The name of the function to pop the parameters of.
				@param *params   The names of the parameters to pop.

				@returns True/False denoting whether the parameters were popped
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopParameters(std::string_view target, std::vector<std::string> param_names) & noexcept(false);

			/*
				@brief Pops one or more of a function's set parameters.

				@param *target   The name of the function to pop the parameters of.
				@param *params   The names of the parameters to pop.

				@returns True/False denoting whether the parameters were popped
					successfully.
			*/
			template <class... _Pname,
				std::enable_if_t<std::conjunction_v<std::is_convertible<_Pname, std::string_view>...>, int> = 0>
			[[nodiscard]] bool PopParameters(std::string_view target, _Pname... param_names) & noexcept(false) {
				return PopParameters(target, { param_names... });
			}

			/*
				@brief Appends a single parameter to a previously added function.

				@param *target       The name of the function to append the parameter to.
				@param *parameter    The parameter to append to the function.

				@returns True/False denoting whether the parameter was appended
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AppendParameter(std::string_view target, FunctionParameter parameter) & noexcept(false);

			/*
				@brief Appends a series of parameters to a previously added function.

				@param *target       The name of the function to append the parameters to.
				@param *parameters   The parameters to append to the function.

				@returns True/False denoting whether the parameters were appended
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AppendParameters(std::string_view target, std::initializer_list<FunctionParameter> parameters) & noexcept(false);

			/*
				@brief Appends a series of parameters to a previously added function.

				@param *target       The name of the function to append the parameters to.
				@param *parameters   The parameters to append to the function.

				@returns True/False denoting whether the parameters were appended
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AppendParameters(std::string_view target, std::vector<FunctionParameter> parameters) & noexcept(false);

			/*
				@brief Appends a series of parameters to a previously added function.

				@param *target       The name of the function to append the parameters to.
				@param *parameters   The parameters to append to the function.

				@returns True/False denoting whether the parameters were appended
					successfully.
			*/
			template <class... _Fp,
				std::enable_if_t<std::conjunction_v<std::is_same<_Fp, FunctionParameter>...>, int> = 0>
			[[nodiscard]] bool AppendParameters(std::string_view target, _Fp... parameters) & noexcept(false) {
				return AppendParameters(target, { parameters... });
			}

			/*
				@brief Returns the JSON object of the internal conversation.
			*/
			LIBOAI_EXPORT const nlohmann::json& GetJSON() const & noexcept;
			
		private:
			using index = std::size_t;
			[[nodiscard]] index GetFunctionIndex(std::string_view function_name) const & noexcept(false);

			nlohmann::json _functions;
	};
		
	/*
		@brief Class containing, and used for keeping track of, the chat history.
			An object of this class should be created, set with system and user data,
			and provided to ChatCompletion::create (system is optional).

			The general usage of this class is as follows:
				1. Create a ChatCompletion::Conversation object.
				2. Set the user data, which is the user's input - such as
				   a question or a command as well as optionally set the
				   system data to guide how the assistant responds.
				3. Provide the ChatCompletion::Conversation object to
				   ChatCompletion::create.
				4. Update the ChatCompletion::Conversation object with
				   the response from the API - either the object or the
				   response content can be used to update the object.
				5. Retrieve the assistant's response from the
				   ChatCompletion::Conversation object.
				6. Repeat steps 2, 3, 4 and 5 until the conversation is
				   complete.

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
			Conversation(const Conversation& other);
			Conversation(Conversation&& old) noexcept;
			Conversation(std::string_view system_data);
			Conversation(std::string_view system_data, std::string_view user_data);
			Conversation(std::string_view system_data, std::initializer_list<std::string_view> user_data);
			Conversation(std::initializer_list<std::string_view> user_data);
			explicit Conversation(const std::vector<std::string>& user_data);
			~Conversation() = default;

			Conversation& operator=(const Conversation& other);
			Conversation& operator=(Conversation&& old) noexcept;

			friend std::ostream& operator<<(std::ostream& os, const Conversation& conv);


			/*
					@brief Changes the content of the first system message
						in the conversation. This method updates the content
						of the first system message in the conversation, if
						it exists and is of type "system". If the first message
						is not a system message or the conversation is empty,
						the method will return false.

					@param new_data A string_view containing the new content
					                for the system message. Must be non-empty.

					@returns True/False denoting whether the first system
					         message was changed successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool ChangeFirstSystemMessage(std::string_view new_data) & noexcept(false);

			/*
				@brief Sets the system data for the conversation.
					This method sets the system data for the conversation.
					The system data is the data that helps set the behavior
					of the assistant so it knows how to respond.

				@param *data      The system data to set.

				@returns True/False denoting whether the system data was set
					successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetSystemData(std::string_view data) & noexcept(false);

			/*
				@brief Removes the set system data from the top of the conversation.
					The system data must be the first data set, if used,
					in order to be removed. If the system data is not
					the first data set, this method will return false.

				@returns True/False denoting whether the system data was
					removed successfully.
			*/
			[[nodiscard]]
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

				@returns True/False denoting whether the user input was
					added successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AddUserData(std::string_view data) & noexcept(false);

			/*
				@brief Adds user input to the conversation.
					This method adds user input to the conversation.
					The user input is the user's input - such as a question
					or a command.

					If using a system prompt, the user input should be
					provided after the system prompt is set - i.e. after
					SetSystemData() is called.

				@param *data        The user input to add.
				@param *name        The name of the author of this message.
					                name is required if role is function, and
									it should be the name of the function whose
									response is in the content.

				@returns True/False denoting whether the user input was
					added successfully.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AddUserData(
				std::string_view data,
				std::string_view name
			) & noexcept(false);

			/*
				@brief Removes the last added user data.

				@returns True/False denoting whether the user data was removed.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopUserData() & noexcept(false);

			/*
				@brief Gets the last response from the assistant.
					This method gets the last response from the assistant.
					The response is the assistant's response to the user's
					input.
			*/
			LIBOAI_EXPORT std::string GetLastResponse() const & noexcept;

			/*
				@brief Returns whether the most recent response, following
					a call to Update, contains a function_call or not.

					It is important to note that, when making use of functions,
					a developer must call this method to determine whether
					the response contains a function call or if it contains a 
					regular response. If the response contains a function call,
					

				@returns True/False denoting whether the most recent response
				contains a function_call or not.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool LastResponseIsFunctionCall() const & noexcept;

			/*
				@brief Returns the name of the function_call in the most recent
					response. This should only be called if LastResponseIsFunctionCall()
					returns true.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT std::string GetLastFunctionCallName() const & noexcept(false);
			
			/*
				@brief Returns the arguments of the function_call in the most
					recent response in their raw JSON form. This should only
					be called if LastResponseIsFunctionCall() returns true.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT std::string GetLastFunctionCallArguments() const & noexcept(false);			

			/*
				@brief Removes the last assistant response.
				
				@returns True/False denoting whether the last response was removed.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool PopLastResponse() & noexcept(false);

			/*
				@brief Updates the conversation given JSON data.
					This method updates the conversation given JSON data.
					The JSON data should be the JSON 'messages' data returned
					from the OpenAI API.

					This method should only be used if AppendStreamData was NOT
					used immediately before it.

					For instance, if we made a call to create*(), and provided a
					callback function to stream and, within this callback, we used
					AppendStreamData to update the conversation per message, we
					would NOT want to use this method. In this scenario, the
					AppendStreamData method would have already updated the
					conversation, so this method would be a bad idea to call
					afterwards.

				@param *history      The JSON data to update the conversation with.
										 This should be the 'messages' array of data returned
										 from a call to ChatCompletion::create.

				@returns True/False denoting whether the conversation was updated.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool Update(std::string_view history) & noexcept(false);

			/*
				@brief Updates the conversation given a Response object.
					This method updates the conversation given a Response object.
					This method should only be used if AppendStreamData was NOT
					used immediately before it.

					For instance, if we made a call to create*(), and provided a
					callback function to stream and, within this callback, we used
					AppendStreamData to update the conversation per message, we
					would NOT want to use this method. In this scenario, the
					AppendStreamData method would have already updated the
					conversation, so this method would be a bad idea to call
					afterwards.

					@param *response     The Response to update the conversation with.
										 This should be the Response returned from a call
										 to ChatCompletion::create.

					@returns True/False denoting whether the update was successful.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool Update(const Response& response) & noexcept(false);

			/*
				@brief Exports the entire conversation to a JSON string.
					This method exports the conversation to a JSON string.
					The JSON string can be used to save the conversation
					to a file. The exported string contains both the
					conversation and included functions, if any.

				@returns The JSON string representing the conversation.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT std::string Export() const & noexcept(false);

			/*
				@brief Imports a conversation from a JSON string.
					This method imports a conversation from a JSON string.
					The JSON string should be the JSON string returned
					from a call to Export().

				@param *json      The JSON string to import the conversation from.

				@returns True/False denoting whether the conversation was imported.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool Import(std::string_view json) & noexcept(false);

			/*
				@brief Appends stream data (SSEs) from streamed methods.
					This method updates the conversation given a token from a
					streamed method. This method should be used when using
					streamed methods such as ChatCompletion::create or 
					create_async with a callback supplied. This function should
					be called from within the stream's callback function
					receiving the SSEs.

					@param *token    Streamed token (data) to update the conversation with.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AppendStreamData(std::string data) & noexcept(false);


			/*
				@brief Appends stream data (SSEs) from streamed methods.
					This method updates the conversation given a token from a
					streamed method. This method should be used when using
					streamed methods such as ChatCompletion::create or 
					create_async with a callback supplied. This function should
					be called from within the stream's callback function
					receiving the SSEs.

					@param *token    Streamed token (data) to update the conversation with.
					@param *delta    output parameter. The delta to append to the conversation.
					@param *completed output parameter. Whether the stream is completed.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool AppendStreamData(std::string data, std::string& delta, bool& completed) & noexcept(false);

			/*
				@brief Sets the functions to be used for the conversation.
					This method sets the functions to be used for the conversation.

					@param *functions      The functions to set.
					
					@returns True/False denoting whether the functions were set.
			*/
			[[nodiscard]]
			LIBOAI_EXPORT bool SetFunctions(Functions functions) & noexcept(false);

			/*
				@brief Pops any previously set functions.

					@param *functions      The functions to set.
			*/
			LIBOAI_EXPORT void PopFunctions() & noexcept(false);

			/*
				@brief Returns the raw JSON dump of the internal conversation object
					in string format.
			*/
			LIBOAI_EXPORT std::string GetRawConversation() const & noexcept;

			/*
				@brief Returns the JSON object of the internal conversation.
			*/
			LIBOAI_EXPORT const nlohmann::json& GetJSON() const & noexcept;

			/*
				@brief Returns the raw JSON dump of the internal functions object
					in string format - if one exists.
			*/
			LIBOAI_EXPORT std::string GetRawFunctions() const & noexcept;

			/*
				@brief Returns the JSON object of the set functions.
			*/
			LIBOAI_EXPORT const nlohmann::json& GetFunctionsJSON() const & noexcept;
			
			/*
				@brief Returns whether the conversation has functions or not. this function call from ChatComplete
			*/
			[[nodiscard]] constexpr bool HasFunctions() const & noexcept { return this->_functions ? true : false; }

			/**
			 * @brief Sets the maximum history size for the conversation.
			 * 
			 * @param size The maximum number of messages allowed in the conversation history.
			 *             Older messages will be removed when the limit is exceeded.
			 */
			void SetMaxHistorySize(size_t size) noexcept { _max_history_size = size; }

		private:
			friend class ChatCompletion; friend class Azure;
			[[nodiscard]] std::vector<std::string> SplitStreamedData(std::string data) const noexcept(false);
			void RemoveStrings(std::string& s, std::string_view p) const noexcept(false);
			void EraseExtra();
			/*
				@brief split full stream data that read from remote server.
				@returns vector of string that contains the split data that will contains the last termination string(data: "DONE").
			*/
			[[nodiscard]] std::vector<std::string> SplitFullStreamedData(std::string data) const noexcept(false);
			bool ParseStreamData(std::string data, std::string& delta, bool& completed);

			nlohmann::json _conversation;
			std::optional<nlohmann::json> _functions = std::nullopt;
			bool _last_resp_is_fc = false;
			std::string _last_incomplete_buffer;
			size_t _max_history_size = std::numeric_limits<size_t>::max();
	};

	class ChatCompletion final : private Network {
		public:
			ChatCompletion(const std::string &root): Network(root) {}
			NON_COPYABLE(ChatCompletion)
			NON_MOVABLE(ChatCompletion)
			~ChatCompletion() = default;

			using ChatStreamCallback = std::function<bool(std::string, intptr_t, Conversation&)>;

			/*
				@brief Creates a completion for the chat message.

				@param *model            ID of the model to use. Currently,
				                         only gpt-3.5-turbo and gpt-3.5-turbo-0301 
								 	     are supported.
				@param *conversation     A Conversation object containing the
									     conversation data.
				@param function_call     Controls how the model responds to function calls. "none"
										 means the model does not call a function, and responds to
										 the end-user. "auto" means the model can pick between an
										 end-user or calling a function.
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
									     vents as they become available, with the stream terminated
									     by a data: [DONE] message.
				@param stop               to 4 sequences where the API will stop generating further
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
				Conversation& conversation,
				std::optional<std::string> function_call = std::nullopt,
				std::optional<float> temperature = std::nullopt,
				std::optional<float> top_p = std::nullopt,
				std::optional<uint16_t> n = std::nullopt,
				std::optional<ChatStreamCallback> stream = std::nullopt,
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
				@param function_call     Controls how the model responds to function calls. "none"
				                         means the model does not call a function, and responds to
										 the end-user. "auto" means the model can pick between an
										 end-user or calling a function.
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
										 vents as they become available, with the stream terminated
										 by a data: [DONE] message.
				@param stop               to 4 sequences where the API will stop generating further
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
				Conversation& conversation,
				std::optional<std::string> function_call = std::nullopt,
				std::optional<float> temperature = std::nullopt,
				std::optional<float> top_p = std::nullopt,
				std::optional<uint16_t> n = std::nullopt,
				std::optional<ChatStreamCallback> stream = std::nullopt,
				std::optional<std::vector<std::string>> stop = std::nullopt,
				std::optional<uint16_t> max_tokens = std::nullopt,
				std::optional<float> presence_penalty = std::nullopt,
				std::optional<float> frequency_penalty = std::nullopt,
				std::optional<std::unordered_map<std::string, int8_t>> logit_bias = std::nullopt,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);
			
		private:
			Authorization& auth_ = Authorization::Authorizer();
			using StrippedStreamCallback = std::function<bool(std::string, intptr_t)>;
	};
}
