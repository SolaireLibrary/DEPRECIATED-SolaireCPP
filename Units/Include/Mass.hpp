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
#include "Implementation\Mass.inl"

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

		conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const{
			return static_cast<conversion_t>(static_cast<double>(aValue) / MassInl::GetScale(aUnit));
		}

		conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const{
			return static_cast<conversion_t>(static_cast<double>(aValue) * MassInl::GetScale(aUnit));
		}

		unit_t GetIntermediaryUnit() const{
			return INTERMEDIARY_UNIT;
		}
	public:

#ifndef SOLAIRE_UNITS_NO_PROPERTIES
		union{
			Mass* const Self;
			ConverterProperty<Mass, unit_t::GRAM>								Grams;
			ConverterProperty<Mass, unit_t::POUND>								Pounds;
			ConverterProperty<Mass, unit_t::OUNCE>								Ounces;
			ConverterProperty<Mass, unit_t::STONE>								Stone;
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

		static constexpr conversion_t Convert(const unit_t aInput, const unit_t aOutput, const conversion_t aValue){
			return static_cast<conversion_t>(
				(static_cast<double>(aValue) / MassInl::GetScale(aInput)) *
				MassInl::GetScale(aOutput)
			);
		}

		

		static constexpr conversion_t Convert(const MetricD::unit_t aInputPrefix, const unit_t aInputUnit, const MetricD::unit_t aOutputPrefix, const unit_t aOutputUnit, const conversion_t aValue){
			return static_cast<conversion_t>(
				MetricD::Convert(
					MetricD::unit_t::NONE, 
					aOutputPrefix,
					MassD::Convert(
						aInputUnit,
						aOutputUnit,
						MetricD::Convert(
							aInputPrefix,
							MetricD::unit_t::NONE,
							static_cast<double>(aValue)
						)
					)
				)
			);
		}

		// Constructors

		constexpr Mass() : 
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Mass(const conversion_t aValue) :
			mValue(aValue)
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Mass(const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aUnit, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Mass(const prefix_t aPrefix, const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aPrefix, aUnit, MetricD::NONE, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{
			
		}

		constexpr Mass(const Mass<conversion_t>& aOther) :
			mValue(aOther.mValue)
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
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