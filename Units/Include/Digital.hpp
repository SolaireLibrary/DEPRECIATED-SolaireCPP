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
#include "PrefixConverter.hpp"
#include "DefaultUnits.hpp"
#include "Implementation\Digital.inl"

namespace Solaire{namespace Units{

	template<class CONVERSION = double>
	class Digital : public PrefixConverter<Metric<CONVERSION>, DigitalUnit, CONVERSION>
	{
	private:
		void ConfigureProperties(){
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			Bytes.mParent = this;
#endif
		}
	protected:
		// Inherited from BaseConverter

		conversion_t ConvertToIntermediaryUnit(unit_t aUnit, conversion_t aValue) const override{
			return static_cast<conversion_t>(static_cast<double>(aValue) / DigitalInl::GetScale(aUnit));
		}

		conversion_t ConvertFromIntermediaryUnit(unit_t aUnit, conversion_t aValue) const override{
			return static_cast<conversion_t>(static_cast<double>(aValue) * DigitalInl::GetScale(aUnit));
		}

		unit_t GetIntermediaryUnit() const override{
			return INTERMEDIARY_UNIT;
		}
	public:
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
		union{
			ConverterProperty<Digital, unit_t::BIT>				Bits;
			ConverterProperty<Digital, unit_t::NYBBLE>			Nybbles;
			ConverterProperty<Digital, unit_t::BYTE>			Bytes;
			PrefixConverterProperty<Digital, prefix_t::KIBI, unit_t::BIT>			Kilobits;
			PrefixConverterProperty<Digital, prefix_t::MEBI, unit_t::BIT>			Megabits;
			PrefixConverterProperty<Digital, prefix_t::GIBI, unit_t::BIT>			Gigabits;
			PrefixConverterProperty<Digital, prefix_t::TEBI, unit_t::BIT>			Terabits;
			PrefixConverterProperty<Digital, prefix_t::KIBI, unit_t::BYTE>			Kilobytes;
			PrefixConverterProperty<Digital, prefix_t::MEBI, unit_t::BYTE>			Megabytes;
			PrefixConverterProperty<Digital, prefix_t::GIBI, unit_t::BYTE>			Gigabytes;
			PrefixConverterProperty<Digital, prefix_t::TEBI, unit_t::BYTE>			Terabytes;
		};
#endif

		static const unit_t INTERMEDIARY_UNIT = DigitalInl::INTERMEDIARY_UNIT;

		// Constructors

		Digital() : 
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
		}

		Digital(conversion_t aValue) : 
			PrefixConverter(aValue)
		{

		}

		Digital(unit_t aUnit, conversion_t aValue) : 
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
			Set(aUnit, aValue);
		}

		Digital(prefix_t aPrefix, unit_t aUnit, conversion_t aValue) :
			PrefixConverter(static_cast<conversion_t>(0.0))
		{
			ConfigureProperties();
			Set(aPrefix, aUnit, aValue);
		}

		Digital(const BaseConverter<unit_t, conversion_t>& aOther) :
			PrefixConverter(aOther.Get(GetIntermediaryUnit()))
		{
			ConfigureProperties();
		}
	};
}}


#endif