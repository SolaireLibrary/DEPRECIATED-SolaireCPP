#ifndef SOLAIRE_MATHS_VECTOR_SHARED_HPP
#define SOLAIRE_MATHS_VECTOR_SHARED_HPP

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

#include <limits>

namespace Solaire{ namespace Maths{
	static size_t AxisToIndex(const char aAxis){
		return 
			aAxis >= 'x' && aAxis <= 'z' ? aAxis - 'x' :
			aAxis >= 'X' && aAxis <= 'Z' ? aAxis - 'X' :
			std::numeric_limits<size_t>::max();
	}

	static char IndexToAxis(const size_t aIndex){
		return 'X' + aIndex;
	}
}}


#endif