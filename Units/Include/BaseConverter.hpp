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
	\version 2.0
	\date 
	Created			: 6th September 2015
	Last Modified	: 7th September 2015
*/

namespace Solaire{ namespace Units{

	/*!
		\class BaseConverter
		\tparam UNIT The type, usually an enum that identifies the unit(s) to convert between.
		\tparam CONVERSION The data type of converted units.
		\brief Provides basic functionality for implementaion of simple unit conversions using an intermediate unit for storage.
		\detail
		\version 2.0
		\author Adam Smith
	*/
	template<class UNIT, class CONVERSION>
	class BaseConverter
	{
	public:
		typedef UNIT unit_t;				//!< The type that identifies which conversion(s) to make.
		typedef CONVERSION conversion_t;	//!< The data type that converted values are stored in.

		/*!
			\brief Create a new BaseConverter.
			\param aValue The initial value stored in the converter.
		*/
		BaseConverter(const conversion_t aValue) :
			mValue(aValue)
		{

		}

		/*!
			\brief Virtual destructor, currently does nothing.
		*/
		virtual ~BaseConverter(){

		}

		/*!
			\brief Convert the stored intermediary value to the requested unit format.
			\param aUnit The unit to convert into.
			\return The stored value represented in the converted unit.
		*/
		conversion_t Get(const unit_t aUnit) const{
			return ConvertFromIntermediaryUnit(aUnit, mValue);	
		}

		/*!
			\brief Set the intermediary.
			\param aUnit The unit format of \a aValue.
			\param aValue The value that will be converted to be the new intermediary value.
		*/
		void Set(const unit_t aUnit, const conversion_t aValue){
			mValue = ConvertToIntermediaryUnit(aUnit, aValue) ;
		}

		/*!
			\brief Compare two converters to check if their intermediary values are the same.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter to compare.
			\return True if the values are the same, else false.
		*/
		bool operator==(const BaseConverter<unit_t, conversion_t>& aOther) const{
			return mValue == aOther.Get(GetIntermediaryUnit());
		}
		
		/*!
			\brief Compare two converters to check if their intermediary values are the different.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter to compare.
			\return True if the values are different, else false.
		*/
		bool operator!=(const BaseConverter<unit_t, conversion_t>& aOther) const{
			return mValue != aOther.Get(GetIntermediaryUnit());
		}
		
		/*!
			\brief Compare two converters to check if the intermediary value of the first is less than the second's.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter to compare.
			\return True if the this converter's value is smaller, else false.
		*/
		bool operator<(const BaseConverter<unit_t, conversion_t>& aOther) const{
			return mValue < aOther.Get(GetIntermediaryUnit());
		}
		
		/*!
			\brief Compare two converters to check if the intermediary value of the first is greater than the second's.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter to compare.
			\return True if the this converter's value is larger, else false.
		*/
		bool operator>(const BaseConverter<unit_t, conversion_t>& aOther) const{
			return mValue > aOther.Get(GetIntermediaryUnit());
		}
		
		/*!
			\brief Compare two converters to check if the intermediary value of the first is less than or equal to the second's.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter to compare.
			\return True if the this converter's value is smaller or equal, else false.
		*/
		bool operator<=(const BaseConverter<unit_t, conversion_t>& aOther) const{
			return mValue <= aOther.Get(GetIntermediaryUnit());
		}
		
		/*!
			\brief Compare two converters to check if the intermediary value of the first is greater than or equal to the second's.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter to compare.
			\return True if the this converter's value is larger or equal, else false.
		*/
		bool operator>=(const BaseConverter<unit_t, conversion_t>& aOther) const{
			return mValue >= aOther.Get(GetIntermediaryUnit());
		}

		/*!
			\brief Add the intermediary value of another compatible converter to this converter's intermediary value.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter.
			\return A reference to this converter.
		*/
		BaseConverter<unit_t, conversion_t>& operator+=(const BaseConverter<unit_t, conversion_t>& aOther){
			mValue += aOther.Get(GetIntermediaryUnit());
			return *this;
		}
		

		/*!
			\brief Subtract the intermediary value of another compatible converter from this converter's intermediary value.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter.
			\return A reference to this converter.
		*/
		BaseConverter<unit_t, conversion_t>& operator-=(const BaseConverter<unit_t, conversion_t>& aOther){
			mValue -= aOther.Get(GetIntermediaryUnit());
			return *this;
		}
		
		/*!
			\brief Multiply this converter's intermediary value by the intermediary value of another compatible converter.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter.
			\return A reference to this converter.
		*/
		BaseConverter<unit_t, conversion_t>& operator*=(const BaseConverter<unit_t, conversion_t>& aOther){
			mValue *= aOther.Get(GetIntermediaryUnit());
			return *this;
		}
		
		/*!
			\brief Divide this converter's intermediary value by the intermediary value of another compatible converter.
			\detail The intermediary value of \a aOther is automatically converted into the intermediary unit of this converter.
			\param aOther A reference to the converter.
			\return A reference to this converter.
		*/
		BaseConverter<unit_t, conversion_t>& operator/=(const BaseConverter<unit_t, conversion_t>& aOther){
			mValue /= aOther.Get(GetIntermediaryUnit());
			return *this;
		}
	protected:
		/*!
			\brief Return the unit identifer that is used to store the intermediary value in this converter.
			\return The intermediary unit identifier.
		*/
		virtual unit_t GetIntermediaryUnit() const = 0;

