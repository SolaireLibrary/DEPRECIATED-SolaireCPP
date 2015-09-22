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
#include "BaseConverter.hpp"
#include "Metric.hpp"
#include "PrefixConverter.hpp"
#include "Mass.inl"

namespace Solaire{ namespace Units{
	template<class CONVERSION = double>
	class Mass
	{
	public:
		typedef MassInl::MassUnit unit_t;
		typedef CONVERSION conversion_t;
		typedef typename MetricD::unit_t prefix_t;
	private:
		conversion_t mValue;

		static constexpr conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) / MassInl::GetScale(aUnit));
		}

		static constexpr conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) * MassInl::GetScale(aUnit));
		}
	public:

#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
		union{
			Mass* const Self;
			ConverterProperty<Mass, unit_t::GRAM>								Grams;
			ConverterProperty<Mass, unit_t::POUND>								Pounds;
			ConverterProperty<Mass, unit_t::OUNCE>								Ounces;
			ConverterProperty<Mass, unit_t::STONE>								Stones;
			ConverterProperty<Mass, unit_t::SHORT_TON>							ShortTons;
			ConverterProperty<Mass, unit_t::LONG_TON>							LongTons;
			ConverterProperty<Mass, unit_t::METRIC_TON>							MetricTons;
			ConverterProperty<Mass, unit_t::SOLAR_MASS>							SolarMasses;
			ConverterProperty<Mass, unit_t::TROY_OUNCE>							TroyOunces;
			ConverterProperty<Mass, unit_t::TROY_POUND>							TroyPounds;
			PrefixConverterProperty<Mass, prefix_t::KILO, unit_t::GRAM>			Kilograms;
			PrefixConverterProperty<Mass, prefix_t::MILLI, unit_t::GRAM>		Milligrams;
			PrefixConverterProperty<Mass, prefix_t::KILO, unit_t::METRIC_TON>	Kilotonnes;
			PrefixConverterProperty<Mass, prefix_t::MEGA, unit_t::METRIC_TON>	Megatonnes;
		};
#endif

		static constexpr unit_t INTERMEDIARY_UNIT = MassInl::INTERMEDIARY_UNIT;

		// Constructors

		constexpr Mass() :
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Mass(const conversion_t aValue) :
			mValue(aValue)
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Mass(const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aUnit, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Mass(const prefix_t aPrefix, const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aPrefix, aUnit, MetricD::unit_t::NONE, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Mass(const Mass<conversion_t>& aOther) :
			mValue(aOther.mValue)
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{
		}

		SOLAIRE_UNITS_CONVERTER_COMMON(Mass<conversion_t>)
		SOLAIRE_UNITS_PREFIXED_CONVERTER_COMMON(Mass<conversion_t>, Metric<conversion_t>)
	};

	typedef Mass<double> MassD;		//!< A Mass value that is a double.
	typedef Mass<float> MassF;		//!< A Mass value that is a float.
	typedef Mass<int32_t> MassI;	//!< A Mass value that is a signed 32 bit integer.
	typedef Mass<uint32_t> MassU;	//!< A Mass value that is a unsigned 32 bit integer.
}}


#endif
