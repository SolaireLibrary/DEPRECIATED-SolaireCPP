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
#include "Metric.inl"

namespace Solaire{ namespace Units{
	template<class CONVERSION = double>
	class Metric
	{
	public:
		typedef MetricInl::MetricPrefix unit_t;
		typedef CONVERSION conversion_t;
	private:
		conversion_t mValue;

		static constexpr conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) * MetricInl::GetScale(aUnit));
		}

		static constexpr conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue){
			return static_cast<conversion_t>(static_cast<double>(aValue) / MetricInl::GetScale(aUnit));
		}
	public:
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
		union{
			Metric* const Self;
			ConverterProperty<Metric, unit_t::NONE>		Value;
			ConverterProperty<Metric, unit_t::YOTTA>	Yotta;
			ConverterProperty<Metric, unit_t::ZETTA>	Zetta;
			ConverterProperty<Metric, unit_t::EXA>		Exa;
			ConverterProperty<Metric, unit_t::PETA>		Peta;
			ConverterProperty<Metric, unit_t::TERA>		Tera;
			ConverterProperty<Metric, unit_t::GIGA>		Giga;
			ConverterProperty<Metric, unit_t::MEGA>		Mega;
			ConverterProperty<Metric, unit_t::KILO>		Kilo;
			ConverterProperty<Metric, unit_t::HECTO>	Hecto;
			ConverterProperty<Metric, unit_t::DECA>		Deca;
			ConverterProperty<Metric, unit_t::DECI>		Deci;
			ConverterProperty<Metric, unit_t::CENTI>	Centi;
			ConverterProperty<Metric, unit_t::MILLI>	Milli;
			ConverterProperty<Metric, unit_t::MICRO>	Micro;
			ConverterProperty<Metric, unit_t::NANO>		Nano;
			ConverterProperty<Metric, unit_t::PICO>		Pico;
			ConverterProperty<Metric, unit_t::FEMTO>	Femto;
			ConverterProperty<Metric, unit_t::ATTO>		Atto;
			ConverterProperty<Metric, unit_t::ZEPTO>	Zepto;
			ConverterProperty<Metric, unit_t::YOCTO>	Yocto;
			ConverterProperty<Metric, unit_t::KIBI>		Kibi;
			ConverterProperty<Metric, unit_t::MEBI>		Mebi;
			ConverterProperty<Metric, unit_t::GIBI>		Gibi;
			ConverterProperty<Metric, unit_t::TEBI>		Tebi;
			ConverterProperty<Metric, unit_t::PEBI>		Pebi;
			ConverterProperty<Metric, unit_t::EXBI>		Exbi;
			ConverterProperty<Metric, unit_t::ZEBI>		Zebi;
			ConverterProperty<Metric, unit_t::YOBI>		Yobi;
		};
#endif
		static constexpr unit_t INTERMEDIARY_UNIT = MetricInl::INTERMEDIARY_UNIT;

		// Constructors

		constexpr Metric() :
			mValue(static_cast<conversion_t>(0.0))
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Metric(const conversion_t aValue) :
			mValue(aValue)
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Metric(const unit_t aUnit, const conversion_t aValue) :
			mValue(Convert(aUnit, INTERMEDIARY_UNIT, aValue))
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		constexpr Metric(const Metric<conversion_t>& aOther) :
			mValue(aOther.mValue)
#ifndef SOLAIRE_DISABLE_CONVERTER_PROPERTIES
			, Self(this)
#endif
		{

		}

		SOLAIRE_UNITS_CONVERTER_COMMON(Metric<conversion_t>)
	};

	typedef Metric<double> MetricD;		//!< A Metric value that is a double.
	typedef Metric<float> MetricF;		//!< A Metric value that is a float.
	typedef Metric<int32_t> MetricI;	//!< A Metric value that is a signed 32 bit integer.
	typedef Metric<uint32_t> MetricU;	//!< A Metric value that is a unsigned 32 bit integer.
}}


#endif
