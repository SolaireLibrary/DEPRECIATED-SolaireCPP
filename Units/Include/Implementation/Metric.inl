#ifndef SOLAIRE_UNITS_METRIC_INL
#define SOLAIRE_UNITS_METRIC_INL

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

namespace Solaire{ namespace Units{ namespace MetricInl{

	static constexpr MetricPrefix INTERMEDIARY_UNIT = MetricPrefix::NONE;

	static constexpr double GetScale(const MetricPrefix aUnit){
		return
			aUnit == MetricPrefix::YOTTA	? 1e+24 :
			aUnit == MetricPrefix::ZETTA	? 1e+21 :
			aUnit == MetricPrefix::EXA		? 1e+18 :
			aUnit == MetricPrefix::PETA		? 1e+15 :
			aUnit == MetricPrefix::TERA		? 1e+12 :
			aUnit == MetricPrefix::GIGA		? 1e+9 :
			aUnit == MetricPrefix::MEGA		? 1e+6 :
			aUnit == MetricPrefix::KILO		? 1e+3 :
			aUnit == MetricPrefix::HECTO	? 1e+2 :
			aUnit == MetricPrefix::DECA		? 1e+1 :
			aUnit == MetricPrefix::NONE		? 1.0 :
			aUnit == MetricPrefix::DECI		? 1e-1 :
			aUnit == MetricPrefix::CENTI	? 1e-2 :
			aUnit == MetricPrefix::MILLI	? 1e-3 :
			aUnit == MetricPrefix::MICRO	? 1e-6 :
			aUnit == MetricPrefix::NANO		? 1e-9 :
			aUnit == MetricPrefix::PICO		? 1e-12 :
			aUnit == MetricPrefix::FEMTO	? 1e-15 :
			aUnit == MetricPrefix::ATTO		? 1e-18 :
			aUnit == MetricPrefix::ZEPTO	? 1e-21 :
			aUnit == MetricPrefix::YOCTO	? 1e-24 :
			aUnit == MetricPrefix::KIBI		? 2e+10 :
			aUnit == MetricPrefix::MEBI		? 2e+20 :
			aUnit == MetricPrefix::GIBI		? 2e+30 :
			aUnit == MetricPrefix::TEBI		? 2e+40 :
			aUnit == MetricPrefix::PEBI		? 2e+50 :
			aUnit == MetricPrefix::EXBI		? 2e+60 :
			aUnit == MetricPrefix::ZEBI		? 2e+70 :
			aUnit == MetricPrefix::YOBI		? 2e+80 :
			0.0;
	}
}}}


#endif