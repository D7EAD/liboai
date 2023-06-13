#include "../include/components/fine_tunes.h"

liboai::Response liboai::FineTunes::create(const std::string& training_file, std::optional<std::string> validation_file, std::optional<std::string> model_id, std::optional<uint8_t> n_epochs, std::optional<uint16_t> batch_size, std::optional<float> learning_rate_multiplier, std::optional<float> prompt_loss_weight, std::optional<bool> compute_classification_metrics, std::optional<uint16_t> classification_n_classes, std::optional<std::string> classification_positive_class, std::optional<std::vector<float>> classification_betas, std::optional<std::string> suffix) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("training_file", training_file);
	jcon.push_back("validation_file", std::move(validation_file));
	jcon.push_back("model_id", std::move(model_id));
	jcon.push_back("n_epochs", std::move(n_epochs));
	jcon.push_back("batch_size", std::move(batch_size));
	jcon.push_back("learning_rate_multiplier", std::move(learning_rate_multiplier));
	jcon.push_back("prompt_loss_weight", std::move(prompt_loss_weight));
	jcon.push_back("compute_classification_metrics", std::move(compute_classification_metrics));
	jcon.push_back("classification_n_classes", std::move(classification_n_classes));
	jcon.push_back("classification_positive_class", std::move(classification_positive_class));
	jcon.push_back("classification_betas", std::move(classification_betas));
	jcon.push_back("suffix", std::move(suffix));

	Response res;
	res = this->Request(
		Method::HTTP_POST, this->openai_root_, "/fine-tunes", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		netimpl::components::Body {
			jcon.dump()
		},
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::FineTunes::create_async(const std::string& training_file, std::optional<std::string> validation_file, std::optional<std::string> model_id, std::optional<uint8_t> n_epochs, std::optional<uint16_t> batch_size, std::optional<float> learning_rate_multiplier, std::optional<float> prompt_loss_weight, std::optional<bool> compute_classification_metrics, std::optional<uint16_t> classification_n_classes, std::optional<std::string> classification_positive_class, std::optional<std::vector<float>> classification_betas, std::optional<std::string> suffix) const & noexcept(false) {
	liboai::JsonConstructor jcon;
	jcon.push_back("training_file", training_file);
	jcon.push_back("validation_file", std::move(validation_file));
	jcon.push_back("model_id", std::move(model_id));
	jcon.push_back("n_epochs", std::move(n_epochs));
	jcon.push_back("batch_size", std::move(batch_size));
	jcon.push_back("learning_rate_multiplier", std::move(learning_rate_multiplier));
	jcon.push_back("prompt_loss_weight", std::move(prompt_loss_weight));
	jcon.push_back("compute_classification_metrics", std::move(compute_classification_metrics));
	jcon.push_back("classification_n_classes", std::move(classification_n_classes));
	jcon.push_back("classification_positive_class", std::move(classification_positive_class));
	jcon.push_back("classification_betas", std::move(classification_betas));
	jcon.push_back("suffix", std::move(suffix));

	auto _fn = [this](liboai::JsonConstructor&& jcon) -> liboai::Response {
		return this->Request(
			Method::HTTP_POST, this->openai_root_, "/fine-tunes", "application/json",
			this->auth_.GetAuthorizationHeaders(),
			netimpl::components::Body {
				jcon.dump()
			},
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};

	return std::async(std::launch::async, _fn, std::move(jcon));
}

liboai::Response liboai::FineTunes::list() const& {
	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/fine-tunes", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::FineTunes::list_async() const & noexcept(false) {
	auto _fn = [this]() -> liboai::Response {
		return this->Request(
			Method::HTTP_GET, this->openai_root_, "/fine-tunes", "application/json",
			this->auth_.GetAuthorizationHeaders(),
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};

	return std::async(std::launch::async, _fn);
}

liboai::Response liboai::FineTunes::retrieve(const std::string& fine_tune_id) const& {
	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/fine-tunes/" + fine_tune_id, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::FineTunes::retrieve_async(const std::string& fine_tune_id) const & noexcept(false) {
	auto _fn = [this](const std::string& fine_tune_id) -> liboai::Response {
		return this->Request(
			Method::HTTP_GET, this->openai_root_, "/fine-tunes/" + fine_tune_id, "application/json",
			this->auth_.GetAuthorizationHeaders(),
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};

	return std::async(std::launch::async, _fn, fine_tune_id);
}

liboai::Response liboai::FineTunes::cancel(const std::string& fine_tune_id) const& {	
	Response res;
	res = this->Request(
		Method::HTTP_POST, this->openai_root_, "/fine-tunes/" + fine_tune_id + "/cancel", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::FineTunes::cancel_async(const std::string& fine_tune_id) const & noexcept(false) {
	auto _fn = [this](const std::string& fine_tune_id) -> liboai::Response {
		return this->Request(
			Method::HTTP_POST, this->openai_root_, "/fine-tunes/" + fine_tune_id + "/cancel", "application/json",
			this->auth_.GetAuthorizationHeaders(),
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};
	
	return std::async(std::launch::async, _fn, fine_tune_id);
}

liboai::Response liboai::FineTunes::list_events(const std::string& fine_tune_id, std::optional<std::function<bool(std::string, intptr_t)>> stream) const & noexcept(false) {
	netimpl::components::Parameters params;
	stream ? params.Add({"stream", "true"}) : void();

	Response res;
	res = this->Request(
		Method::HTTP_GET, this->openai_root_, "/fine-tunes/" + fine_tune_id + "/events", "application/json",
		this->auth_.GetAuthorizationHeaders(),
		std::move(params),
		stream ? netimpl::components::WriteCallback{std::move(stream.value())} : netimpl::components::WriteCallback{},
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::FineTunes::list_events_async(const std::string& fine_tune_id, std::optional<std::function<bool(std::string, intptr_t)>> stream) const & noexcept(false) {
	netimpl::components::Parameters params;
	stream ? params.Add({ "stream", "true"}) : void();
	
	auto _fn = [this](
		netimpl::components::Parameters&& params,
		const std::string& fine_tune_id,
		std::optional<std::function<bool(std::string, intptr_t)>>&& stream
	) -> liboai::Response {
		return this->Request(
			Method::HTTP_GET, this->openai_root_, "/fine-tunes/" + fine_tune_id + "/events", "application/json",
			this->auth_.GetAuthorizationHeaders(),
			std::move(params),
			stream ? netimpl::components::WriteCallback{std::move(stream.value())} : netimpl::components::WriteCallback{},
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};

	return std::async(std::launch::async, _fn, std::move(params), fine_tune_id, std::move(stream));
}

liboai::Response liboai::FineTunes::remove(const std::string& model) const& noexcept(false) {
	Response res;
	res = this->Request(
		Method::HTTP_DELETE, this->openai_root_, "/models/" + model, "application/json",
		this->auth_.GetAuthorizationHeaders(),
		this->auth_.GetProxies(),
		this->auth_.GetProxyAuth(),
		this->auth_.GetMaxTimeout()
	);

	return res;
}

liboai::FutureResponse liboai::FineTunes::remove_async(const std::string& model) const & noexcept(false) {
	auto _fn = [this](const std::string& model) -> liboai::Response {
		return this->Request(
			Method::HTTP_DELETE, this->openai_root_, "/models/" + model, "application/json",
			this->auth_.GetAuthorizationHeaders(),
			this->auth_.GetProxies(),
			this->auth_.GetProxyAuth(),
			this->auth_.GetMaxTimeout()
		);
	};
	
	return std::async(std::launch::async, _fn, model);
}