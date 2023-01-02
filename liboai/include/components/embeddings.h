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
			~Embeddings() = default;
			Embeddings(const Embeddings&) = delete;
			Embeddings(Embeddings&&) = delete;
			
			Embeddings& operator=(const Embeddings&) = delete;
			Embeddings& operator=(Embeddings&&) = delete;

			/*
				@brief Creates an embedding vector representing the input text.

				@param *model       The model to use for the edit.
				@param input        The input text to edit.
				@param user         A unique identifier representing your end-user

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT Response create(
				const std::string& model_id,
				const std::optional<std::string>& input = std::nullopt,
				const std::optional<std::string>& user = std::nullopt
			) const &;

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}