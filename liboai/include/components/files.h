#pragma once

/*
	files.h : Files component class for OpenAI.
		This class contains all the methods for the Files component
		of the OpenAI API. This class provides access to 'Files'
		endpoints on the OpenAI API and should be accessed via the
		liboai.h header file through an instantiated liboai::OpenAI
		object after setting necessary authentication information
		through the liboai::Authorization::Authorizer() singleton
		object.
*/

#include "../core/authorization.h"
#include "../core/response.h"

namespace liboai {
	class Files final : private Network {
		public:
			Files() = default;
			~Files() = default;
			Files(const Files&) = delete;
			Files(Files&&) = delete;			
			
			Files& operator=(const Files&) = delete;
			Files& operator=(Files&&) = delete;

			/*
				@brief Returns a list of files that belong to the user's organization.

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response list() const & noexcept(false);

			/*
				@brief Asynchronously returns a list of files that belong to the
					user's organization.

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse list_async() const & noexcept(false);

			/*
				@brief Upload a file that contains document(s) to be
					used across various endpoints/features. Currently,
					the size of all the files uploaded by one organization
					can be up to 1 GB.

				@param file     The JSON Lines file to be uploaded (path).
				@param purpose  The intended purpose of the uploaded documents.

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response create(
				const std::filesystem::path& file,
				const std::string& purpose
			) const & noexcept(false);

			/*
				@brief Asynchronously upload a file that contains document(s)
					to be used across various endpoints/features. Currently,
					the size of all the files uploaded by one organization
					can be up to 1 GB.

				@param file     The JSON Lines file to be uploaded (path).
				@param purpose  The intended purpose of the uploaded documents.

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse create_async(
				const std::filesystem::path& file,
				const std::string& purpose
			) const & noexcept(false);

			/*
				@brief Delete [remove] a file.

				@param *file_id   The ID of the file to use for this request

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/			
			LIBOAI_EXPORT liboai::Response remove(
				const std::string& file_id
			) const & noexcept(false);

			/*
				@brief Asynchronously delete [remove] a file.

				@param *file_id   The ID of the file to use for this request

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/			
			LIBOAI_EXPORT liboai::FutureResponse remove_async(
				const std::string& file_id
			) const & noexcept(false);

			/*
				@brief Returns information about a specific file.

				@param *file_id   The ID of the file to use for this request

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response retrieve(
				const std::string& file_id
			) const & noexcept(false);

			/*
				@brief Asynchronously returns information about a specific file.

				@param *file_id   The ID of the file to use for this request

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse retrieve_async(
				const std::string& file_id
			) const & noexcept(false);

			/*
				@brief Downloads the contents of the specified file
					to the specified path.

				@param *file_id    The ID of the file to use for this request
				@param *save_to    The path to save the file to

				@return a boolean value indicating whether the file was
					successfully downloaded or not.
			*/
			LIBOAI_EXPORT bool download(
				const std::string& file_id,
				const std::string& save_to
			) const & noexcept(false);

			/*
				@brief Asynchronously downloads the contents of the specified file
					to the specified path.

				@param *file_id    The ID of the file to use for this request
				@param *save_to    The path to save the file to

				@return a boolean future indicating whether the file was
					successfully downloaded or not.
			*/
			LIBOAI_EXPORT std::future<bool> download_async(
				const std::string& file_id,
				const std::string& save_to
			) const & noexcept(false);
			
		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}