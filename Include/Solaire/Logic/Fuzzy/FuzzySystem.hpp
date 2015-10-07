#ifndef SOLAIRE_LOGIC_FUZZY_SYSTEM_HPP
#define SOLAIRE_LOGIC_FUZZY_SYSTEM_HPP

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
\file FuzzySystem.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 7th October 2015
Last Modified	: 7th October 2015
*/

#include "Fuzzifier.hpp"
#include "Defuzzifier.hpp"
#include "FuzzyBody.hpp"

namespace Solaire {namespace Fuzzy{

    typedef std::function<float(const float* const, const size_t)> FOutputCombiner;

    class FScript : public Fuzzifier{
    private:
        typedef std::pair<String, std::shared_ptr<const FBody>> FLine;
        DynamicArray<FLine> mLines;


        std::map<String, float> mInputMap;
        std::map<String, DynamicArray<float>> mOutputBuffer;

        std::shared_ptr<const Fuzzifier> mFuzzifier;
        std::shared_ptr<Defuzzifier> mDefuzzifier;
        FOutputCombiner mOutputCombiner;

        Allocator* mAllocator;
    public:
        FScript(Allocator& aAllocator):
            mAllocator(& aAllocator)
        {}

        void SetFuzzifier(std::shared_ptr<const Fuzzifier> aFuzzifier){
            mFuzzifier.swap(aFuzzifier);

            mInputMap.clear();

            const size_t count = mFuzzifier->GetInputCount();
            for(size_t i = 0; i < count; ++i){
                const ConstStringFragment name = mFuzzifier->GetInputName(i);
                mInputMap.emplace(String(name, *mAllocator), 0.f);
                mOutputBuffer.emplace(String(name, *mAllocator), DynamicArray<float>(16, *mAllocator));
            }
        }

        void SetDefuzzifier(std::shared_ptr<Defuzzifier> aDefuzzifier){
            mDefuzzifier.swap(aDefuzzifier);
        }

        void SetDefuzzifier(FOutputCombiner aOutputCombiner){
            mOutputCombiner = aOutputCombiner;
        }

        void Execute(){
            for(std::pair<const String, float>& input : mInputMap){
                input.second = mFuzzifier->GetInputValue(input.first);
            }

            for(const FLine& line : mLines){
                const float output = line.second->Execute(*this);
                DynamicArray<float>& buffer = mOutputBuffer.find(line.first)->second;
                buffer.PushBack(output);
            }

            for(std::pair<const String, DynamicArray<float>>& buffer : mOutputBuffer){
                if(buffer.second.IsEmpty()) buffer.second.PushBack(0.f);
                const float output = mOutputCombiner(&buffer.second[0], buffer.second.Size());
                mDefuzzifier->SetOutputValue(buffer.first, output);
                buffer.second.Clear();
            }
        }

        // Inherited from fuzzifier

        size_t GetInputCount() const override{
            return mInputMap.size();
        }

        ConstStringFragment GetInputName(const size_t aInput) const override{
            auto it = mInputMap.begin();
            for(size_t i = 0; i < aInput; ++i) ++it;
            return it->first;
        }

        float GetInputValue(const ConstStringFragment aName) const override{
            auto it = mInputMap.find(aName);
            return it == mInputMap.end() ? 0.f : it->second;
        }
    };

}}

#endif
