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
	class OpenAI {
		public:
			OpenAI(const std::string &root = "https://api.openai.com/v1"):
				Audio(std::make_unique<liboai::Audio>(root)),
				Azure(std::make_unique<liboai::Azure>(root)),
				ChatCompletion(std::make_unique<liboai::ChatCompletion>(root)),
				Completion(std::make_unique<liboai::Completions>(root)),
				Edit(std::make_unique<liboai::Edits>(root)),
				Embedding(std::make_unique<liboai::Embeddings>(root)),
				File(std::make_unique<liboai::Files>(root)),
				FineTune(std::make_unique<liboai::FineTunes>(root)),
				Image(std::make_unique<liboai::Images>(root)),
				Model(std::make_unique<liboai::Models>(root)),
				Moderation(std::make_unique<liboai::Moderations>(root))
			{}
			OpenAI(OpenAI const&) = delete;
			OpenAI(OpenAI&&) = delete;
			void operator=(OpenAI const&) = delete;
			void operator=(OpenAI&&) = delete;
			
		public: // component interfaces
			/*
				@brief A pointer to the Audio component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Audio> Audio;

			/*
				@brief A pointer to the Azure component class that
					provides access to its API endpoints.
			*/
			std::unique_ptr<liboai::Azure> Azure;
			
			/*
				@brief A pointer to the Chat component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::ChatCompletion> ChatCompletion;

			/*
				@brief A pointer to the Completions component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Completions> Completion;

			/*
				@brief A pointer to the Edits component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Edits> Edit;

			/*
				@brief A pointer to the Embeddings component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Embeddings> Embedding;

			/*
				@brief A pointer to the Files component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Files> File;
			
			/*
				@brief A pointer to the FineTunes component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::FineTunes> FineTune;

			/*
				@brief A pointer to the Images component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Images> Image;

			/*
				@brief A pointer to the Models component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Models> Model;

			/*
				@brief A pointer to the Moderations component class that
					provides access to its OpenAI API endpoints.
			*/
			std::unique_ptr<liboai::Moderations> Moderation;
		
		public:			
			/*
				@brief Convenience reference to the Authorization class
					singleton used to set authorization information.
			*/
			Authorization& auth = Authorization::Authorizer();
	};
}