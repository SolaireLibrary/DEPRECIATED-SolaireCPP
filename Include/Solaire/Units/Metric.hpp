#ifndef SOLAIRE_UNITS_METRIC_HPP
#define SOLAIRE_UNITS_METRIC_HPP

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
#include "PrefixConverter.hpp"
#include "Metric.inl"

namespace Solaire{

	template<class VALUE>
	class MetricConverter : public UnitConverter<MetricInl::MetricPrefix, VALUE, MetricInl::INTERMEDIARY_UNIT>{
    public:
        typedef typename UnitConverter<MetricInl::MetricPrefix, VALUE, MetricInl::INTERMEDIARY_UNIT>::UnitType UnitType;
        typedef typename UnitConverter<MetricInl::MetricPrefix, VALUE, MetricInl::INTERMEDIARY_UNIT>::ValueType ValueType;
        typedef UnitConverter<MetricInl::MetricPrefix, VALUE, MetricInl::INTERMEDIARY_UNIT> ParentClass;

	    template<const UnitType CONVERSION>
	    using MetricProperty = UnitConverterProperty<MetricConverter<ValueType>, CONVERSION>;
	public:
		static constexpr ValueType StaticConvertToIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) * MetricInl::GetScale(aUnit));
		}

		static constexpr ValueType StaticConvertFromIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) / MetricInl::GetScale(aUnit));
		}
    public:
        MetricConverter():
            ParentClass(static_cast<ValueType>(0)),
            Value(*this)
        {}

        MetricConverter(const ValueType aValue):
            ParentClass(aValue),
            Value(*this)
        {}

        MetricConverter(const MetricConverter& aOther):
            ParentClass(aOther.Get()),
            Value(*this)
        {}

        MetricConverter(MetricConverter&& aOther):
            ParentClass(aOther.Get()),
            Value(*this)
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
			MetricProperty<UnitType::NONE>		Value;
			MetricProperty<UnitType::YOTTA>	    Yotta;
			MetricProperty<UnitType::ZETTA>	    Zetta;
			MetricProperty<UnitType::EXA>		Exa;
			MetricProperty<UnitType::PETA>		Peta;
			MetricProperty<UnitType::TERA>		Tera;
			MetricProperty<UnitType::GIGA>		Giga;
			MetricProperty<UnitType::MEGA>		Mega;
			MetricProperty<UnitType::KILO>		Kilo;
			MetricProperty<UnitType::HECTO>	    Hecto;
			MetricProperty<UnitType::DECA>		Deca;
			MetricProperty<UnitType::DECI>		Deci;
			MetricProperty<UnitType::CENTI>	    Centi;
			MetricProperty<UnitType::MILLI>	    Milli;
			MetricProperty<UnitType::MICRO>	    Micro;
			MetricProperty<UnitType::NANO>		Nano;
			MetricProperty<UnitType::PICO>		Pico;
			MetricProperty<UnitType::FEMTO>	    Femto;
			MetricProperty<UnitType::ATTO>		Atto;
			MetricProperty<UnitType::ZEPTO>	    Zepto;
			MetricProperty<UnitType::YOCTO>	    Yocto;
			MetricProperty<UnitType::KIBI>		Kibi;
			MetricProperty<UnitType::MEBI>		Mebi;
			MetricProperty<UnitType::GIBI>		Gibi;
			MetricProperty<UnitType::TEBI>		Tebi;
			MetricProperty<UnitType::PEBI>		Pebi;
			MetricProperty<UnitType::EXBI>		Exbi;
			MetricProperty<UnitType::ZEBI>		Zebi;
			MetricProperty<UnitType::YOBI>		Yobi;
		};
	};

	typedef MetricConverter<double> MetricConverterD;	//!< A Metric value that is a double.
	typedef MetricConverter<float> MetricConverterF;	//!< A Metric value that is a float.
	typedef MetricConverter<int32_t> MetricConverterI;	//!< A Metric value that is a signed 32 bit integer.
	typedef MetricConverter<uint32_t> MetricConverterU;	//!< A Metric value that is a unsigned 32 bit integer.

    template<class UNIT, class VALUE, const UNIT INTERMEDIATE>
    using MetricPrefixConverter = PrefixConverter<UNIT, VALUE, INTERMEDIATE, MetricConverter<VALUE>>;
}


#endif
