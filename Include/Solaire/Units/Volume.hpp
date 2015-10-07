#ifndef SOLAIRE_UNITS_VOLUME_HPP
#define SOLAIRE_UNITS_VOLUME_HPP

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
#include "Volume.inl"

namespace Solaire{

	template<class VALUE>
	class VolumeConverter : public MetricPrefixConverter<VolumeInl::VolumeUnit, VALUE, VolumeInl::INTERMEDIARY_UNIT>
	{
	public:
 typedef MetricPrefixConverter<VolumeInl::VolumeUnit, VALUE, VolumeInl::INTERMEDIARY_UNIT> ParentClass;
        typedef typename ParentClass::PrefixType PrefixType;
        typedef typename ParentClass::UnitType UnitType;
        typedef typename ParentClass::ValueType ValueType;
        typedef MetricConverter<ValueType> PrefixConverterType;

	    template<const UnitType CONVERSION>
	    using VolumeProperty = UnitConverterProperty<VolumeConverter<ValueType>, CONVERSION>;

	    template<const PrefixType PREFIX, const UnitType CONVERSION>
        using VolumePrefixProperty = PrefixConverterProperty<VolumeConverter<ValueType>, PREFIX, CONVERSION>;
	public:
		static constexpr ValueType StaticConvertToIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) / VolumeInl::GetScale(aUnit));
		}

		static constexpr ValueType StaticConvertFromIntermediaryUnit(const UnitType aUnit, const ValueType aValue){
			return static_cast<ValueType>(static_cast<double>(aValue) * VolumeInl::GetScale(aUnit));
		}
    public:
        VolumeConverter():
            ParentClass(static_cast<ValueType>(0)),
            Litres(*this)
        {}

        VolumeConverter(const ValueType aValue):
            ParentClass(aValue),
            Litres(*this)
        {}

        VolumeConverter(const VolumeConverter& aOther):
            ParentClass(aOther.Get()),
            Litres(*this)
        {}

        VolumeConverter(VolumeConverter&& aOther):
            ParentClass(aOther.Get()),
            Litres(*this)
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
			VolumeProperty<UnitType::LITRE>							    Litres;
			VolumeProperty<UnitType::PINT_UK>							PintsUK;
			VolumeProperty<UnitType::PINT_US>							PintsUS;
			VolumeProperty<UnitType::GALLON_UK>						    GallonsUK;
			VolumeProperty<UnitType::GALLON_US>						    GallonsUS;
			VolumeProperty<UnitType::CUP_US>							CupsUS;
			VolumeProperty<UnitType::OUNCE_UK>							FluidOuncesUK;
			VolumeProperty<UnitType::OUNCE_US>							FluidOuncesUS;
			VolumeProperty<UnitType::TABLE_SPOON_UK>					TableSpoonsUK;
			VolumeProperty<UnitType::TABLE_SPOON_US>					TableSpoonsUS;
			VolumeProperty<UnitType::TEA_SPOON_UK>						TeaSpoonsUK;
			VolumeProperty<UnitType::TEA_SPOON_US>						TeaSpoonsUS;
			VolumeProperty<UnitType::QUART_UK>							QuartsUK;
			VolumeProperty<UnitType::QUART_US>							QuartsUS;
			VolumePrefixProperty<PrefixType::CENTI, UnitType::LITRE>	Centilitres;
			VolumePrefixProperty<PrefixType::MILLI, UnitType::LITRE>	Millilitres;
		};
	};

	typedef VolumeConverter<double> VolumeConverterD;		//!< A volume value that is a double.
	typedef VolumeConverter<float> VolumeConverterF;		//!< A volume value that is a float.
	typedef VolumeConverter<int32_t> VolumeConverterI;	    //!< A volume value that is a signed 32 bit integer.
	typedef VolumeConverter<uint32_t> VolumeConverterU;	    //!< A volume value that is a unsigned 32 bit integer.
}

#endif
