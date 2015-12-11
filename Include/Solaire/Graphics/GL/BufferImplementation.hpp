#ifndef SOLAIRE_GL_BUFFER_IMPLEMENTATION_HPP
#define SOLAIRE_GL_BUFFER_IMPLEMENTATION_HPP

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
\file BufferImplementation.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 11th December 2015
Last Modified	: 11th December 2015
*/

#define SOLAIRE_GL_COPY_READ_BUFFER_VER 3,1
#define SOLAIRE_GL_COPY_WRITE_BUFFER_VER 3,1
#define SOLAIRE_GL_TEXTURE_BUFFER_VER 3,1
#define SOLAIRE_GL_UNIFORM_BUFFER_VER 4,0
#define SOLAIRE_GL_DRAW_INDIRECT_BUFFER_VER 4,1
#define SOLAIRE_GL_ATOMIC_COUNTER_BUFFER_VER 4,2
#define SOLAIRE_GL_GL_DISPATCH_INDIRECT_BUFFER_VER 4,3
#define SOLAIRE_GL_SHADER_STORAGE_BUFFER_VER 4,3
#define SOLAIRE_GL_QUERY_BUFFER_VER 4,4
#define SOLAIRE_GL_ARRAY_BUFFER_VER 3,0
#define SOLAIRE_GL_ELEMENT_ARRAY_BUFFER_VER 3,0
#define SOLAIRE_GL_PIXEL_PACK_BUFFER_VER 3,0
#define SOLAIRE_GL_PIXEL_UNPACK_BUFFER_VER 3,0
#define SOLAIRE_GL_TRANSFORM_FEEDBACK_BUFFER_VER 3,0
	
#include <map>
#include <vector>
#include "Object.hpp"

namespace Solaire {

	class Buffer;

	namespace GLBufferImplementation {

		constexpr GLenum GetBinding(const GLenum aTarget) {
			return
				#if SOLAIRE_GL_VER_GTE(3,1)
					aTarget == GL_COPY_READ_BUFFER ? GL_COPY_READ_BUFFER_BINDING :
					aTarget == GL_COPY_WRITE_BUFFER ? GL_COPY_WRITE_BUFFER_BINDING :
					aTarget == GL_TEXTURE_BUFFER ? GL_TEXTURE_BUFFER_BINDING :
				#endif
				#if SOLAIRE_GL_VER_GTE(4,0)
					aTarget == GL_UNIFORM_BUFFER ? GL_UNIFORM_BUFFER_BINDING :
				#endif
				#if SOLAIRE_GL_VER_GTE(4,1)
					aTarget == GL_DRAW_INDIRECT_BUFFER ? GL_DRAW_INDIRECT_BUFFER_BINDING :
				#endif
				#if SOLAIRE_GL_VER_GTE(4,2)
					aTarget == GL_ATOMIC_COUNTER_BUFFER ? GL_ATOMIC_COUNTER_BUFFER_BINDING :
				#endif
				#if SOLAIRE_GL_VER_GTE(4,3)
					aTarget == GL_DISPATCH_INDIRECT_BUFFER ? GL_DISPATCH_INDIRECT_BUFFER_BINDING :
					aTarget == GL_SHADER_STORAGE_BUFFER ? GL_SHADER_STORAGE_BUFFER_BINDING :
				#endif
				#if SOLAIRE_GL_VER_GTE(4,4)
					aTarget ==  GL_QUERY_BUFFER ? GL_QUERY_BUFFER_BINDING :
				#endif
				aTarget == GL_ARRAY_BUFFER,
				aTarget == GL_ELEMENT_ARRAY_BUFFER,
				aTarget == GL_PIXEL_PACK_BUFFER,
				aTarget == GL_PIXEL_UNPACK_BUFFER,
				aTarget == GL_TRANSFORM_FEEDBACK_BUFFER,
				GL_INVALID_ENUM;
		}

