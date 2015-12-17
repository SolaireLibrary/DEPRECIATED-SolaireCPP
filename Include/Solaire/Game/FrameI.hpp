#ifndef SOLAIRE_FRAMEI_HPP
#define SOLAIRE_FRAMEI_HPP

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
	\file FrameI.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 17th December 2015
	Last Modified	: 17th December 2015
*/

#include <cstdint>
#include "..\Core\ModuleHeader.hpp"
#include "..\Core\System.hpp"

namespace Solaire{

	SOLAIRE_EXPORT_INTERFACE FrameI {
	public:
		// Frame Management
		virtual uint64_t SOLAIRE_EXPORT_CALL GetFrameNumber() const throw() = 0;

		// Frame Timing
		virtual uint64_t SOLAIRE_EXPORT_CALL GetStartTime() const throw() = 0;
		virtual uint64_t SOLAIRE_EXPORT_CALL GetDuration() const throw() = 0;

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_EXPORT_CALL GetEndTime() const throw() {
			return GetStartTime() + GetDuration();
		}

		SOLAIRE_FORCE_INLINE uint64_t SOLAIRE_EXPORT_CALL GetRemainingTime() const throw() {
			const uint64_t end = GetEndTime();
			const uint64_t current = GetTimeMilliseconds();
			return current < end ? end - current : 0;
		}
	};

}

#endif
