#ifndef SOLAIRE_UNITS_VOLUME_INL
#define SOLAIRE_UNITS_VOLUME_INL

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

namespace Solaire{ namespace VolumeInl{

	enum class VolumeUnit{
		LITRE,
		PINT_UK,
		PINT_US,
		GALLON_UK,
		GALLON_US,
		CUP_US,
		OUNCE_UK,
		OUNCE_US,
		TABLE_SPOON_UK,
		TABLE_SPOON_US,
		TEA_SPOON_UK,
		TEA_SPOON_US,
		QUART_UK,
		QUART_US
	};

	static constexpr VolumeUnit INTERMEDIARY_UNIT = VolumeUnit::LITRE;

	static constexpr double SCALE_LITRE				= 1.0;
	static constexpr double SCALE_OUNCE_UK			= SCALE_LITRE * 35.1951;
	static constexpr double SCALE_OUNCE_US			= SCALE_LITRE * 33.814;
	static constexpr double SCALE_PINT_UK			= SCALE_OUNCE_UK / 20.0;
	static constexpr double SCALE_PINT_US			= SCALE_OUNCE_US / 16.0;
	static constexpr double SCALE_GALLON_UK			= SCALE_OUNCE_UK / 160.0;
	static constexpr double SCALE_GALLON_US			= SCALE_OUNCE_US / 128.0;
	static constexpr double SCALE_CUP_US			= SCALE_OUNCE_US / 8.0;
	static constexpr double SCALE_TABLE_SPOON_UK	= SCALE_OUNCE_UK * 1.6;
	static constexpr double SCALE_TABLE_SPOON_US	= SCALE_OUNCE_US * 2.0;
	static constexpr double SCALE_TEA_SPOON_UK		= SCALE_OUNCE_UK * 4.8;
	static constexpr double SCALE_TEA_SPOON_US		= SCALE_OUNCE_US * 6.0;
	static constexpr double SCALE_QUART_UK			= SCALE_OUNCE_UK / 40.0;
	static constexpr double SCALE_QUART_US			= SCALE_OUNCE_US / 32.0;


	static constexpr double GetScale(const VolumeUnit aUnit){
		return
			aUnit == VolumeUnit::LITRE			? SCALE_LITRE :
			aUnit == VolumeUnit::PINT_UK		? SCALE_PINT_UK :
			aUnit == VolumeUnit::PINT_US		? SCALE_PINT_US :
			aUnit == VolumeUnit::GALLON_UK		? SCALE_GALLON_UK :
			aUnit == VolumeUnit::GALLON_US		? SCALE_GALLON_US :
			aUnit == VolumeUnit::CUP_US			? SCALE_CUP_US :
			aUnit == VolumeUnit::OUNCE_UK		? SCALE_OUNCE_UK :
			aUnit == VolumeUnit::OUNCE_US		? SCALE_OUNCE_US :
			aUnit == VolumeUnit::TABLE_SPOON_UK	? SCALE_TABLE_SPOON_UK :
			aUnit == VolumeUnit::TABLE_SPOON_US	? SCALE_TABLE_SPOON_US :
			aUnit == VolumeUnit::TEA_SPOON_UK	? SCALE_TEA_SPOON_UK :
			aUnit == VolumeUnit::TEA_SPOON_US	? SCALE_TEA_SPOON_US :
			aUnit == VolumeUnit::QUART_UK		? SCALE_QUART_UK :
			aUnit == VolumeUnit::QUART_US		? SCALE_QUART_US :
												0.0;
	}
}}


#endif
