#pragma once

/*
	embeddings.h : Embeddings component class for OpenAI.
		This class contains all the methods for the Embeddings component
		of the OpenAI API. This class provides access to 'Embeddings'
		endpoints on the OpenAI API and should be accessed via the
		liboai.h header file through an instantiated liboai::OpenAI
		object after setting necessary authentication information
		through the liboai::Authorization::Authorizer() singleton
		object.
*/

#include "../core/authorization.h"
#include "../core/response.h"

namespace liboai {
	class Embeddings final : private Network {
		public:
			Embeddings() = default;
			NON_COPYABLE(Embeddings)
			NON_MOVABLE(Embeddings)
			~Embeddings() = default;

			/*
				@brief Creates an embedding vector representing the input text.

				@param *model       The model to use for the edit.
				@param input        The input text to edit.
				@param user         A unique identifier representing your end-user

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response create(
				const std::string& model_id,
				std::optional<std::string> input = std::nullopt,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);

			/*
				@brief Asynchronously creates an embedding vector representing the input text.

				@param *model       The model to use for the edit.
				@param input        The input text to edit.
				@param user         A unique identifier representing your end-user

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse create_async(
				const std::string& model_id,
				std::optional<std::string> input = std::nullopt,
				std::optional<std::string> user = std::nullopt
			) const & noexcept(false);

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}