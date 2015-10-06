#ifndef SOLAIRE_UNITS_MASS_HPP
#define SOLAIRE_UNITS_MASS_HPP

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

#include <cstdint>
#include "Metric.hpp"
#include "Mass.inl"

namespace Solaire{ namespace Units{
	template<class VALUE>
	class Mass : public MetricPrefixConverter<MassInl::MassUnit, VALUE, MassInl::INTERMEDIARY_UNIT>
	{
	public:
	    typedef MetricPrefixConverter<MassInl::MassUnit, VALUE, MassInl::INTERMEDIARY_UNIT> ParentClass;
        typedef typename ParentClass::PrefixType PrefixType;
        typedef typename ParentClass::UnitType UnitType;
        typedef typename ParentClass::ValueType ValueType;

	    template<const UnitType CONVERSION>
	    using MassProperty = UnitConverterProperty<UnitType, ValueType, MassInl::INTERMEDIARY_UNIT, CONVERSION>;

	    template<const PrefixType PREFIX, const UnitType CONVERSION>
        using MassPrefixProperty = PrefixConverterProperty<UnitType, ValueType, MassInl::INTERMEDIARY_UNIT, MetricConverter<VALUE>, PREFIX, CONVERSION>;
	public:
		static constexpr ValueType StaticConvertToIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) / MassInl::GetScale(aUnit));
		}

		static constexpr ValueType StaticConvertFromIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) * MassInl::GetScale(aUnit));
    }
    public:
		union{
			MassProperty<UnitType::GRAM>								    Grams;
			MassProperty<UnitType::POUND>								    Pounds;
			MassProperty<UnitType::OUNCE>								    Ounces;
			MassProperty<UnitType::STONE>								    Stones;
			MassProperty<UnitType::SHORT_TON>							    ShortTons;
			MassProperty<UnitType::LONG_TON>							    LongTons;
			MassProperty<UnitType::METRIC_TON>							    MetricTons;
			MassProperty<UnitType::SOLAR_MASS>							    SolarMasses;
			MassProperty<UnitType::TROY_OUNCE>							    TroyOunces;
			MassProperty<UnitType::TROY_POUND>							    TroyPounds;
			MassPrefixProperty<PrefixType::KILO,    UnitType::GRAM>			Kilograms;
			MassPrefixProperty<PrefixType::MILLI,   UnitType::GRAM>		    Milligrams;
			MassPrefixProperty<PrefixType::KILO,    UnitType::METRIC_TON>	Kilotonnes;
			MassPrefixProperty<PrefixType::MEGA,    UnitType::METRIC_TON>	Megatonnes;
		};
	};

	typedef Mass<double> MassD;		//!< A Mass value that is a double.
	typedef Mass<float> MassF;		//!< A Mass value that is a float.
	typedef Mass<int32_t> MassI;	//!< A Mass value that is a signed 32 bit integer.
	typedef Mass<uint32_t> MassU;	//!< A Mass value that is a unsigned 32 bit integer.
}}


#endif
