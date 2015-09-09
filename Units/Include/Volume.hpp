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
#include "PrefixConverter.hpp"
#include "DefaultUnits.hpp"
#include "Implementation\Volume.inl"

namespace Solaire{ namespace Units{

	template<class CONVERSION = double>
	class Volume : public PrefixConverter<Metric<CONVERSION>, VolumeUnit, CONVERSION>
	{
	private:
		void ConfigureProperties(){
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			Litres.mParent = this;
#endif
		}
	protected:
		// Inherited from BaseConverter

		conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const override{
			return static_cast<conversion_t>(static_cast<double>(aValue) / VolumeInl::GetScale(aUnit));
		}

		conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const override{
			return static_cast<conversion_t>(static_cast<double>(aValue) * VolumeInl::GetScale(aUnit));
		}

		unit_t GetIntermediaryUnit() const override{
			return INTERMEDIARY_UNIT;
		}
	public:
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
		union{
			ConverterProperty<Volume, unit_t::LITRE>							Litres;
			ConverterProperty<Volume, unit_t::PINT_UK>							PintsUK;
			ConverterProperty<Volume, unit_t::PINT_US>							PintsUS;
			ConverterProperty<Volume, unit_t::GALLON_UK>						GallonsUK;
			ConverterProperty<Volume, unit_t::GALLON_US>						GallonsUS;
			ConverterProperty<Volume, unit_t::CUP_US>							CupsUS;
			ConverterProperty<Volume, unit_t::OUNCE_UK>							FluidOuncesUK;
			ConverterProperty<Volume, unit_t::OUNCE_US>							FluidOuncesUS;
			ConverterProperty<Volume, unit_t::TABLE_SPOON_UK>					TableSpoonsUK;
			ConverterProperty<Volume, unit_t::TABLE_SPOON_US>					TableSpoonsUS;
			ConverterProperty<Volume, unit_t::TEA_SPOON_UK>						TeaSpoonsUK;
			ConverterProperty<Volume, unit_t::TEA_SPOON_US>						TeaSpoonsUS;
			ConverterProperty<Volume, unit_t::QUART_UK>							QuartsUK;
			ConverterProperty<Volume, unit_t::QUART_US>							QuartsUS;
			PrefixConverterProperty<Volume, prefix_t::CENTI, unit_t::LITRE>		Centilitres;
			PrefixConverterProperty<Volume, prefix_t::MILLI, unit_t::LITRE>		Millilitres;
		};
#endif

		static constexpr unit_t INTERMEDIARY_UNIT = VolumeInl::INTERMEDIARY_UNIT;

		static constexpr conversion_t Convert(const unit_t aInput, const unit_t aOutput, const conversion_t aValue){
			return static_cast<conversion_t>(
				(static_cast<double>(aValue) / VolumeInl::GetScale(aInput)) * 
				VolumeInl::GetScale(aOutput)
			);
		}

		// Constructors

		Volume() : 
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
		}

		Volume(conversion_t aValue) : 
			PrefixConverter(aValue)
		{

		}

		Volume(unit_t aUnit, conversion_t aValue) : 
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
			Set(aUnit, aValue);
		}

		Volume(prefix_t aPrefix, unit_t aUnit, conversion_t aValue) :
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
			Set(aPrefix, aUnit, aValue);
		}

		Volume(const BaseConverter<unit_t, conversion_t>& aOther) :
			PrefixConverter(aOther.Get(GetIntermediaryUnit()))
		{
			ConfigureProperties();
		}
	};
}}

#endif