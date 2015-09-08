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

#include "BaseConverter.hpp"
#include "Metric.hpp"
#include "PrefixConverter.hpp"
#include "DefaultUnits.hpp"
#include "Implementation\Temperature.inl"

namespace Solaire{ namespace Units{

	template<class CONVERSION = double>
	class Temperature : public PrefixConverter<Metric<CONVERSION>, TemperatureUnit, CONVERSION>
	{
	private:
		void ConfigureProperties(){
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			Celcius.mParent = this;
#endif
		}
	protected:
		// Inherited from BaseConverter

		conversion_t ConvertToIntermediaryUnit(unit_t aUnit, conversion_t aValue) const override{
			return static_cast<conversion_t>(TemperatureInl::ConvertToIntermediary(aUnit, static_cast<double>(aValue)));
		}

		conversion_t ConvertFromIntermediaryUnit(unit_t aUnit, conversion_t aValue) const override{
			return static_cast<conversion_t>(TemperatureInl::ConvertFromIntermediary(aUnit, static_cast<double>(aValue)));
		}

		unit_t GetIntermediaryUnit() const override{
			return INTERMEDIARY_UNIT;
		}
	public:
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
		union{
			ConverterProperty<Temperature, unit_t::CELCIUS>						Celcius;
			ConverterProperty<Temperature, unit_t::FAHRENHEIT>					Fahrenheit;
			ConverterProperty<Temperature, unit_t::KELVIN>						Kelvin;
			ConverterProperty<Temperature, unit_t::GAS_MARK>					GasMark;
		};
#endif

		static const unit_t INTERMEDIARY_UNIT = TemperatureInl::INTERMEDIARY_UNIT;

		// Constructors

		Temperature() : 
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
		}

		Temperature(conversion_t aValue) : 
			PrefixConverter(aValue)
		{

		}

		Temperature(unit_t aUnit, conversion_t aValue) : 
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
			Set(aUnit, aValue);
		}

		Temperature(prefix_t aPrefix, unit_t aUnit, conversion_t aValue) :
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
			Set(aPrefix, aUnit, aValue);
		}

		Temperature(const BaseConverter<unit_t, conversion_t>& aOther) :
			PrefixConverter(aOther.Get(GetIntermediaryUnit()))
		{
			ConfigureProperties();
		}
	};
}}


#endif