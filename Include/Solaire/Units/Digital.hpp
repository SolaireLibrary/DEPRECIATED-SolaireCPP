#ifndef SOLAIRE_UNITS_DIGITAL_HPP
#define SOLAIRE_UNITS_DIGITAL_HPP

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
#include "Digital.inl"

namespace Solaire{

	template<class VALUE>
	class DigitalConverter : public MetricPrefixConverter<DigitalInl::DigitalUnit, VALUE, DigitalInl::INTERMEDIARY_UNIT>
	{
	public:
        typedef MetricPrefixConverter<DigitalInl::DigitalUnit, VALUE, DigitalInl::INTERMEDIARY_UNIT> ParentClass;
        typedef typename ParentClass::PrefixType PrefixType;
        typedef typename ParentClass::UnitType UnitType;
        typedef typename ParentClass::ValueType ValueType;
        typedef MetricConverter<ValueType> PrefixConverterType;

	    template<const UnitType CONVERSION>
	    using DigitalProperty = UnitConverterProperty<DigitalConverter<ValueType>, CONVERSION>;

	    template<const PrefixType PREFIX, const UnitType CONVERSION>
        using DigitalPrefixProperty = PrefixConverterProperty<DigitalConverter<ValueType>, PREFIX, CONVERSION>;
	public:
		static constexpr ValueType StaticConvertToIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) / DigitalInl::GetScale(aUnit));
		}

		static constexpr ValueType StaticConvertFromIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) * DigitalInl::GetScale(aUnit));
		}
    public:
        DigitalConverter():
            ParentClass(static_cast<ValueType>(0)),
            Bits(*this)
        {}

        DigitalConverter(const ValueType aValue):
            ParentClass(aValue),
            Bits(*this)
        {}

        DigitalConverter(const DigitalConverter& aOther):
            ParentClass(aOther.Get()),
            Bits(*this)
        {}

        DigitalConverter(DigitalConverter&& aOther):
            ParentClass(aOther.Get()),
            Bits(*this)
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
			DigitalProperty<UnitType::BIT>				Bits;
			DigitalProperty<UnitType::NYBBLE>			Nybbles;
			DigitalProperty<UnitType::BYTE>			    Bytes;
			DigitalPrefixProperty<PrefixType::KIBI, UnitType::BIT>			Kibibits;
			DigitalPrefixProperty<PrefixType::MEBI, UnitType::BIT>			Mebibits;
			DigitalPrefixProperty<PrefixType::GIBI, UnitType::BIT>			Gibibits;
			DigitalPrefixProperty<PrefixType::TEBI, UnitType::BIT>			Tebibits;
			DigitalPrefixProperty<PrefixType::KIBI, UnitType::BYTE>			Kibibytes;
			DigitalPrefixProperty<PrefixType::MEBI, UnitType::BYTE>			Mebibytes;
			DigitalPrefixProperty<PrefixType::GIBI, UnitType::BYTE>			Gibibytes;
			DigitalPrefixProperty<PrefixType::TEBI, UnitType::BYTE>			Tebibytes;
			DigitalPrefixProperty<PrefixType::KILO, UnitType::BIT>			Kilobits;
			DigitalPrefixProperty<PrefixType::MEGA, UnitType::BIT>			Megabits;
			DigitalPrefixProperty<PrefixType::GIGA, UnitType::BIT>			Gigabits;
			DigitalPrefixProperty<PrefixType::TERA, UnitType::BIT>			Terabits;
			DigitalPrefixProperty<PrefixType::KILO, UnitType::BYTE>			Kilobytes;
			DigitalPrefixProperty<PrefixType::MEGA, UnitType::BYTE>			Megabytes;
			DigitalPrefixProperty<PrefixType::GIGA, UnitType::BYTE>			Gigabytes;
			DigitalPrefixProperty<PrefixType::TERA, UnitType::BYTE>			Terabytes;
		};
	};

	typedef DigitalConverter<double> DigitalConverterD;		//!< A Digital value that is a double.
	typedef DigitalConverter<float> DigitalConverterF;		//!< A Digital value that is a float.
	typedef DigitalConverter<int32_t> DigitalConverterI;	//!< A Digital value that is a signed 32 bit integer.
	typedef DigitalConverter<uint32_t> DigitalConverterU;	//!< A Digital value that is a unsigned 32 bit integer.
}


#endif
