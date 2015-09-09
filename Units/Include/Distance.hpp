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
#include "DefaultUnits.hpp"
#include "Implementation\Distance.inl"

namespace Solaire{ namespace Units{
	template<class CONVERSION = double>
	class Distance : public PrefixConverter<Metric<CONVERSION>, DistanceUnit, CONVERSION>
	{
	private:
		void ConfigureProperties(){
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			Metres.mParent = this;
#endif
		}
	protected:
		// Inherited from BaseConverter

		conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const override{
			return static_cast<conversion_t>(static_cast<double>(aValue) / DistanceInl::GetScale(aUnit));
		}

		conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const override{
			return static_cast<conversion_t>(static_cast<double>(aValue) * DistanceInl::GetScale(aUnit));
		}

		unit_t GetIntermediaryUnit() const override{
			return INTERMEDIARY_UNIT;
		}
	public:
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
		union{
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

		static const unit_t INTERMEDIARY_UNIT = DistanceInl::INTERMEDIARY_UNIT;

		// Constructors

		Distance() : 
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
		}

		Distance(conversion_t aValue) : 
			PrefixConverter(aValue)
		{

		}

		Distance(unit_t aUnit, conversion_t aValue) : 
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
			Set(aUnit, aValue);
		}

		Distance(prefix_t aPrefix, unit_t aUnit, conversion_t aValue) :
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
			Set(aPrefix, aUnit, aValue);
		}

		Distance(const BaseConverter<unit_t, conversion_t>& aOther) :
			PrefixConverter(aOther.Get(GetIntermediaryUnit()))
		{
			ConfigureProperties();
		}
	};
}}


#endif