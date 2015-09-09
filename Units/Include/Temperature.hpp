#ifndef SOLAIRE_UNITS_TEMPERATURE_HPP
#define SOLAIRE_UNITS_TEMPERATURE_HPP

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
#include "Implementation\Temperature.inl"

namespace Solaire{ namespace Units{

	template<class CONVERSION = double>
	class Temperature
	{
	public:
		typedef TemperatureUnit unit_t;
		typedef CONVERSION conversion_t;
		typedef MetricPrefix prefix_t;
	private:
		conversion_t mValue;
	protected:
		conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const{
			return static_cast<conversion_t>(TemperatureInl::ConvertToIntermediary(aUnit, static_cast<double>(aValue)));
		}

		conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const{
			return static_cast<conversion_t>(TemperatureInl::ConvertFromIntermediary(aUnit, static_cast<double>(aValue)));
		}

		unit_t GetIntermediaryUnit() const{
			return INTERMEDIARY_UNIT;
		}
	public:
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
		union{
			Temperature* const Self;
			ConverterProperty<Temperature, unit_t::CELCIUS>						Celcius;
			ConverterProperty<Temperature, unit_t::FAHRENHEIT>					Fahrenheit;
			ConverterProperty<Temperature, unit_t::KELVIN>						Kelvin;
			ConverterProperty<Temperature, unit_t::GAS_MARK>					GasMark;
		};
#endif

		static constexpr unit_t INTERMEDIARY_UNIT = TemperatureInl::INTERMEDIARY_UNIT;

		static constexpr conversion_t Convert(const unit_t aInput, const unit_t aOutput, const conversion_t aValue){
			return static_cast<conversion_t>(
				TemperatureInl::ConvertFromIntermediary(
					aOutput, 
					TemperatureInl::ConvertToIntermediary(aInput, static_cast<double>(aValue))
				)
			);
		}

		// Constructors

		Temperature() : 
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		Temperature(conversion_t aValue) : 
			mValue(aValue)
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		Temperature(unit_t aUnit, conversion_t aValue) : 
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{
			Set(aUnit, aValue);
		}

		Temperature(prefix_t aPrefix, unit_t aUnit, conversion_t aValue) :
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{
			Set(aPrefix, aUnit, aValue);
		}

		Temperature(const Temperature<conversion_t>& aOther) :
			mValue(aOther.mValue)
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		SOLAIRE_UNITS_CONVERTER_COMMON(Temperature<conversion_t>)
		SOLAIRE_UNITS_PREFIXED_CONVERTER_COMMON(Temperature<conversion_t>, Metric<conversion_t>)
	};
}}


#endif