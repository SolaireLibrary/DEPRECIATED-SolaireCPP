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

#include <stdexcept>
#include "..\DefaultUnits.hpp"

namespace Solaire{ namespace Units{ namespace MetricInl{

	static const MetricPrefix INTERMEDIARY_UNIT = MetricPrefix::NONE;

	static double GetScale(const MetricPrefix aUnit){
		switch (aUnit){
		case MetricPrefix::YOTTA:
			return 1e+24;
		case MetricPrefix::ZETTA:
			return 1e+21;
		case MetricPrefix::EXA:
			return 1e+18;
		case MetricPrefix::PETA:
			return 1e+15;
		case MetricPrefix::TERA:
			return 1e+12;
		case MetricPrefix::GIGA:
			return 1e+9;
		case MetricPrefix::MEGA:
			return 1e+6;
		case MetricPrefix::KILO:
			return 1e+3;
		case MetricPrefix::HECTO:
			return 1e+2;
		case MetricPrefix::DECA:
			return 1e+1;
		case MetricPrefix::NONE:
			return 1.0;
		case MetricPrefix::DECI:
			return 1e-1;
		case MetricPrefix::CENTI:
			return 1e-2;
		case MetricPrefix::MILLI:
			return 1e-3;
		case MetricPrefix::MICRO:
			return 1e-6;
		case MetricPrefix::NANO:
			return 1e-9;
		case MetricPrefix::PICO:
			return 1e-12;
		case MetricPrefix::FEMTO:
			return 1e-15;
		case MetricPrefix::ATTO:
			return 1e-18;
		case MetricPrefix::ZEPTO:
			return 1e-21;
		case MetricPrefix::YOCTO:
			return 1e-24;
		case MetricPrefix::KIBI:
			return 2e+10;
		case MetricPrefix::MEBI:
			return 2e+20;
		case MetricPrefix::GIBI:
			return 2e+30;
		case MetricPrefix::TEBI:
			return 2e+40;
		case MetricPrefix::PEBI:
			return 2e+50;
		case MetricPrefix::EXBI:
			return 2e+60;
		case MetricPrefix::ZEBI:
			return 2e+70;
		case MetricPrefix::YOBI:
			return 2e+80;
		default:
			throw std::runtime_error("Could not recognise prefix");
		}
	}
}}}


#endif