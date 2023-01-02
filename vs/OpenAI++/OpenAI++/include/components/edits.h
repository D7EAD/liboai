#pragma once

/*
	edits.h : Edits component class for OpenAI.
		This class contains all the methods for the Edits component
		of the OpenAI API. This class provides access to 'Edits'
		endpoints on the OpenAI API and should be accessed via the
		liboai.h header file through an instantiated liboai::OpenAI
		object after setting necessary authentication information
		through the liboai::Authorization::Authorizer() singleton
		object.
*/

#include "../core/authorization.h"
#include "../core/response.h"

namespace liboai {
	class Edits final : private Network {
		public:
			Edits() = default;
			~Edits() = default;
			Edits(const Edits&) = delete;
			Edits(Edits&&) = delete;
			
			Edits& operator=(const Edits&) = delete;
			Edits& operator=(Edits&&) = delete;

			/*
				@brief Creates a new edit for the provided input,
					instruction, and parameters

				@param *model       The model to use for the edit.
				@param input        The input text to edit.
				@param instruction  The instruction to edit the input.
				@param n            The number of edits to return.
				@param temperature  Higher values means the model will take more
										risks. Try 0.9 for more creative applications,
										and 0 (argmax sampling) for ones with a
										well-defined answer.
				@param top_p        An alternative to sampling with temperature,
										 called nucleus sampling, where the model
										 considers the results of the tokens with
										 top_p probability mass. So 0.1 means only
										 the tokens comprising the top 10% probability
										 mass are considered.

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT Response create(
				const std::string& model_id,
				const std::optional<std::string>& input = std::nullopt,
				const std::optional<std::string>& instruction = std::nullopt,
				const std::optional<uint16_t>& n = std::nullopt,
				const std::optional<float>& temperature = std::nullopt,
				const std::optional<float>& top_p = std::nullopt
			) const &;

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}