#pragma once

/*
	audio.h : Audio component class for OpenAI.
		This class contains all the methods for the Audio component
		of the OpenAI API. This class provides access to 'Audio'
		endpoints on the OpenAI API and should be accessed via the
		liboai.h header file through an instantiated liboai::OpenAI
		object after setting necessary authentication information
		through the liboai::Authorization::Authorizer() singleton
		object.
*/

#include "../core/authorization.h"
#include "../core/response.h"

namespace liboai {
	class Audio : private Network {
		public:
			Audio() = default;
			~Audio() = default;
			Audio(const Audio&) = delete;
			Audio(Audio&&) = delete;

			Audio& operator=(const Audio&) = delete;
			Audio& operator=(Audio&&) = delete;

			/*
				@brief Transcribes audio into the input language.

				@param *file           The audio file to transcribe.
				@param *model          The model to use for transcription.
				                       Only 'whisper-1' is currently available.
				@param prompt          An optional text to guide the model's style
				                       or continue a previous audio segment. The
								       prompt should match the audio language.
				@param response_format The format of the transcript output.
				@param temperature     The sampling temperature, between 0 and 1.
				                       Higher values like 0.8 will make the output
									   more random, while lower values like 0.2
									   will make it more focused and deterministic.
									   If set to 0, the model will use log probability
									   to automatically increase the temperature until
									   certain thresholds are hit.
				@param language        The language of the audio file.
			*/
			LIBOAI_EXPORT liboai::Response transcribe(
				const std::filesystem::path& file,
				const std::string& model,
				std::optional<std::string> prompt = std::nullopt,
				std::optional<std::string> response_format = std::nullopt,
				std::optional<float> temperature = std::nullopt,
				std::optional<std::string> language = std::nullopt
			) const & noexcept(false);

			/*
				@brief Translates audio into English.

				@param *file           The audio file to translate.
				@param *model          The model to use for translation.
									   Only 'whisper-1' is currently available.
				@param prompt          An optional text to guide the model's style
				                       or continue a previous audio segment.
				@param response_format The format of the transcript output.
				@param temperature     The sampling temperature, between 0 and 1.
									   Higher values like 0.8 will make the output
									   more random, while lower values like 0.2
									   will make it more focused and deterministic.
									   If set to 0, the model will use log probability
									   to automatically increase the temperature until
									   certain thresholds are hit.
			*/
			LIBOAI_EXPORT liboai::Response translate(
				const std::filesystem::path& file,
				const std::string& model,
				std::optional<std::string> prompt = std::nullopt,
				std::optional<std::string> response_format = std::nullopt,
				std::optional<float> temperature = std::nullopt
			) const & noexcept(false);

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}