		enum class BufferTarget : GLenum {
			#if SOLAIRE_GL_VER_GTE(3,1)
				COPY_READ = GL_COPY_READ_BUFFER,
				COPY_WRITE = GL_COPY_WRITE_BUFFER,
				TEXTURE = GL_TEXTURE_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,0)
				UNIFORM = GL_UNIFORM_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,1)
				DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,2)
				ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,3)
				DISPATCH_INDIRECT = GL_DISPATCH_INDIRECT_BUFFER,
				SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER,
			#endif
			#if SOLAIRE_GL_VER_GTE(4,4)
				QUERY = GL_QUERY_BUFFER,
			#endif
			ARRAY = GL_ARRAY_BUFFER,
			ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER,
			PIXEL_PACK = GL_PIXEL_PACK_BUFFER,
			PIXEL_UNPACK = GL_PIXEL_UNPACK_BUFFER,
			TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
			INVALID_TARGET = GL_INVALID_ENUM
		};

		template<class F>
		uint32_t ForEachBufferTarget(F aFn) {
			#if SOLAIRE_GL_VER_GTE(3,1)
				aFn(BufferTarget::COPY_READ);
				aFn(BufferTarget::COPY_WRITE);
				aFn(BufferTarget::TEXTURE);
			#endif
			aFn(BufferTarget::ARRAY);
			aFn(BufferTarget::ELEMENT_ARRAY);
			#if SOLAIRE_GL_VER_GTE(4,0)
				aFn(BufferTarget::UNIFORM);
			#endif
			#if SOLAIRE_GL_VER_GTE(4,1)
				aFn(BufferTarget::DRAW_INDIRECT);
			#endif
			#if SOLAIRE_GL_VER_GTE(4,2)
				aFn(BufferTarget::ATOMIC_COUNTER);
			#endif
			#if SOLAIRE_GL_VER_GTE(4,3)
				aFn(BufferTarget::DISPATCH_INDIRECT);
				aFn(BufferTarget::SHADER_STORAGE);
			#endif
			#if SOLAIRE_GL_VER_GTE(4,4)
				aFn(BufferTarget::QUERY);
			#endif
			aFn(BufferTarget::PIXEL_PACK);
			aFn(BufferTarget::PIXEL_UNPACK);
			aFn(BufferTarget::TRANSFORM_FEEDBACK);
		}

		static std::map<BufferTarget, std::vector<Buffer*>> BUFFER_BIND_MAP;

		void InitialiseBufferData() {
			static bool ONCE = true;
			if(ONCE) {
				ONCE = false;
				ForEachBufferTarget([](const BufferTarget aTarget) {
					BUFFER_BIND_MAP.emplace(aTarget, std::vector<const Buffer*>());
				});
			}
		}

		std::vector<Buffer*>* GetBindList(const BufferTarget aTarget) {
			auto it = BUFFER_BIND_MAP.find(aTarget);
			return it == BUFFER_BIND_MAP.end() ? nullptr : &it->second;
		}

		GLuint GetBindCount(const BufferTarget aTarget) {
			const std::vector<Buffer*>* const list = GetBindList(aTarget);
			return list ? list->size() : 0;
		}

		BufferTarget GetUnusedBuffer() {
			for(const auto& i : BUFFER_BIND_MAP) if(i.second.empty()) return i.first;
			return BufferTarget::INVALID_TARGET;
		}

		bool Bind(const BufferTarget aTarget, Buffer& aBuffer) {
			std::vector<Buffer*>* const list = GetBindList(aTarget);
			if(list == nullptr) return false;
			list->push_back(&aBuffer);
			glBindBuffer(static_cast<GLenum>(aTarget), reinterpret_cast<Object&>(aBuffer).GetID());
			return true;
		}

		bool Unbind(const BufferTarget aTarget, Buffer& aBuffer) {
			std::vector<Buffer*>* const list = GetBindList(aTarget);
			if(list == nullptr) return false;
			if(list->empty()) return false;
			if(list->back() != &aBuffer) return false;
			list->pop_back();
			glBindBuffer(static_cast<GLenum>(aTarget), list->empty() ? Object::NULL_ID : reinterpret_cast<Object*>(list->back())->GetID());
			return true;
		}
	}
}

#endif
