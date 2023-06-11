#pragma once

/*
	Copyright (c) 2012-2022 Johnny (pseud. Dread) and others

	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
	LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
	OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	liboai.h : main library header.
		This header file provides an interface to all component classes
		in the library. It is the only header file that needs to be
		included in order to use the library.
*/

#include "components/audio.h"
#include "components/azure.h"
#include "components/chat.h"
#include "components/completions.h"
#include "components/edits.h"
#include "components/embeddings.h"
#include "components/files.h"
#include "components/fine_tunes.h"
#include "components/images.h"
#include "components/models.h"
#include "components/moderations.h"

namespace liboai {
	class OpenAI final {
		public:
			OpenAI() = default;
			OpenAI(OpenAI const&) = delete;
			OpenAI(OpenAI&&) = delete;
			void operator=(OpenAI const&) = delete;
			void operator=(OpenAI&&) = delete;
			
		public: // component interfaces
			/*
				@brief A pointer to the Audio component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Audio> Audio = std::make_unique<liboai::Audio>();

			/*
				@brief A pointer to the Azure component class that
					provides access to its API endpoints.
			*/
			std::unique_ptr<liboai::Azure> Azure = std::make_unique<liboai::Azure>();
			
			/*
				@brief A pointer to the Chat component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::ChatCompletion> ChatCompletion = std::make_unique<liboai::ChatCompletion>();

			/*
				@brief A pointer to the Completions component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Completions> Completion = std::make_unique<liboai::Completions>();

			/*
				@brief A pointer to the Edits component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Edits> Edit = std::make_unique<liboai::Edits>();

			/*
				@brief A pointer to the Embeddings component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Embeddings> Embedding = std::make_unique<liboai::Embeddings>();

			/*
				@brief A pointer to the Files component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Files> File = std::make_unique<liboai::Files>();
			
			/*
				@brief A pointer to the FineTunes component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::FineTunes> FineTune = std::make_unique<liboai::FineTunes>();

			/*
				@brief A pointer to the Images component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Images> Image = std::make_unique<liboai::Images>();

			/*
				@brief A pointer to the Models component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Models> Model = std::make_unique<liboai::Models>();

			/*
				@brief A pointer to the Moderations component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Moderations> Moderation = std::make_unique<liboai::Moderations>();
		
		public:			
			/*
				@brief Convenience reference to the Authorization class
					singleton used to set authorization information.
			*/
			Authorization& auth = Authorization::Authorizer();
	};
}