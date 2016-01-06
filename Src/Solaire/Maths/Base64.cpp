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
#include "Solaire\Maths\Base64.hpp"

namespace Solaire{

	/*!
		\brief Calculate how many padding bytes an unpadded message would have.
		\param aLength The length of the unpadded message in bytes.
		\return The number of padding bytes.
	*/
    constexpr uint32_t Base64::UnpaddedPaddingBytes(const uint32_t aLength) {
		return (aLength & 3) == 3 ? 1 : aLength & 3;
	}

	/*!
		\brief Calculate the length of an unpadded Base64 encode.
		\param aLength The number of bytes being encoded into Base64.
		\return The number of bytes required to encode the data into Base64.
	*/
    uint32_t Base64::UnpaddedEncodeLength(const uint32_t aLength) {
        //! \todo Optimise UnpaddedEncodeLength

		uint32_t input = 0;
		uint32_t output = 0;

		while(input < aLength) {
    		++output;
    		if(input + 1 < aLength) {
    			++output;
    			if(input + 2 < aLength) {
    				output += 2;
    			}else {
    				++output;
    			}
    		}else{
    			++output;
    		}
    		input += 3;
    	}

    	return output;
	}

	/*!
		\brief Calculate the length of an unpadded Base64 decode.
		\param aLength The number of bytes being decoded from Base64.
		\return The number of bytes required to decode the data from Base64.
	*/
	uint32_t Base64::UnpaddedDecodeLength(const uint32_t aLength) {
        //! \todo Optimise UnpaddedDecodeLength
        uint32_t i = 0;
        uint32_t length = 0;
        while(length != aLength){
            length = UnpaddedEncodeLength(i);
            ++i;
        }
        return i - 1;
	}


    /*!
		\brief Calculate how many padding bytes an padded message would have.
		\param aLength The length of the unpadded message in bytes.
		\return The number of padding bytes.
	*/
	uint32_t Base64::PaddedPaddingBytes(const uint32_t aLength) {
		return UnpaddedPaddingBytes(UnpaddedEncodeLength(PaddedDecodeLength(aLength)));
	}

	/*!
		\brief Calculate the length of a padded Base64 encode.
		\param aLength The number of bytes being encoded into Base64.
		\return The number of bytes required to encode the data into Base64.
	*/
	 uint32_t Base64::PaddedEncodeLength(const uint32_t aLength) {
        //! \todo Optimise PaddedEncodeLength
        uint32_t input = 0;
        uint32_t output = 0;
        while (input < aLength) {
            output += 4;
    		input += 3;
    	}

    	return output;
	}

	/*!
		\brief Calculate the length of a padded Base64 decode.
		\param aLength The number of bytes being decoded from Base64.
		\return The number of bytes required to decode the data from Base64.
	*/
	uint32_t Base64::PaddedDecodeLength(const uint32_t aLength) {
        //! \todo Optimise PaddedDecodeLength
        uint32_t i = 0;
        uint32_t length = 0;
        while(length != aLength){
            length = PaddedEncodeLength(i);
            ++i;
        }
        return i - 1;
	}

    char* Base64::Encode(char* aOutput, const uint32_t aOutputLength, const void* const aInput, const uint32_t aInputLength, const char* const aBase64, const char* const aPadding) {
		const uint32_t outputLength = UnpaddedEncodeLength(aInputLength) + aPadding ? UnpaddedPaddingBytes(aInputLength) : 0;
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

    	const uint32_t paddingBytes =
            input[aInputLength - 2] == aPadding ? 2 :
            input[aInputLength - 1] == aPadding ? 1 :
            0;

    	const uint32_t outputLength = Base64::UnpaddedDecodeLength(aInputLength - paddingBytes);

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
		const uint32_t padding = Base64::UnpaddedPaddingBytes(aInputLength);

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