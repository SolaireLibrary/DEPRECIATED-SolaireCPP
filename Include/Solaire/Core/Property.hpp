#ifndef SOLAIRE_PROPERTY_HPP
#define SOLAIRE_PROPERTY_HPP

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
	\file Property.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 6th October 2015
	Last Modified	: 6th October 2015
*/

namespace Solaire{
    template<class Type, class ConstReturnType, class Parent, Type Parent::* const Member>
    class ReadValueProperty{
    private:
        ReadValueProperty(const ReadValueProperty&) = delete;
        ReadValueProperty(ReadValueProperty&&) = delete;
        ReadValueProperty& operator=(const ReadValueProperty&) = delete;
        ReadValueProperty& operator=(ReadValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        ReadValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        operator ConstReturnType() const{
            return mParent.*Member;
        }
    };

    template<class Type, class ReturnType, class Parent, Type Parent::* const Member>
    class WriteValueProperty{
    private:
        WriteValueProperty(const WriteValueProperty&) = delete;
        WriteValueProperty(WriteValueProperty&&) = delete;
        WriteValueProperty& operator=(const WriteValueProperty&) = delete;
        WriteValueProperty& operator=(WriteValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        WriteValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        operator ReturnType(){
            return mParent.*Member;
        }
    };

    template<class Type, class ReturnType, class ConstReturnType, class Parent, Type Parent::* const Member>
    class ReadWriteValueProperty{
    private:
        ReadWriteValueProperty(const ReadWriteValueProperty&) = delete;
        ReadWriteValueProperty(ReadWriteValueProperty&&) = delete;
        ReadWriteValueProperty& operator=(const ReadWriteValueProperty&) = delete;
        ReadWriteValueProperty& operator=(ReadWriteValueProperty&&) = delete;
    private:
        Parent& mParent;
    public:
        ReadWriteValueProperty(Parent& aParent):
            mParent(aParent)
        {}

        operator ReturnType(){
            return mParent.*Member;
        }

        operator ConstReturnType() const{
            return mParent.*Member;
        }
    };

}


#endif
