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

#include <iostream>
#include <cstring>
#include "Solaire\Core\Base64.hpp"

namespace Solaire{

    char* Base64::Encode(char* aOutput, const uint32_t aOutputLength, const void* const aInput, const uint32_t aInputLength, const char* const aBase64, const char* const aPadding) {
        const uint32_t outputLength = (aInputLength * 4) / 3;
    	if (aOutputLength < outputLength) {
    		return nullptr;
    	}
    
    	const char* input = static_cast<const char*>(aInput);
    	const char* const end = input + aInputLength;
    
    	uint8_t b;
    	while (input < end) {
    		b = (*input & 0xFC) >> 2;
    		*aOutput = aBase64[b];
    		++aOutput;
    		b = (*input & 0x03) << 4;
    
    		if (input + 1 < end) {
    			b |= (input[1] & 0xF0) >> 4;
    			*aOutput = aBase64[b];
    			++aOutput;
    			b = (input[1] & 0x0F) << 2;
    			if (input + 2 < end) {
    				b |= (input[2] & 0xC0) >> 6;
    				*aOutput = aBase64[b];
    				++aOutput;
    				b = input[2] & 0x3F;
    				*aOutput = aBase64[b];
    				++aOutput;
    			}
    			else {
    				*aOutput = aBase64[b];
    				++aOutput;
    				 if(aPadding){
    				    *aOutput = *aPadding;
    			    	++aOutput;
    				 }
    			}
    		}
    		else {
    			*aOutput = aBase64[b];
    			++aOutput;
    			 if(aPadding){
        			*aOutput = *aPadding;
        			++aOutput;
        			*aOutput = *aPadding;
        			++aOutput;
    			 }
    		}
    
    		input += 3;
    	}
    
    	return aOutput;
    }
    
    static char* DecodeBase64WithPadding(char* aOutput, const uint32_t aOutputLength, const void* const aInput, const uint32_t aInputLength, const char* const aBase64, const char aPadding) {
    	if((aInputLength & 3) != 0) {
    		return nullptr;
    	}
    	const char* input = static_cast<const char*>(aInput);
    	const char* const end = input + aInputLength;
    
    	const uint32_t lastPadding =
            input[aInputLength - 2] == aPadding ? aInputLength - 2 :
            input[aInputLength - 1] == aPadding ? aInputLength - 1 :
            aInputLength;
            
    	const uint32_t outputLength = ((aInputLength * 3) / 4) - (lastPadding != aInputLength ? (aInputLength - lastPadding) : 0);
    
    	if (aOutputLength < outputLength) {
    		return nullptr;
    	}
    
    	uint8_t b[4];
    	while (input != end) {
    		//! \todo Pre-calculate?
    		b[0] = std::strchr(aBase64, input[0]) - aBase64;
    		b[1] = std::strchr(aBase64, input[1]) - aBase64;
    		b[2] = std::strchr(aBase64, input[2]) - aBase64;
    		b[3] = std::strchr(aBase64, input[3]) - aBase64;
    		*aOutput = static_cast<char>((b[0] << 2) | (b[1] >> 4));
    		++aOutput;
    		if (b[2] < 64) {
    			*aOutput = static_cast<char>((b[1] << 4) | (b[2] >> 2));
    			++aOutput;
    			if (b[3] < 64) {
    				*aOutput = static_cast<char>((b[2] << 6) | b[3]);
    				++aOutput;
    			}
    		}
    
    		input += 4;
    	}
    
    	return aOutput;
    }
    
    static char* DecodeBase64WithoutPadding(char* aOutput, const uint32_t aOutputLength, const void* const aInput, const uint32_t aInputLength, const char* const aBase64) {
    	//! \todo Optimise DecodeBase64WithoutPadding

		const char paddingChar = *BASE_64_STANDARD_PADDING;
    	uint32_t padding = aInputLength & 3;
    	if(padding == 3){
            padding = 1;
    	}

		char* output = nullptr;

		if(padding > 0) {
			char* const buf = new char[aInputLength + 3];
			std::memcpy(buf, aInput, aInputLength);

			if(padding == 1) {
				buf[aInputLength] = paddingChar;
				buf[aInputLength + 1] = '\0';
			}else if(padding == 2) {
				buf[aInputLength] = paddingChar;
				buf[aInputLength + 1] = paddingChar;
				buf[aInputLength + 2] = '\0';
			}

			char base64[66];
			std::memcpy(base64, aBase64, 64);
			base64[64] = '=';
			base64[65] = '\0';

			output = DecodeBase64WithPadding(aOutput, aOutputLength, buf, aInputLength + padding, base64, paddingChar);

			delete buf;
		}else{
			output = DecodeBase64WithPadding(aOutput, aOutputLength, aInput, aInputLength, aBase64, paddingChar);
		}

    	return output;
    }
    
    char* Base64::Decode(char* aOutput, const uint32_t aOutputLength, const void* const aInput, const uint32_t aInputLength, const char* const aBase64, const char* const aPadding) {
    	return aPadding ? 
    	    DecodeBase64WithPadding(aOutput, aOutputLength, aInput, aInputLength, aBase64, *aPadding) :
    	    DecodeBase64WithoutPadding(aOutput, aOutputLength, aInput, aInputLength, aBase64);
    }
}