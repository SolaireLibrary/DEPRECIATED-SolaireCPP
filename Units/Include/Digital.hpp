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

#include <cstdint>
#include "BaseConverter.hpp"
#include "Metric.hpp"
#include "PrefixConverter.hpp"
#include "Implementation\Digital.inl"

namespace Solaire{namespace Units{

	template<class CONVERSION = double>
	class Digital
	{
	public:
		typedef DigitalInl::DigitalUnit unit_t;
		typedef CONVERSION conversion_t;
		typedef typename MetricD::unit_t prefix_t;
	private:
		conversion_t mValue;

		static constexpr conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) / DigitalInl::GetScale(aUnit));
		}

		static constexpr conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) * DigitalInl::GetScale(aUnit));
		}
	public:
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
		union{
			Digital* Self;
			ConverterProperty<Digital, unit_t::BIT>				Bits;
			ConverterProperty<Digital, unit_t::NYBBLE>			Nybbles;
			ConverterProperty<Digital, unit_t::BYTE>			Bytes;
			PrefixConverterProperty<Digital, prefix_t::KIBI, unit_t::BIT>			Kibibits;
			PrefixConverterProperty<Digital, prefix_t::MEBI, unit_t::BIT>			Mebibits;
			PrefixConverterProperty<Digital, prefix_t::GIBI, unit_t::BIT>			Gibibits;
			PrefixConverterProperty<Digital, prefix_t::TEBI, unit_t::BIT>			Tebibits;
			PrefixConverterProperty<Digital, prefix_t::KIBI, unit_t::BYTE>			Kibibytes;
			PrefixConverterProperty<Digital, prefix_t::MEBI, unit_t::BYTE>			Mebibytes;
			PrefixConverterProperty<Digital, prefix_t::GIBI, unit_t::BYTE>			Gibibytes;
			PrefixConverterProperty<Digital, prefix_t::TEBI, unit_t::BYTE>			Tebibytes;
			PrefixConverterProperty<Digital, prefix_t::KILO, unit_t::BIT>			Kilobits;
			PrefixConverterProperty<Digital, prefix_t::MEGA, unit_t::BIT>			Megabits;
			PrefixConverterProperty<Digital, prefix_t::GIGA, unit_t::BIT>			Gigabits;
			PrefixConverterProperty<Digital, prefix_t::TERA, unit_t::BIT>			Terabits;
			PrefixConverterProperty<Digital, prefix_t::KILO, unit_t::BYTE>			Kilobytes;
			PrefixConverterProperty<Digital, prefix_t::MEGA, unit_t::BYTE>			Megabytes;
			PrefixConverterProperty<Digital, prefix_t::GIGA, unit_t::BYTE>			Gigabytes;
			PrefixConverterProperty<Digital, prefix_t::TERA, unit_t::BYTE>			Terabytes;
		};
#endif

		static constexpr unit_t INTERMEDIARY_UNIT = DigitalInl::INTERMEDIARY_UNIT;

		// Constructors

		constexpr Digital() :
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Digital(const conversion_t aValue) :
			mValue(aValue)
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Digital(const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aUnit, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Digital(const prefix_t aPrefix, const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aPrefix, aUnit, MetricD::unit_t::NONE, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Digital(const Digital<conversion_t>& aOther) :
			mValue(aOther.mValue)
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
			, Self(this)
#endif
		{

		}

		SOLAIRE_UNITS_CONVERTER_COMMON(Digital<conversion_t>)
		SOLAIRE_UNITS_PREFIXED_CONVERTER_COMMON(Digital<conversion_t>, Metric<conversion_t>)
	};

	typedef Digital<double> DigitalD;		//!< A Digital value that is a double.
	typedef Digital<float> DigitalF;		//!< A Digital value that is a float.
	typedef Digital<int32_t> DigitalI;		//!< A Digital value that is a signed 32 bit integer.
	typedef Digital<uint32_t> DigitalU;		//!< A Digital value that is a unsigned 32 bit integer.
}}


#endif
