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
#include "Temperature.inl"

namespace Solaire{

	template<class VALUE>
	class TemperatureConverter : public MetricPrefixConverter<TemperatureInl::TemperatureUnit, VALUE, TemperatureInl::INTERMEDIARY_UNIT>
	{
	public:
		typedef MetricPrefixConverter<TemperatureInl::TemperatureUnit, VALUE, TemperatureInl::INTERMEDIARY_UNIT> ParentClass;
        typedef typename ParentClass::PrefixType PrefixType;
        typedef typename ParentClass::UnitType UnitType;
        typedef typename ParentClass::ValueType ValueType;
        typedef MetricConverter<ValueType> PrefixConverterType;

	    template<const UnitType CONVERSION>
	    using TemperatureProperty = UnitConverterProperty<TemperatureConverter<ValueType>, CONVERSION>;

	    template<const PrefixType PREFIX, const UnitType CONVERSION>
        using TemperaturePrefixProperty = PrefixConverterProperty<TemperatureConverter<ValueType>, PREFIX, CONVERSION>;
	public:
		static constexpr ValueType StaticConvertToIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(TemperatureInl::ConvertToIntermediary(aUnit, static_cast<double>(aValue)));
		}

		static constexpr ValueType StaticConvertFromIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(TemperatureInl::ConvertFromIntermediary(aUnit, static_cast<double>(aValue)));
		}
    public:
        TemperatureConverter():
            ParentClass(static_cast<ValueType>(0)),
            Celcius(*this)
        {}

        TemperatureConverter(const ValueType aValue):
            ParentClass(aValue),
            Celcius(*this)
        {}

        TemperatureConverter(const TemperatureConverter& aOther):
            ParentClass(aOther.Get()),
            Celcius(*this)
        {}

        TemperatureConverter(TemperatureConverter&& aOther):
            ParentClass(aOther.Get()),
            Celcius(*this)
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
			TemperatureProperty<UnitType::CELCIUS>		    Celcius;
			TemperatureProperty<UnitType::FAHRENHEIT>		Fahrenheit;
			TemperatureProperty<UnitType::KELVIN>			Kelvin;
			TemperatureProperty<UnitType::GAS_MARK>		    GasMark;
		};
	};

	typedef TemperatureConverter<double> TemperatureConverterD;		//!< A Temperature value that is a double.
	typedef TemperatureConverter<float> TemperatureConverterF;		//!< A Temperature value that is a float.
	typedef TemperatureConverter<int32_t> TemperatureConverterI;	//!< A Temperature value that is a signed 32 bit integer.
	typedef TemperatureConverter<uint32_t> TemperatureConverterU;	//!< A Temperature value that is a unsigned 32 bit integer.
}


#endif
