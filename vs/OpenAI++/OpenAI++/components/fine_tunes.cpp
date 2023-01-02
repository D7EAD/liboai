#include "../include/components/fine_tunes.h"

liboai::Response liboai::FineTunes::create(const std::string& training_file, const std::optional<std::string>& validation_file, const std::optional<std::string>& model_id, const std::optional<uint8_t>& n_epochs, const std::optional<uint16_t>& batch_size, const std::optional<float>& learning_rate_multiplier, const std::optional<float>& prompt_loss_weight, const std::optional<bool>& compute_classification_metrics, const std::optional<uint16_t>& classification_n_classes, const std::optional<std::string>& classification_positive_class, const std::optional<std::vector<float>>& classification_betas, const std::optional<std::string>& suffix) const& {
	nlohmann::json json_data = {
		{ "training_file", training_file }
	};
	if (validation_file.has_value()) { json_data["validation_file"] = validation_file.value(); }
	if (model_id.has_value()) { json_data["model_id"] = model_id.value(); }
	if (n_epochs.has_value()) { json_data["n_epochs"] = n_epochs.value(); }
	if (batch_size.has_value()) { json_data["batch_size"] = batch_size.value(); }
	if (learning_rate_multiplier.has_value()) { json_data["learning_rate_multiplier"] = learning_rate_multiplier.value(); }
	if (prompt_loss_weight.has_value()) { json_data["prompt_loss_weight"] = prompt_loss_weight.value(); }
	if (compute_classification_metrics.has_value()) { json_data["compute_classification_metrics"] = compute_classification_metrics.value(); }
	if (classification_n_classes.has_value()) { json_data["classification_n_classes"] = classification_n_classes.value(); }
	if (classification_positive_class.has_value()) { json_data["classification_positive_class"] = classification_positive_class.value(); }
	if (classification_betas.has_value()) { json_data["classification_betas"] = classification_betas.value(); }
	if (suffix.has_value()) { json_data["suffix"] = suffix.value(); }

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/fine-tunes", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		cpr::Body {
			json_data.dump()
		},
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::FineTunes::list() const& {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_GET, "/fine-tunes", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res)); 
}

liboai::Response liboai::FineTunes::retrieve(const std::string& fine_tune_id) const& {
	cpr::Response res;
	res = this->Request(
		Method::HTTP_GET, "/fine-tunes/" + fine_tune_id, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::FineTunes::cancel(const std::string& fine_tune_id) const& {	
	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/fine-tunes/" + fine_tune_id + "/cancel", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}

liboai::Response liboai::FineTunes::list_events(const std::string& fine_tune_id, const std::optional<std::function<bool(std::string, intptr_t)>>& stream) const& {
	cpr::Parameters params;
	stream.has_value() ? params.Add({ "stream", "true"}) : void();

	cpr::Response res;
	res = this->Request(
		Method::HTTP_POST, "/fine-tunes/" + fine_tune_id + "/events", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		std::move(params),
		stream.has_value() ? cpr::WriteCallback{stream.value()} : cpr::WriteCallback{},
		this->auth_.GetProxies()
	);

	return Response(std::move(res));
}