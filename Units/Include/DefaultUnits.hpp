#ifndef SOLAIRE_UNITS_DEFAULT_UNITS_HPP
#define SOLAIRE_UNITS_DEFAULT_UNITS_HPP

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

namespace Solaire{ namespace Units{

	enum class MassUnit{
		GRAM,
		POUND,
		OUNCE,
		STONE,
		SHORT_TON,
		LONG_TON,
		METRIC_TON,
		TROY_POUND,
		TROY_OUNCE,
		SOLAR_MASS
	};
	
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

	enum class TemperatureUnit{
		CELCIUS,
		FAHRENHEIT,
		KELVIN,
		GAS_MARK
	};

	enum class DigitalUnit{
		BIT,
		NYBBLE,
		BYTE
	};

	enum class DistanceUnit{
		METRE,
		MILE,
		MILE_NAUTICAL,
		YARD,
		FOOT,
		INCH,
		PARSEC,
		FURLONG,
		CHAIN,
		LINK,
		ROD,
		LEAGUE_UK,
		LEAGUE_US,
		LEAGUE_NAUTICAL,
		LIGHTYEAR
	};

	enum class MetricPrefix{
		// SI
		YOTTA,
		ZETTA,
		EXA,
		PETA,
		TERA,
		GIGA,
		MEGA,
		KILO,
		HECTO,
		DECA,
		NONE,
		DECI,
		CENTI,
		MILLI,
		MICRO,
		NANO,
		PICO,
		FEMTO,
		ATTO,
		ZEPTO,
		YOCTO,
		// Digital
		KIBI,
		MEBI,
		GIBI,
		TEBI,
		PEBI,
		EXBI,
		ZEBI,
		YOBI
	};
}}


#endif