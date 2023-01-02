#pragma once

/*
	models.h : Models component class for OpenAI.
		This class contains all the methods for the Models component
		of the OpenAI API. This class provides access to 'Models'
		endpoints on the OpenAI API and should be accessed via the
		liboai.h header file through an instantiated liboai::OpenAI
		object after setting necessary authentication information
		through the liboai::Authorization::Authorizer() singleton
		object.
*/

#include "../core/authorization.h"
#include "../core/response.h"

namespace liboai {
	class Models final : private Network {
		public:
			Models() = default;
			~Models() = default;
			Models(const Models&) = delete;
			Models(Models&&) = delete;

			Models& operator=(const Models&) = delete;
			Models& operator=(Models&&) = delete;

			/*
				@brief List all available models.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT Response list() const &;

			/*
				@brief Retrieve a specific model's information.

				#param *model   The model to retrieve information for.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT Response retrieve(
				const std::string& model
			) const &;

			/*
				@brief Delete a fine-tuned model.

				#param *model   The model to delete.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT Response remove(
				const std::string& model
			) const &;

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}