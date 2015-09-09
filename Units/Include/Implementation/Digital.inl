#ifndef SOLAIRE_UNITS_DIGITAL_INL
#define SOLAIRE_UNITS_DIGITAL_INL

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

#include <stdexcept>

namespace Solaire{ namespace Units{ namespace DigitalInl{

	enum class DigitalUnit{
		BIT,
		NYBBLE,
		BYTE
	};

	static constexpr DigitalUnit INTERMEDIARY_UNIT = DigitalUnit::BYTE;
	
	static constexpr double SCALE_BYTE	 			= 1.0;
	static constexpr double SCALE_NYBBLE	 		= SCALE_BYTE * 2.0;
	static constexpr double SCALE_BIT	 			= SCALE_BYTE * 8.0;


	static constexpr double GetScale(const DigitalUnit aUnit){
		return
			aUnit == DigitalUnit::BIT		?  SCALE_BIT :
			aUnit == DigitalUnit::NYBBLE	? SCALE_NYBBLE :
			aUnit == DigitalUnit::BYTE		? SCALE_BYTE :
											0.0;
	}
}}}


#endif