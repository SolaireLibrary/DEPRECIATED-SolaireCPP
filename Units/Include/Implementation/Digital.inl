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
#include "..\DefaultUnits.hpp"

namespace Solaire{ namespace Units{ namespace DigitalInl{

	static const DigitalUnit INTERMEDIARY_UNIT = DigitalUnit::BYTE;
	
	static const double SCALE_BYTE	 			= 1.0;
	static const double SCALE_NYBBLE	 		= SCALE_BYTE * 2.0;
	static const double SCALE_BIT	 			= SCALE_BYTE * 8.0;


	static double GetScale(const DigitalUnit aUnit){
		switch (aUnit){
		case DigitalUnit::BIT:
			return SCALE_BIT;
		case DigitalUnit::NYBBLE:
			return SCALE_NYBBLE;
		case DigitalUnit::BYTE:
			return SCALE_BYTE;
		default:
			throw std::runtime_error("Could not recognise unit");
		}
	}
}}}


#endif