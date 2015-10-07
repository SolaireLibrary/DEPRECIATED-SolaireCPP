#ifndef SOLAIRE_UNITS_TEMPERATURE_INL
#define SOLAIRE_UNITS_TEMPERATURE_INL

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

namespace Solaire{ namespace TemperatureInl{

	enum class TemperatureUnit{
		CELCIUS,
		FAHRENHEIT,
		KELVIN,
		GAS_MARK
	};

	static const TemperatureUnit INTERMEDIARY_UNIT = TemperatureUnit::CELCIUS;

	static constexpr double ConvertToIntermediary(const TemperatureUnit aUnit, const double aValue){
		return
			aUnit == TemperatureUnit::CELCIUS		? aValue :
			aUnit == TemperatureUnit::FAHRENHEIT	? (aValue - 32.0) / 1.8 :
			aUnit == TemperatureUnit::KELVIN		? aValue - 273.15 :
			aUnit == TemperatureUnit::GAS_MARK		? (aValue * 14.0) + 121.0 :
													0.0;
	}

	static constexpr double ConvertFromIntermediary(const TemperatureUnit aUnit, double aValue){
		return
			aUnit == TemperatureUnit::CELCIUS		? aValue :
			aUnit == TemperatureUnit::FAHRENHEIT	? (aValue * 1.8) + 32.0 :
			aUnit == TemperatureUnit::KELVIN		? aValue + 273.15 :
			aUnit == TemperatureUnit::GAS_MARK		? (aValue - 121.0) / 14.0 :
													0.0;
	}
}}


#endif
