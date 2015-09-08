#ifndef SOLAIRE_UNITS_PREFIX_CONVERTER_HPP
#define SOLAIRE_UNITS_PREFIX_CONVERTER_HPP

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

namespace Solaire{ namespace Units{

	template<class PREFIX_CONVERTER, typename UNIT, typename CONVERSION>
	class PrefixConverter : public BaseConverter<UNIT, CONVERSION>
	{
	public:
		typedef typename PREFIX_CONVERTER::unit_t prefix_t;

		PrefixConverter(conversion_t aValue) :
			BaseConverter(aValue)
		{

		}

		virtual ~PrefixConverter(){

		}

		conversion_t PrefixedGet(prefix_t aPrefix, unit_t aUnit) const{
			return Convert<PREFIX_CONVERTER>(PREFIX_CONVERTER::INTERMEDIARY_UNIT, aPrefix, Get(aUnit));
		}

		void PrefixedSet(prefix_t aPrefix, unit_t aUnit, conversion_t aValue){
			Set(aUnit, Convert<PREFIX_CONVERTER>>(aPrefix, PREFIX_CONVERTER::INTERMEDIARY_UNIT, aValue));
		}
	};

#ifndef SOLAIRE_UNITS_NO_PROPERTIES
	template<class CONVERTER, typename CONVERTER::prefix_t PREFIX, typename CONVERTER::unit_t UNIT>
	class PrefixConverterProperty{
	private:
		CONVERTER* mParent;
	public:
		friend CONVERTER;

		void operator=(typename CONVERTER::conversion_t aValue){
			return mParent->PrefixedSet(PREFIX, UNIT, aValue);
		}

		operator typename CONVERTER::conversion_t() const{
			return mParent->PrefixedGet(PREFIX, UNIT);
		}

		void operator+=(typename CONVERTER::conversion_t aValue){
			mParent->PrefixedSet(PREFIX, UNIT, mParent->PrefixedGet(PREFIX, UNIT) + aValue);
		}

		void operator-=(typename CONVERTER::conversion_t aValue){
			mParent->PrefixedSet(PREFIX, UNIT, mParent->PrefixedGetPREFIX, (UNIT) - aValue);
		}

		void operator*=(typename CONVERTER::conversion_t aValue){
			mParent->PrefixedSet(PREFIX, UNIT, mParent->PrefixedGetPREFIX, (UNIT) * aValue);
		}

		void operator/=(typename CONVERTER::conversion_t aValue){
			mParent->PrefixedSet(PREFIX, UNIT, mParent->PrefixedGet(PREFIX, UNIT) / aValue);
		}
	};
#endif
}}


#endif