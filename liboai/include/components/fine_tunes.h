#pragma once

/*
	fine_tunes.h : Fine-tunes component class for OpenAI.
		This class contains all the methods for the Fine-tunes component
		of the OpenAI API. This class provides access to 'Fine-tunes'
		endpoints on the OpenAI API and should be accessed via the
		liboai.h header file through an instantiated liboai::OpenAI
		object after setting necessary authentication information
		through the liboai::Authorization::Authorizer() singleton
		object.
*/

#include "../core/authorization.h"
#include "../core/response.h"

namespace liboai {
	class FineTunes final : private Network {
		public:
			FineTunes(const std::string &root): Network(root) {}
			NON_COPYABLE(FineTunes)
			NON_MOVABLE(FineTunes)
			~FineTunes() = default;
			
			using StreamCallback = std::function<bool(std::string, intptr_t)>;

			/*
				@brief Creates a job that fine-tunes a specified model from a
					given dataset.

				@param *training_file                  The ID of an uploaded file that contains
												          training data.
				@param validation_file                 The ID of an uploaded file that contains
												          validation data.
				@param model                           The name of the base model to fine-tune.
				@param n_epochs                        The number of epochs to train for.
				@param batch_size                      The batch size to use for training.
				@param learning_rate_multiplier        The learning rate multiplier to use for training.
				@param prompt_loss_weight              The prompt loss weight to use for training.
				@param compute_classification_metrics  If set, we calculate classification-specific metrics
													      such as accuracy and F-1 score using the validation
														  set at the end of every epoch.
				@param classification_n_classes        The number of classes in the classification task.
				@param classification_positive_class   The positive class in binary classification.
				@param classification_betas            If this is provided, we calculate F-beta scores at the
													      specified beta values. 
				@param suffix 						   A suffix to append to the model name.

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response create(
				const std::string& training_file,
				std::optional<std::string> validation_file = std::nullopt,
				std::optional<std::string> model_id = std::nullopt,
				std::optional<uint8_t> n_epochs = std::nullopt,
				std::optional<uint16_t> batch_size = std::nullopt,
				std::optional<float> learning_rate_multiplier = std::nullopt,
				std::optional<float> prompt_loss_weight = std::nullopt,
				std::optional<bool> compute_classification_metrics = std::nullopt,
				std::optional<uint16_t> classification_n_classes = std::nullopt,
				std::optional<std::string> classification_positive_class = std::nullopt,
				std::optional<std::vector<float>> classification_betas = std::nullopt,
				std::optional<std::string> suffix = std::nullopt
			) const & noexcept(false);
				
			/*
				@brief Asynchronously creates a job that fine-tunes a specified
					model from a given dataset.

				@param *training_file                  The ID of an uploaded file that contains
												          training data.
				@param validation_file                 The ID of an uploaded file that contains
												          validation data.
				@param model                           The name of the base model to fine-tune.
				@param n_epochs                        The number of epochs to train for.
				@param batch_size                      The batch size to use for training.
				@param learning_rate_multiplier        The learning rate multiplier to use for training.
				@param prompt_loss_weight              The prompt loss weight to use for training.
				@param compute_classification_metrics  If set, we calculate classification-specific metrics
													      such as accuracy and F-1 score using the validation
														  set at the end of every epoch.
				@param classification_n_classes        The number of classes in the classification task.
				@param classification_positive_class   The positive class in binary classification.
				@param classification_betas            If this is provided, we calculate F-beta scores at the
													      specified beta values. 
				@param suffix 						   A suffix to append to the model name.

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse create_async(
				const std::string& training_file,
				std::optional<std::string> validation_file = std::nullopt,
				std::optional<std::string> model_id = std::nullopt,
				std::optional<uint8_t> n_epochs = std::nullopt,
				std::optional<uint16_t> batch_size = std::nullopt,
				std::optional<float> learning_rate_multiplier = std::nullopt,
				std::optional<float> prompt_loss_weight = std::nullopt,
				std::optional<bool> compute_classification_metrics = std::nullopt,
				std::optional<uint16_t> classification_n_classes = std::nullopt,
				std::optional<std::string> classification_positive_class = std::nullopt,
				std::optional<std::vector<float>> classification_betas = std::nullopt,
				std::optional<std::string> suffix = std::nullopt
			) const & noexcept(false);
			
			/*
				@brief List your organization's fine-tuning jobs

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response list() const & noexcept(false);

			/*
				@brief Asynchronously list your organization's fine-tuning jobs

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse list_async() const & noexcept(false);
			
			/*
				@brief Returns information about a specific file.

				@param *fine_tune_id   The ID of the fine-tune job

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response retrieve(
				const std::string& fine_tune_id
			) const & noexcept(false);

			/*
				@brief Asynchronously returns information about a specific file.

				@param *fine_tune_id   The ID of the fine-tune job

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse retrieve_async(
				const std::string& fine_tune_id
			) const & noexcept(false);
			
			/*
				@brief Immediately cancel a fine-tune job.

				@param *fine_tune_id   The ID of the fine-tune job to cancel

				@return A liboai::Response object containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response cancel(
				const std::string& fine_tune_id
			) const & noexcept(false);

			/*
				@brief Immediately cancel a fine-tune job asynchronously.

				@param *fine_tune_id   The ID of the fine-tune job to cancel

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse cancel_async(
				const std::string& fine_tune_id
			) const & noexcept(false);

			/*
				@brief Get fine-grained status updates for a fine-tune job.

				@param *fine_tune_id   The ID of the fine-tune job to get events for.
				@param stream          Callback to stream events for the fine-tune job.
									      If no callback is supplied, this parameter is
										  disabled and the API will wait until the completion
										  is finished before returning the response.

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response list_events(
				const std::string& fine_tune_id,
				std::optional<StreamCallback> stream = std::nullopt
			) const & noexcept(false);

			/*
				@brief Asynchronously get fine-grained status updates for a fine-tune job.

				@param *fine_tune_id   The ID of the fine-tune job to get events for.
				@param stream          Callback to stream events for the fine-tune job.
									      If no callback is supplied, this parameter is
										  disabled and the API will wait until the completion
										  is finished before returning the response.

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse list_events_async(
				const std::string& fine_tune_id,
				std::optional<StreamCallback> stream = std::nullopt
			) const & noexcept(false);

			/*
				@brief Delete a fine-tuned model. You must have the Owner role in your organization.

				@param *model   The model to delete

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::Response remove(
				const std::string& model
			) const & noexcept(false);

			/*
				@brief Asynchronously deletes a fine-tuned model. You must have the Owner role in your organization.

				@param *model   The model to delete

				@return A liboai::Response future containing the image(s)
					data in JSON format.
			*/
			LIBOAI_EXPORT liboai::FutureResponse remove_async(
				const std::string& model
			) const & noexcept(false);

		private:
			Authorization& auth_ = Authorization::Authorizer();
	};
}	