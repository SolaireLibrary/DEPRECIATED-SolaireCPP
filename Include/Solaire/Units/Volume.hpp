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

#include <cstdint>
#include "BaseConverter.hpp"
#include "Metric.hpp"
#include "PrefixConverter.hpp"
#include "Volume.inl"

namespace Solaire{ namespace Units{

	template<class CONVERSION = double>
	class Volume
	{
	public:
		typedef VolumeInl::VolumeUnit unit_t;
		typedef CONVERSION conversion_t;
		typedef typename MetricD::unit_t prefix_t;
	private:
		conversion_t mValue;

		static constexpr conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) / VolumeInl::GetScale(aUnit));
		}

		static constexpr conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) * VolumeInl::GetScale(aUnit));
		}
	public:
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
		union{
			Volume* const Self;
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

		// Constructors

		constexpr Volume() :
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Volume(const conversion_t aValue) :
			mValue(aValue)
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Volume(const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aUnit, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Volume(const prefix_t aPrefix, const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aPrefix, aUnit, MetricD::unit_t::NONE, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Volume(const Volume<conversion_t>& aOther) :
			mValue(aOther.mValue)
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		SOLAIRE_UNITS_CONVERTER_COMMON(Volume<conversion_t>)
		SOLAIRE_UNITS_PREFIXED_CONVERTER_COMMON(Volume<conversion_t>, Metric<conversion_t>)
	};

	typedef Volume<double> VolumeD;		//!< A volume value that is a double.
	typedef Volume<float> VolumeF;		//!< A volume value that is a float.
	typedef Volume<int32_t> VolumeI;	//!< A volume value that is a signed 32 bit integer.
	typedef Volume<uint32_t> VolumeU;	//!< A volume value that is a unsigned 32 bit integer.
}}

#endif
