#ifndef SOLAIRE_UNITS_DISTANCE_HPP
#define SOLAIRE_UNITS_DISTANCE_HPP

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

#include "BaseConverter.hpp"
#include "Metric.hpp"
#include "PrefixConverter.hpp"
#include "Distance.inl"

namespace Solaire{ namespace Units{
	template<class CONVERSION = double>
	class Distance
	{
	public:
		typedef DistanceInl::DistanceUnit unit_t;
		typedef CONVERSION conversion_t;
		typedef typename MetricD::unit_t prefix_t;
	private:
		conversion_t mValue;

		static constexpr conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) / DistanceInl::GetScale(aUnit));
		}

		static constexpr conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) * DistanceInl::GetScale(aUnit));
		}
	public:
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
		union{
			Distance* const Self;
			ConverterProperty<Distance, unit_t::METRE>				Metres;
			ConverterProperty<Distance, unit_t::MILE>				Miles;
			ConverterProperty<Distance, unit_t::MILE_NAUTICAL>		NauticalMiles;
			ConverterProperty<Distance, unit_t::YARD>				Yards;
			ConverterProperty<Distance, unit_t::FOOT>				Feet;
			ConverterProperty<Distance, unit_t::INCH>				Inches;
			ConverterProperty<Distance, unit_t::PARSEC>				Parsecs;
			ConverterProperty<Distance, unit_t::FURLONG>			Furlongs;
			ConverterProperty<Distance, unit_t::CHAIN>				Chains;
			ConverterProperty<Distance, unit_t::LINK>				Links;
			ConverterProperty<Distance, unit_t::ROD>				Rods;
			ConverterProperty<Distance, unit_t::LEAGUE_UK>			LeaguesUK;
			ConverterProperty<Distance, unit_t::LEAGUE_US>			LeaguesUS;
			ConverterProperty<Distance, unit_t::LEAGUE_NAUTICAL>	NauticalLeagues;
			ConverterProperty<Distance, unit_t::LIGHTYEAR>			Lightyears;
			PrefixConverterProperty<Distance, prefix_t::MICRO, unit_t::METRE>			Microns;
			PrefixConverterProperty<Distance, prefix_t::MILLI, unit_t::METRE>			Millimetres;
			PrefixConverterProperty<Distance, prefix_t::CENTI, unit_t::METRE>			Centimetres;
			PrefixConverterProperty<Distance, prefix_t::KILO, unit_t::METRE>			Kilometres;
		};
#endif

		static constexpr unit_t INTERMEDIARY_UNIT = DistanceInl::INTERMEDIARY_UNIT;

		// Constructors

		constexpr Distance() :
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Distance(const conversion_t aValue) :
			mValue(aValue)
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Distance(const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aUnit, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Distance(const prefix_t aPrefix, const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aPrefix, aUnit, MetricD::unit_t::NONE, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Distance(const Distance<conversion_t>& aOther) :
			mValue(aOther.mValue)
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		SOLAIRE_UNITS_CONVERTER_COMMON(Distance<conversion_t>)
		SOLAIRE_UNITS_PREFIXED_CONVERTER_COMMON(Distance<conversion_t>, Metric<conversion_t>)
	};

	typedef Distance<double> DistanceD;		//!< A Distance value that is a double.
	typedef Distance<float> DistanceF;		//!< A Distance value that is a float.
	typedef Distance<int32_t> DistanceI;	//!< A Distance value that is a signed 32 bit integer.
	typedef Distance<uint32_t> DistanceU;	//!< A Distance value that is a unsigned 32 bit integer.
}}


#endif
