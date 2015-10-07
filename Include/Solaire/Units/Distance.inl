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

namespace Solaire{ namespace DistanceInl{

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

	static constexpr DistanceUnit INTERMEDIARY_UNIT = DistanceUnit::METRE;

	static constexpr double SCALE_METRE 			= 1.0;
	static constexpr double SCALE_MILE 				= SCALE_METRE / 1609.344;
	static constexpr double SCALE_YARD 				= SCALE_MILE * 1760.0;
	static constexpr double SCALE_FOOT 				= SCALE_YARD * 3.0;
	static constexpr double SCALE_INCH 				= SCALE_FOOT * 12.0;
	static constexpr double SCALE_LINK 				= SCALE_INCH / 7.92;
	static constexpr double SCALE_CHAIN 			= SCALE_LINK / 100;
	static constexpr double SCALE_ROD 				= SCALE_FOOT / 16.5;
	static constexpr double SCALE_FURLONG 			= SCALE_MILE * 8.0;
	static constexpr double SCALE_MILE_NAUTICAL 	= SCALE_MILE / 1.15078;
	static constexpr double SCALE_LEAGUE_UK 		= SCALE_METRE / 4828.0;
	static constexpr double SCALE_LEAGUE_US 		= SCALE_FOOT / 15840.0;
	static constexpr double SCALE_LEAGUE_NAUTICAL 	= SCALE_MILE_NAUTICAL / 3.0;
	static constexpr double SCALE_PARSEC 			= SCALE_METRE / 3.08567758e16;
	static constexpr double SCALE_LIGHTYEAR 		= SCALE_METRE / 9.4605284e+15;


	static constexpr double GetScale(const DistanceUnit aUnit){
		return
			aUnit == DistanceUnit::METRE				? SCALE_METRE :
			aUnit == DistanceUnit::MILE					? SCALE_MILE :
			aUnit == DistanceUnit::MILE_NAUTICAL		? SCALE_MILE_NAUTICAL :
			aUnit == DistanceUnit::YARD					? SCALE_YARD :
			aUnit == DistanceUnit::FOOT					? SCALE_FOOT :
			aUnit == DistanceUnit::INCH					? SCALE_INCH :
			aUnit == DistanceUnit::PARSEC				? SCALE_PARSEC :
			aUnit == DistanceUnit::FURLONG				? SCALE_FURLONG :
			aUnit == DistanceUnit::CHAIN				? SCALE_CHAIN :
			aUnit == DistanceUnit::LINK					? SCALE_LINK :
			aUnit == DistanceUnit::ROD					? SCALE_ROD :
			aUnit == DistanceUnit::LEAGUE_UK			? SCALE_LEAGUE_UK :
			aUnit == DistanceUnit::LEAGUE_US			? SCALE_LEAGUE_US :
			aUnit == DistanceUnit::LEAGUE_NAUTICAL		? SCALE_LEAGUE_NAUTICAL :
			aUnit == DistanceUnit::LIGHTYEAR			? SCALE_LIGHTYEAR :
														0.0;
	}
}}


#endif
