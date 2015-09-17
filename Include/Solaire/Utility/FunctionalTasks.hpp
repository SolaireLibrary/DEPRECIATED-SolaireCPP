#ifndef SOLAIRE_UTILITY_FUNCTIONAL_TASK_HPP
#define SOLAIRE_UTILITY_FUNCTIONAL_TASK_HPP

//Copyright 2015 Adam Smith
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

// Contact :
// Email             : solairelibrary@mail.com
// GitHub repository : https://github.com/SolaireLibrary/SolaireCPP

/*!
	\file FunctionalTasks.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 11th September 2015
	Last Modified	: 11th September 2015
*/

#include "Task.hpp"

namespace Solaire{ namespace Utility{

	enum class FunctionTaskMode{
		PRE_EXECUTE,
		EXECUTE,
		POST_EXECUTE
	};

	template<const FunctionTaskMode MODE = FunctionTaskMode::EXECUTE, class FUNCTION = std::function<void(void)>>
	class FunctionTask : public Task{
	private:
		const FUNCTION mFunction;
	public:

		FunctionTask(const FUNCTION aFunction) :
			mFunction(aFunction)
		{

		}

		// Inherited from ReturnTask

		void OnScheduled(const State aPreviousState) override {

		}

		void OnPreExecute() override {
			if(MODE == FunctionTaskMode::PRE_EXECUTE){
				mFunction();
			}
		}

		void OnExecute() override {
			if (MODE == FunctionTaskMode::EXECUTE) {
				mFunction();
			}
		}

		void OnPostExecute(const bool aWasCanceled) override {
			if (MODE == FunctionTaskMode::POST_EXECUTE) {
				mFunction();
			}
		}

		void OnCanceled() override {

		}

		// Inherited from ReturnTask

		void OnRecieveProgress(void* aProgress) override {

		}

	};
}}


#endif
