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

/*!
	\file PrefixConverter.hpp
	\brief Contains the code for BaseConverter and ConverterProperty
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 3.0
	\date
	Created			: 6th September 2015
	Last Modified	: 6th October 2015
*/

namespace Solaire{

    template<class UNIT, class VALUE, const UNIT INTERMEDIATE, class PREFIX_CONVERTER>
    class PrefixConverter : public UnitConverter<UNIT, VALUE, INTERMEDIATE>{
    public:
        typedef typename PREFIX_CONVERTER::UnitType PrefixType;
        typedef UNIT UnitType;
        typedef VALUE ValueType;
        typedef PREFIX_CONVERTER PrefixConverterType;
        typedef UnitConverter<UNIT, VALUE, INTERMEDIATE> ParentClass;
    public:
        PrefixConverter(const ValueType aIntermediateValue):
            ParentClass(aIntermediateValue)
        {}

        virtual ~PrefixConverter(){

        }

        ValueType GetPrefixed(const PrefixType aPrefix, const UnitType aUnit) const{
            return PREFIX_CONVERTER::StaticConvertFromIntermediateUnit(aPrefix, Get(aUnit));
        }

        void SetPrefixed(const PrefixType aPrefix, const UnitType aUnit, const ValueType aValue) const{
            Set(aUnit, PREFIX_CONVERTER::StaticConvertToIntermediateUnit(aPrefix, aValue));
        }
    };

    template<class CONVERTER, const typename CONVERTER::PrefixType PREFIX, const typename CONVERTER::UnitType CONVERSION>
    class PrefixConverterProperty{
    public:
        typedef typename CONVERTER::ValueType ValueType;
        typedef typename CONVERTER::PrefixConverterType PrefixConverterType;
    private:
        CONVERTER& mParent;
    public:
        constexpr PrefixConverterProperty(CONVERTER& aParent):
            mParent(aParent)
        {}

        inline operator ValueType() const{
            return mParent.GetPrefixed(PREFIX, CONVERSION);
        }

        inline CONVERTER& operator+=(const ValueType aValue){
            return mParent += CONVERTER::StaticConvertToIntermediateUnit(CONVERSION, PrefixConverterType::StaticConvertToIntermediateUnit(PREFIX, aValue));
        }

        inline CONVERTER& operator-=(const ValueType aValue){
            return mParent -= CONVERTER::StaticConvertToIntermediateUnit(CONVERSION, PrefixConverterType::StaticConvertToIntermediateUnit(PREFIX, aValue));
        }
    };

}

#endif
