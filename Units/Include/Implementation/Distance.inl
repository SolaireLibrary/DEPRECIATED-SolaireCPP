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

namespace Solaire{ namespace Units{ namespace DistanceInl{

	static const DistanceUnit INTERMEDIARY_UNIT = DistanceUnit::METRE;
	
	static const double SCALE_METRE 			= 1.0;
	static const double SCALE_MILE 				= SCALE_METRE / 1609.344;
	static const double SCALE_YARD 				= SCALE_MILE * 1760.0;
	static const double SCALE_FOOT 				= SCALE_YARD * 3.0;
	static const double SCALE_INCH 				= SCALE_FOOT * 12.0;
	static const double SCALE_LINK 				= SCALE_INCH / 7.92;
	static const double SCALE_CHAIN 			= SCALE_LINK / 100;
	static const double SCALE_ROD 				= SCALE_FOOT / 16.5;
	static const double SCALE_FURLONG 			= SCALE_MILE * 8.0;
	static const double SCALE_MILE_NAUTICAL 	= SCALE_MILE / 1.15078;
	static const double SCALE_LEAGUE_UK 		= SCALE_METRE / 4828.0;
	static const double SCALE_LEAGUE_US 		= SCALE_FOOT / 15840.0;
	static const double SCALE_LEAGUE_NAUTICAL 	= SCALE_MILE_NAUTICAL / 3.0;
	static const double SCALE_PARSEC 			= SCALE_METRE / 3.08567758e16;
	static const double SCALE_LIGHTYEAR 		= SCALE_METRE / 9.4605284e+15;


	static double GetScale(const DistanceUnit aUnit){
		switch (aUnit){
		case DistanceUnit::METRE:
			return SCALE_METRE;
		case DistanceUnit::MILE:
			return SCALE_MILE;
		case DistanceUnit::MILE_NAUTICAL:
			return SCALE_MILE_NAUTICAL;
		case DistanceUnit::YARD:
			return SCALE_YARD;
		case DistanceUnit::FOOT:
			return SCALE_FOOT;
		case DistanceUnit::INCH:
			return SCALE_INCH;
		case DistanceUnit::PARSEC:
			return SCALE_PARSEC;
		case DistanceUnit::FURLONG:
			return SCALE_FURLONG;
		case DistanceUnit::CHAIN:
			return SCALE_CHAIN;
		case DistanceUnit::LINK:
			return SCALE_LINK;
		case DistanceUnit::ROD:
			return SCALE_ROD;
		case DistanceUnit::LEAGUE_UK:
			return SCALE_LEAGUE_UK;
		case DistanceUnit::LEAGUE_US:
			return SCALE_LEAGUE_US;
		case DistanceUnit::LEAGUE_NAUTICAL:
			return SCALE_LEAGUE_NAUTICAL;
		case DistanceUnit::LIGHTYEAR:
			return SCALE_LIGHTYEAR;
		default:
			throw std::runtime_error("Could not recognise unit");
		}
	}
}}}


#endif