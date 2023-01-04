#pragma once

/*
	exception.h : liboai exception header.
		This header file provides declarations for exception
		directives for handling exceptions thrown by liboai
		component classes.
*/

#include <iostream>
#include <exception>
#include <memory>

namespace liboai {
	namespace exception {
		enum class EType : uint8_t {
			E_FAILURETOPARSE,
			E_BADRESPONSE,
			E_APIERROR,
			E_RATELIMIT,
			E_CONNECTIONERROR
		};

		constexpr const char* _etype_strs_[5] = {
			"E_FAILURETOPARSE:0x00",
			"E_BADRESPONSE:0x01",
			"E_APIERROR:0x02",
			"E_RATELIMIT:0x03",
			"E_CONNECTIONERROR:0x04"
		};

		class OpenAIException : public std::exception {
			public:
				OpenAIException() = default;
				OpenAIException(const OpenAIException& rhs) noexcept
					: data_(rhs.data_), error_type_(rhs.error_type_), locale_(rhs.locale_) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }
				OpenAIException(OpenAIException&& rhs) noexcept
					: data_(std::move(rhs.data_)), error_type_(rhs.error_type_), locale_(std::move(rhs.locale_)) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }
				OpenAIException(const std::string& msg, const EType& type, const std::string& where) noexcept
					: data_(msg), error_type_(type), locale_(where) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }
				OpenAIException(std::string&& msg, const EType& type, std::string&& where) noexcept
					: data_(std::move(msg)), error_type_(type), locale_(std::move(where)) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }

				const char* what() const noexcept override {
					return this->fmt_str_.c_str();
				}

				constexpr const char* GetETypeString(EType type) const noexcept {
					return _etype_strs_[static_cast<uint8_t>(type)];
				}

			private:
				EType error_type_;
				std::string data_, locale_, fmt_str_;
		};

		class OpenAIRateLimited : public std::exception {
			public:
				OpenAIRateLimited() = default;
				OpenAIRateLimited(const OpenAIRateLimited& rhs) noexcept
					: data_(rhs.data_), error_type_(rhs.error_type_), locale_(rhs.locale_) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }
				OpenAIRateLimited(OpenAIRateLimited&& rhs) noexcept
					: data_(std::move(rhs.data_)), error_type_(rhs.error_type_), locale_(std::move(rhs.locale_)) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }
				OpenAIRateLimited(const std::string& msg, const EType& type, const std::string& where) noexcept
					: data_(msg), error_type_(type), locale_(where) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }
				OpenAIRateLimited(std::string&& msg, const EType& type, std::string&& where) noexcept
					: data_(std::move(msg)), error_type_(type), locale_(std::move(where)) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }

				const char* what() const noexcept override {
					return this->fmt_str_.c_str();
				}

				constexpr const char* GetETypeString(EType type) const noexcept {
					return _etype_strs_[static_cast<uint8_t>(type)];
				}

			private:
				EType error_type_;
				std::string data_, locale_, fmt_str_;
		};
	}
}