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
#include "..\DefaultUnits.hpp"

namespace Solaire{ namespace Units{ namespace VolumeInl{

	static const VolumeUnit INTERMEDIARY_UNIT = VolumeUnit::LITRE;
	
	static const double SCALE_LITRE				= 1.0;
	static const double SCALE_OUNCE_UK			= SCALE_LITRE * 35.1951;
	static const double SCALE_OUNCE_US			= SCALE_LITRE * 33.814;
	static const double SCALE_PINT_UK			= SCALE_OUNCE_UK / 20.0;
	static const double SCALE_PINT_US			= SCALE_OUNCE_US / 16.0;
	static const double SCALE_GALLON_UK			= SCALE_OUNCE_UK / 160.0;
	static const double SCALE_GALLON_US			= SCALE_OUNCE_US / 128.0;
	static const double SCALE_CUP_US			= SCALE_OUNCE_US / 8.0;
	static const double SCALE_TABLE_SPOON_UK	= SCALE_OUNCE_UK * 1.6;
	static const double SCALE_TABLE_SPOON_US	= SCALE_OUNCE_US * 2.0;
	static const double SCALE_TEA_SPOON_UK		= SCALE_OUNCE_UK * 4.8;
	static const double SCALE_TEA_SPOON_US		= SCALE_OUNCE_US * 6.0;
	static const double SCALE_QUART_UK			= SCALE_OUNCE_UK / 40.0;
	static const double SCALE_QUART_US			= SCALE_OUNCE_US / 32.0;


	static double GetScale(const VolumeUnit aUnit){
		switch (aUnit){
		case VolumeUnit::LITRE:
			return SCALE_LITRE;
		case VolumeUnit::PINT_UK:
			return SCALE_PINT_UK;
		case VolumeUnit::PINT_US:
			return SCALE_PINT_US;
		case VolumeUnit::GALLON_UK:
			return SCALE_GALLON_UK;
		case VolumeUnit::GALLON_US:
			return SCALE_GALLON_US;
		case VolumeUnit::CUP_US:
			return SCALE_CUP_US;
		case VolumeUnit::OUNCE_UK:
			return SCALE_OUNCE_UK;
		case VolumeUnit::OUNCE_US:
			return SCALE_OUNCE_US;
		case VolumeUnit::TABLE_SPOON_UK:
			return SCALE_TABLE_SPOON_UK;
		case VolumeUnit::TABLE_SPOON_US:
			return SCALE_TABLE_SPOON_US;
		case VolumeUnit::TEA_SPOON_UK:
			return SCALE_TEA_SPOON_UK;
		case VolumeUnit::TEA_SPOON_US:
			return SCALE_TEA_SPOON_US;
		case VolumeUnit::QUART_UK:
			return SCALE_QUART_UK;
		case VolumeUnit::QUART_US:
			return SCALE_QUART_US;
		default:
			throw std::runtime_error("Could not recognise unit");
		}
	}
}}}


#endif