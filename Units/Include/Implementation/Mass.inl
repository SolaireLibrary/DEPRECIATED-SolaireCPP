#ifndef SOLAIRE_UNITS_MASS_INL
#define SOLAIRE_UNITS_MASS_INL

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

namespace Solaire{ namespace Units{ namespace MassInl{

	static constexpr MassUnit INTERMEDIARY_UNIT = MassUnit::METRIC_TON;
	
	static constexpr double SCALE_METRIC_TON = 1.0;
	static constexpr double SCALE_GRAM			= SCALE_METRIC_TON * 1e+6;
	static constexpr double SCALE_OUNCE			= SCALE_GRAM / 28.3495;
	static constexpr double SCALE_POUND			= SCALE_OUNCE / 16.0;
	static constexpr double SCALE_STONE			= SCALE_POUND / 14.0;
	static constexpr double SCALE_SHORT_TON		= SCALE_POUND / 2000.0;
	static constexpr double SCALE_LONG_TON		= SCALE_POUND / 2240.0;
	static constexpr double SCALE_TROY_OUNCE	= SCALE_OUNCE / 1.09714286;
	static constexpr double SCALE_TROY_POUND	= SCALE_TROY_OUNCE / 12.0;
	static constexpr double SCALE_SOLAR_MASS	= SCALE_METRIC_TON / 1.989e+27;


	static constexpr double GetScale(const MassUnit aUnit){
		return
			aUnit == MassUnit::GRAM ? SCALE_GRAM :
			aUnit == MassUnit::OUNCE ? SCALE_OUNCE :
			aUnit == MassUnit::POUND ? SCALE_POUND :
			aUnit == MassUnit::OUNCE ? SCALE_OUNCE :
			aUnit == MassUnit::STONE ? SCALE_STONE :
			aUnit == MassUnit::SHORT_TON ? SCALE_SHORT_TON :
			aUnit == MassUnit::LONG_TON ? SCALE_LONG_TON :
			aUnit == MassUnit::SHORT_TON ? SCALE_SHORT_TON :
			aUnit == MassUnit::METRIC_TON ? SCALE_METRIC_TON :
			aUnit == MassUnit::TROY_OUNCE ? SCALE_TROY_OUNCE :
			aUnit == MassUnit::TROY_POUND ? SCALE_TROY_POUND :
			aUnit == MassUnit::SOLAR_MASS ? SCALE_SOLAR_MASS :
			0.0;
	}
}}}


#endif