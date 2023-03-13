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
			E_BADREQUEST,
			E_APIERROR,
			E_RATELIMIT,
			E_CONNECTIONERROR,
			E_FILEERROR
		};

		constexpr const char* _etype_strs_[6] = {
			"E_FAILURETOPARSE:0x00",
			"E_BADREQUEST:0x01",
			"E_APIERROR:0x02",
			"E_RATELIMIT:0x03",
			"E_CONNECTIONERROR:0x04",
			"E_FILEERROR:0x05"
		};

		class OpenAIException : public std::exception {
			public:
				OpenAIException() = default;
				OpenAIException(const OpenAIException& rhs) noexcept
					: data_(rhs.data_), error_type_(rhs.error_type_), locale_(rhs.locale_) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }
				OpenAIException(OpenAIException&& rhs) noexcept
					: data_(std::move(rhs.data_)), error_type_(rhs.error_type_), locale_(std::move(rhs.locale_)) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }
				OpenAIException(std::string_view data, EType error_type, std::string_view locale) noexcept
					: data_(data), error_type_(error_type), locale_(locale) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }

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
				OpenAIRateLimited(std::string_view data, EType error_type, std::string_view locale) noexcept
					: data_(data), error_type_(error_type), locale_(locale) { this->fmt_str_ = (this->locale_ + ": " + this->data_ + " (" + this->GetETypeString(this->error_type_) + ")"); }

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