		/*!
			\brief Convert a value from one unit format to the current intermediary unit.
			\param aUnit The unit format to convert from.
			\param aValue The value to convert.
			\return The value of \a aValue when converted into the intermediary unit
			\see GetIntermediaryUnit
		*/
		virtual conversion_t ConvertToIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const = 0;

		/*!
			\brief Convert a value from the current intermediary unit into a different unit.
			\param aUnit The unit format to convert into.
			\param aValue The value to convert.
			\return The value of \a aValue when converted into the requested unit
			\see GetIntermediaryUnit
		*/
		virtual conversion_t ConvertFromIntermediaryUnit(const unit_t aUnit, const conversion_t aValue) const = 0;
	private:
		conversion_t mValue;	//!< The intermediary value.
	};
	
#ifndef SOLAIRE_UNITS_NO_PROPERTIES
	/*!
		\class ConverterProperty
		\tparam CONVERTER The concrete class that defines this property, used to allow access to the mParent pointer.
		\tparam UNIT The unit that this property should convert to / from.
		\version 1.0
		\author Adam Smith
		\brief Allows for a unit to be represented to the programmer as a named variable, instead of using Get / Set.
		\detail 
		Properties are designed so they can be placed in a union to save memory, their paramiters should be defined by templates.
		Properties can be disabled by defineing the macro SOLAIRE_UNITS_NO_PROPERTIES, this will reduce the size of each BaseConverter object by the size of a pointer.

		Code in the Converter class:
		\code
		ConverterProperty<Distance, unit_t::METRE>	Metres;
		ConverterProperty<Distance, unit_t::FOOT>	Feet;
		\endcode
		There are now two ways to convert values using an object of this type.
		The first is the old way:
		\code
		Distance<double> converter;
		converter.Set(converter::unit_t::METRE, 5.0);
		double valueInFeet = converter.Get(converter::unit_t::FOOT);
		\endcode
		The second is the new way, using the properties:
		\code
		Distance<double> converter;
		converter.Metres = 5.0;
		double valueInFeet = converter.Feet;
		\endcode
		While these two methods are similar, the second version is much faster to type and easier to read. This makes the conversion code less error prone, and easier to develop with.
		
		Properties can also be used with the basic maths operators:
		\code
		Distance<double> converter;
		converter.Metres = 5.0;				// Sets the conveter to store a value equal to 5 metres
		converter.Feet += 2.5;				// Increaces the length stored by and additional 2.5 feet
		converter.Inches -= 1.0;			// Shortens the length stored by an inch
		double result = converter.Metres;	// Return the total length formatted as a length in metres;

		// Implementing the same code without properties

		converter.Set(converter::unit_t::METRE,	5.0);
		converter.Set(converter::unit_t::FOOT,	converter.Get(converter::unit_t::FOOT) + 2.5);
		converter.Set(converter::unit_t::INCH,	converter.Get(converter::unit_t::INCH) - 1.0);
		result = converter.Get(converter::unit_t::METRE);
		\endcode
	*/
	template<class CONVERTER, typename CONVERTER::unit_t UNIT>
	class ConverterProperty{
	private:
		CONVERTER* mParent;	//!< A pointer to the converter that owns this property.
	public:
		friend CONVERTER;

		/*!
			\brief Call the parent converter's Set function.
			\param aValue The value to use as a Set paramiter.
			\see BaseConverter::Set
		*/
		void operator=(typename CONVERTER::conversion_t aValue){
			return mParent->Set(UNIT, aValue);
		}
		
		/*!
			\brief Call the parent converter's Get function.
			\see BaseConverter::Get
		*/
		operator typename CONVERTER::conversion_t() const{
			return mParent->Get(UNIT);
		}
		
		/*!
			\brief Call a combination of the parent converter's Get and Set functions to add to it's intermediary value.
			\param aValue The value to add to the intermediary.
			\see BaseConverter::Get
			\see BaseConverter::Set
		*/
		void operator+=(typename CONVERTER::conversion_t aValue){
			mParent->Set(UNIT, mParent->Get(UNIT) + aValue);
		}
		
		/*!
			\brief Call a combination of the parent converter's Get and Set functions to subtract from it's intermediary value.
			\param aValue The value to subtract from the intermediary.
			\see BaseConverter::Get
			\see BaseConverter::Set
		*/
		void operator-=(typename CONVERTER::conversion_t aValue){
			mParent->Set(UNIT, mParent->Get(UNIT) - aValue);
		}
		
		/*!
			\brief Call a combination of the parent converter's Get and Set functions to multiply it's intermediary value.
			\param aValue The value to multiply the intermediary by.
			\see BaseConverter::Get
			\see BaseConverter::Set
		*/
		void operator*=(typename CONVERTER::conversion_t aValue){
			mParent->Set(UNIT, mParent->Get(UNIT) * aValue);
		}
		
		/*!
			\brief Call a combination of the parent converter's Get and Set functions to divide it's intermediary value.
			\param aValue The value to divide the intermediary by.
			\see BaseConverter::Get
			\see BaseConverter::Set
		*/
		void operator/=(typename CONVERTER::conversion_t aValue){
			mParent->Set(UNIT, mParent->Get(UNIT) / aValue);
		}
	};
#endif
}}


#endif