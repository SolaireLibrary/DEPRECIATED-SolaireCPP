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
#include "Distance.inl"

namespace Solaire{
	template<class VALUE>
	class DistanceConverter : public MetricPrefixConverter<DistanceInl::DistanceUnit, VALUE, DistanceInl::INTERMEDIARY_UNIT>
	{
	public:
	    typedef MetricPrefixConverter<DistanceInl::DistanceUnit, VALUE, DistanceInl::INTERMEDIARY_UNIT> ParentClass;
        typedef typename ParentClass::PrefixType PrefixType;
        typedef typename ParentClass::UnitType UnitType;
        typedef typename ParentClass::ValueType ValueType;
        typedef MetricConverter<ValueType> PrefixConverterType;

	    template<const UnitType CONVERSION>
	    using DistanceProperty = UnitConverterProperty<DistanceConverter<ValueType>, CONVERSION>;

	    template<const PrefixType PREFIX, const UnitType CONVERSION>
        using DistancePrefixProperty = PrefixConverterProperty<DistanceConverter<ValueType>, PREFIX, CONVERSION>;
	public:
		static constexpr ValueType ConvertToIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) / DistanceInl::GetScale(aUnit));
		}

		static constexpr ValueType ConvertFromIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) * DistanceInl::GetScale(aUnit));
		}
    public:
        DistanceConverter():
            ParentClass(static_cast<ValueType>(0)),
            Metres(*this)
        {}

        DistanceConverter(const ValueType aValue):
            ParentClass(aValue),
            Metres(*this)
        {}

        DistanceConverter(const DistanceConverter& aOther):
            ParentClass(aOther.Get()),
            Metres(*this)
        {}

        DistanceConverter(DistanceConverter&& aOther):
            ParentClass(aOther.Get()),
            Metres(*this)
        {}

        // Inherited from UnitConverter

        ValueType ConvertToIntermediateUnit(const UnitType aUnit, const ValueType aValue) const override{
            return StaticConvertToIntermediaryUnit(aUnit, aValue);
        }

        ValueType ConvertFromIntermediateUnit(const UnitType aUnit, const ValueType aValue) const override{
            return ConvertFromIntermediateUnit(aUnit, aValue);
        }
	public:
		union{
			DistanceProperty<UnitType::METRE>				Metres;
			DistanceProperty<UnitType::MILE>				Miles;
			DistanceProperty<UnitType::MILE_NAUTICAL>		NauticalMiles;
			DistanceProperty<UnitType::YARD>				Yards;
			DistanceProperty<UnitType::FOOT>				Feet;
			DistanceProperty<UnitType::INCH>				Inches;
			DistanceProperty<UnitType::PARSEC>				Parsecs;
			DistanceProperty<UnitType::FURLONG>			    Furlongs;
			DistanceProperty<UnitType::CHAIN>				Chains;
			DistanceProperty<UnitType::LINK>				Links;
			DistanceProperty<UnitType::ROD>				    Rods;
			DistanceProperty<UnitType::LEAGUE_UK>			LeaguesUK;
			DistanceProperty<UnitType::LEAGUE_US>			LeaguesUS;
			DistanceProperty<UnitType::LEAGUE_NAUTICAL>	    NauticalLeagues;
			DistanceProperty<UnitType::LIGHTYEAR>			Lightyears;
			DistancePrefixProperty<PrefixType::MICRO, UnitType::METRE>			Microns;
			DistancePrefixProperty<PrefixType::MILLI, UnitType::METRE>			Millimetres;
			DistancePrefixProperty<PrefixType::CENTI, UnitType::METRE>			Centimetres;
			DistancePrefixProperty<PrefixType::KILO, UnitType::METRE>			Kilometres;
		};

	};

	typedef DistanceConverter<double> DistanceConverterD;		//!< A Distance value that is a double.
	typedef DistanceConverter<float> DistanceConverterF;		//!< A Distance value that is a float.
	typedef DistanceConverter<int32_t> DistanceConverterI;	    //!< A Distance value that is a signed 32 bit integer.
	typedef DistanceConverter<uint32_t> DistanceConverterU;	    //!< A Distance value that is a unsigned 32 bit integer.
}


#endif
