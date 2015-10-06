#ifndef SOLAIRE_UNITS_BASE_CONVERTER_HPP
#define SOLAIRE_UNITS_BASE_CONVERTER_HPP

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
	\file BaseConverter.hpp
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

    template<class UNIT, class VALUE, const UNIT INTERMEDIATE>
    class UnitConverter{
    public:
        typedef UNIT UnitType;
        typedef VALUE ValueType;
    protected:
        ValueType mIntermediateValue;
    public:
        virtual ValueType ConvertToIntermediateUnit(const UnitType aUnit, const ValueType aValue) const = 0;
        virtual ValueType ConvertFromIntermediateUnit(const UnitType aUnit, const ValueType aValue) const = 0;

        UnitConverter(const ValueType aIntermediateValue):
            mIntermediateValue(aIntermediateValue)
        {}

        virtual ~UnitConverter(){

        }

        UnitType GetIntermediateUnit() const{
            return INTERMEDIATE;
        }

        ValueType Get() const{
            return mIntermediateValue;
        }

        void Set(const ValueType aValue) const{
            mIntermediateValue = aValue;
        }

        ValueType Get(const UnitType aUnit) const{
            return ConvertFromIntermediateUnit(aUnit, mIntermediateValue);
        }

        void Set(const UnitType aUnit, const ValueType aValue) const{
            mIntermediateValue = ConvertToIntermediateUnit(aUnit, aValue);
        }

        operator VALUE() const{
            return mIntermediateValue;
        }

        operator VALUE&(){
            return mIntermediateValue;
        }

        operator const VALUE&() const{
            return mIntermediateValue;
        }

        UnitConverter<UNIT, VALUE, INTERMEDIATE>& operator=(const VALUE aValue){
            mIntermediateValue = aValue;
        }

        UnitConverter<UNIT, VALUE, INTERMEDIATE>& operator+=(const VALUE aValue){
            mIntermediateValue += aValue;
            return *this;
        }

        UnitConverter<UNIT, VALUE, INTERMEDIATE>& operator-=(const VALUE aValue){
            mIntermediateValue -= aValue;
            return *this;
        }

        bool operator==(const VALUE aValue) const{
            return mIntermediateValue == aValue;
        }

        bool operator!=(const VALUE aValue) const{
            return mIntermediateValue != aValue;
        }

        bool operator<(const VALUE aValue) const{
            return mIntermediateValue < aValue;
        }

        bool operator>(const VALUE aValue) const{
            return mIntermediateValue > aValue;
        }

        bool operator<=(const VALUE aValue) const{
            return mIntermediateValue <= aValue;
        }

        bool operator>=(const VALUE aValue) const{
            return mIntermediateValue >= aValue;
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        UnitConverter<UNIT, VALUE, INTERMEDIATE>& operator=(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther){
            mIntermediateValue = INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
            return *this;
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        UnitConverter<UNIT, VALUE, INTERMEDIATE>& operator+=(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther){
            mIntermediateValue += INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
            return *this;
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        UnitConverter<UNIT, VALUE, INTERMEDIATE>& operator-=(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther){
            mIntermediateValue -= INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
            return *this;
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        bool operator==(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther) const{
            return mIntermediateValue == INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        bool operator!=(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther) const{
            return mIntermediateValue != INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        bool operator<(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther) const{
            return mIntermediateValue < INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        bool operator>(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther) const{
            return mIntermediateValue > INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        bool operator<=(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther) const{
            return mIntermediateValue <= INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
        }

        template<class VALUE2, const UNIT INTERMEDIATE2>
        bool operator>=(const UnitConverter<UNIT, VALUE2, INTERMEDIATE2>& aOther) const{
            return mIntermediateValue >= INTERMEDIATE == INTERMEDIATE2 ? aOther.mIntermediateValue : aOther.Get(INTERMEDIATE);
        }
    };

    template<class UNIT, class VALUE, const UNIT INTERMEDIATE, const UNIT CONVERSION>
    class UnitConverterProperty{
    private:
        UnitConverter<UNIT, VALUE, INTERMEDIATE>& mParent;
    public:
        constexpr UnitConverterProperty(UnitConverter<UNIT, VALUE, INTERMEDIATE>& aParent):
            mParent(aParent)
        {}

        inline operator VALUE() const{
            return mParent.Get(CONVERSION);
        }

        inline UnitConverter<UNIT, VALUE, INTERMEDIATE>& operator+=(const VALUE aValue){
            return mParent += mParent.ConvertToIntermediateUnit(CONVERSION, aValue);
        }

        inline UnitConverter<UNIT, VALUE, INTERMEDIATE>& operator-=(const VALUE aValue){
            return mParent -= mParent.ConvertToIntermediateUnit(CONVERSION, aValue);
        }
    };

}

#endif
