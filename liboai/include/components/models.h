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
			Models(const std::string &root): Network(root) {}
			NON_COPYABLE(Models)
			NON_MOVABLE(Models)
			~Models() = default;

			/*
				@brief List all available models.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response list() const & noexcept(false);

			/*
				@brief Asynchronously list all available models.

				@returns A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse list_async() const & noexcept(false);
			
			/*
				@brief Retrieve a specific model's information.

				#param *model   The model to retrieve information for.

				@returns A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response retrieve(
				const std::string& model
			) const & noexcept(false);

			/*
				@brief Asynchronously retrieve a specific model's information.

				@param *model   The model to retrieve information for.

				@returns A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse retrieve_async(
				const std::string& model
			) const & noexcept(false);

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}