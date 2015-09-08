#ifndef SOLAIRE_UNITS_DISTANCE_INL
#define SOLAIRE_UNITS_DISTANCE_INL

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

namespace Solaire{ namespace Units{ namespace TemperatureInl{
	static const TemperatureUnit INTERMEDIARY_UNIT = TemperatureUnit::CELCIUS;
	
	static double ConvertToIntermediary(TemperatureUnit aUnit, double aValue){
		switch (aUnit){
		case TemperatureUnit::CELCIUS:
			return aValue;
		case TemperatureUnit::FAHRENHEIT:
			return (aValue - 32.0) / 1.8;
		case TemperatureUnit::KELVIN:
			return aValue - 273.15;
		case TemperatureUnit::GAS_MARK:
			return (aValue * 14.0) + 121.0;
		default:
			throw std::runtime_error("Could not recognise unit");
		}
	}

	static double ConvertFromIntermediary(TemperatureUnit aUnit, double aValue){
		switch (aUnit){
		case TemperatureUnit::CELCIUS:
			return aValue;
		case TemperatureUnit::FAHRENHEIT:
			return (aValue * 1.8) + 32.0;
		case TemperatureUnit::KELVIN:
			return aValue + 273.15;
		case TemperatureUnit::GAS_MARK:
			return (aValue - 121.0) / 14.0;
		default:
			throw std::runtime_error("Could not recognise unit");
	}
	}
}}}


#endif