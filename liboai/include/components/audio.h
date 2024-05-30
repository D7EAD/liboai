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
	class Audio final : private Network {
		public:
			Audio() = default;
			NON_COPYABLE(Audio)
			NON_MOVABLE(Audio)
			~Audio() = default;

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

				@returns A liboai::Response object containing the
					data in JSON format.
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
				@brief Asynchronously transcribes audio into the input language.

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
				
				@returns A liboai::Response future containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse transcribe_async(
				const std::filesystem::path& file,
				const std::string& model,
				std::optional<std::string> prompt = std::nullopt,
				std::optional<std::string> response_format = std::nullopt,
				std::optional<float> temperature = std::nullopt,
				std::optional<std::string> language = std::nullopt
			) const& noexcept(false);

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

				@returns A liboai::Response object containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response translate(
				const std::filesystem::path& file,
				const std::string& model,
				std::optional<std::string> prompt = std::nullopt,
				std::optional<std::string> response_format = std::nullopt,
				std::optional<float> temperature = std::nullopt
			) const & noexcept(false);

			/*
				@brief Asynchronously translates audio into English.

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

				@returns A liboai::Response future containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse translate_async(
				const std::filesystem::path& file,
				const std::string& model,
				std::optional<std::string> prompt = std::nullopt,
				std::optional<std::string> response_format = std::nullopt,
				std::optional<float> temperature = std::nullopt
			) const& noexcept(false);

			/*
				@brief Turn text into lifelike spoken audio.

				@param *model          The model to use for translation.
									   Only 'tts-1' and 'tts-1-hd' are currently available.
				@param *voice          The voice to use when generating the audio.
									   Supported voices are alloy, echo, fable, onyx, nova, and shimmer.
				@param *input          The text to generate audio for.
									   The maximum length is 4096 characters.
				@param response_format The format to audio in.
									   Supported formats are mp3, opus, aac, flac, wav, and pcm.
				@param speed           The speed of the generated audio.
									   Select a value from 0.25 to 4.0. 1.0 is the default.

				@returns A liboai::Response object containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response speech(
				const std::string& model,
				const std::string& voice,
				const std::string& input,
				std::optional<std::string> response_format = std::nullopt,
				std::optional<float> speed = std::nullopt
			) const& noexcept(false);

/*
				@brief Asynchronously turn text into lifelike spoken audio.

				@param *model          The model to use for translation.
									   Only 'tts-1' and 'tts-1-hd' are currently available.
				@param *voice          The voice to use when generating the audio.
									   Supported voices are alloy, echo, fable, onyx, nova, and shimmer.
				@param *input          The text to generate audio for.
									   The maximum length is 4096 characters.
				@param response_format The format to audio in.
									   Supported formats are mp3, opus, aac, flac, wav, and pcm.
				@param speed           The speed of the generated audio.
									   Select a value from 0.25 to 4.0. 1.0 is the default.

				@returns A liboai::Response object containing the
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse speech_async(
				const std::string& model,
				const std::string& voice,
				const std::string& input,
				std::optional<std::string> response_format = std::nullopt,
				std::optional<float> speed = std::nullopt
			) const& noexcept(false);

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}