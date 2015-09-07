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

#include <stdexcept>
#include "..\DefaultUnits.hpp"

namespace Solaire{ namespace Units{ namespace MassInl{

	static const MassUnit INTERMEDIARY_UNIT = MassUnit::METRIC_TON;
	
	static const double SCALE_METRIC_TON	= 1.0;
	static const double SCALE_GRAM			= SCALE_METRIC_TON * 1e+6;
	static const double SCALE_OUNCE			= SCALE_GRAM / 28.3495;
	static const double SCALE_POUND			= SCALE_OUNCE / 16.0;
	static const double SCALE_STONE			= SCALE_POUND / 14.0;
	static const double SCALE_SHORT_TON		= SCALE_POUND / 2000.0;
	static const double SCALE_LONG_TON		= SCALE_POUND / 2240.0;
	static const double SCALE_TROY_OUNCE	= SCALE_OUNCE / 1.09714286;
	static const double SCALE_TROY_POUND	= SCALE_TROY_OUNCE / 12.0;
	static const double SCALE_SOLAR_MASS	= SCALE_METRIC_TON / 1.989e+27;


	static double GetScale(const MassUnit aUnit){
		switch (aUnit){
		case MassUnit::GRAM:
			return SCALE_GRAM;
		case MassUnit::OUNCE:
			return SCALE_OUNCE;
		case MassUnit::POUND:
			return SCALE_POUND;
		case MassUnit::STONE:
			return SCALE_STONE;
		case MassUnit::SHORT_TON:
			return SCALE_SHORT_TON;
		case MassUnit::LONG_TON:
			return SCALE_SHORT_TON;
		case MassUnit::METRIC_TON:
			return SCALE_METRIC_TON;
		case MassUnit::TROY_OUNCE:
			return SCALE_TROY_OUNCE;
		case MassUnit::TROY_POUND:
			return SCALE_TROY_POUND;
		case MassUnit::SOLAR_MASS:
			return SCALE_SOLAR_MASS;
		default:
			throw std::runtime_error("Could not recognise unit");
		}
	}
}}}


#endif