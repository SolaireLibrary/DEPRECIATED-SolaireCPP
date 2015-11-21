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

#include <map>
#include "Solaire\IO\File.hpp"
#include "Solaire\Memory\DefaultAllocator.hpp"

#if SOLAIRE_OS == SOLAIRE_WINDOWS

namespace Solaire {

	static std::map<String, HANDLE> FILE_TABLE;

	static bool ImplementationOpenFile(const String& aFilename){

		if(FILE_TABLE.find(aFilename) != FILE_TABLE.end()) return true;

		//! \bug read-only files are not handled
		const HANDLE handle = CreateFileA(
			aFilename.CString(),
			GENERIC_READ | GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr
			);

		if(handle != INVALID_HANDLE_VALUE) {
			FILE_TABLE.emplace(aFilename, handle);
			return true;
		}else {
			return false;
		}
	}

	static bool ImplementationCloseFile(const String& aFilename) {
		const auto it = FILE_TABLE.find(aFilename);
		if(it == FILE_TABLE.end()) return false;
		if(! CloseHandle(it->second)) return false;
		FILE_TABLE.erase(it);
		return true;
	}

	// File

	File::Attributes SOLAIRE_EXPORT_CALL File::GetAttributes(const ConstStringFragment aFilename) throw() {
		const String filename(DEFAULT_ALLOCATOR, aFilename);
		const DWORD attributes = GetFileAttributesA(filename.CString());
		DWORD binaryType;
		const BOOL executable = GetBinaryTypeA(filename.CString(), &binaryType);
		
		Attributes tmp = NO_ATTRIBUTES;
		if(attributes & FILE_ATTRIBUTE_HIDDEN) tmp |= HIDDEN;
		if(attributes & FILE_ATTRIBUTE_DIRECTORY) tmp |= DIRECTORY;
		else tmp |= FILE;
		if(attributes & FILE_ATTRIBUTE_READONLY) tmp |= READ;
		else tmp |= READ | WRITE;
		if(executable) tmp |= EXECUTABLE;

		return tmp;
	}

	bool SOLAIRE_EXPORT_CALL File::CreateFile(const ConstStringFragment aFilename, const Attributes aAttributes) throw() {
		const String filename(DEFAULT_ALLOCATOR, aFilename);

		DWORD access = 0;
		if(aAttributes & READ) access |= GENERIC_READ;
		if(aAttributes & WRITE) access |= GENERIC_WRITE;

		DWORD flags = 0;
		if(aAttributes & HIDDEN) flags |= FILE_ATTRIBUTE_HIDDEN;
		if((aAttributes & READ) != 0 && (aAttributes & WRITE) == 0) flags |= FILE_ATTRIBUTE_READONLY;
		if(flags == 0) flags = FILE_ATTRIBUTE_NORMAL;

		const HANDLE handle = CreateFileA(
			filename.CString(),
			access,
			0,
			nullptr,
			CREATE_NEW,
			flags,
			nullptr
		);

		if(handle != INVALID_HANDLE_VALUE) {
			CloseHandle(handle);
			return true;
		}else {
			return false;
		}
	}

	bool SOLAIRE_EXPORT_CALL File::CreateDirectory(const ConstStringFragment aFilename) throw() {
		const String filename(DEFAULT_ALLOCATOR, aFilename);
		return CreateDirectoryA(filename.CString(), nullptr);
	}

	bool SOLAIRE_EXPORT_CALL File::Delete(const ConstStringFragment aFilename) throw() {
		const String filename(DEFAULT_ALLOCATOR, aFilename);
		return DeleteFileA(filename.CString());
	}

	String SOLAIRE_EXPORT_CALL File::GetParent(const ConstStringFragment aFilename) throw() {
		auto it = aFilename.FindLast(FILE_SEPERATOR);
		if(it == aFilename.end()) {
			return String(DEFAULT_ALLOCATOR);
		}else{
			String tmp(DEFAULT_ALLOCATOR, aFilename.begin(), it + 1);
		}
		
	}

	String SOLAIRE_EXPORT_CALL File::GetName(const ConstStringFragment aFilename) throw() {
		auto it = aFilename.FindLast(FILE_SEPERATOR);
		if(it == aFilename.end()) {
			return String(DEFAULT_ALLOCATOR);
		}else{
			String tmp(DEFAULT_ALLOCATOR, it, aFilename.end());
		}
	}

	String SOLAIRE_EXPORT_CALL File::GetExtension(const ConstStringFragment aFilename) throw() {
		auto it = aFilename.FindLast('.');
		if(it == aFilename.end()) {
			return String(DEFAULT_ALLOCATOR);
		}else{
			String tmp(DEFAULT_ALLOCATOR, it, aFilename.end());
		}
	}

	uint32_t SOLAIRE_EXPORT_CALL File::Size(const ConstStringFragment aFilename) throw() {
		const String filename(DEFAULT_ALLOCATOR, aFilename);

		bool opened = false;
		auto it = FILE_TABLE.find(filename);
		if(it == FILE_TABLE.end()) {
			opened = true;
			if (!ImplementationOpenFile(filename)) return 0;
			it = FILE_TABLE.find(filename);
		}

		const uint32_t size = GetFileSize(it->second, nullptr);

		if(opened) {
			ImplementationCloseFile(filename);
		}

		return size;
	}

	DynamicArray<String> SOLAIRE_EXPORT_CALL File::GetFileList(const ConstStringFragment aFilename) throw() {
		const String parent(DEFAULT_ALLOCATOR, aFilename);
		DynamicArray<String> children(DEFAULT_ALLOCATOR);
		WIN32_FIND_DATAA findData;
		HANDLE handle = INVALID_HANDLE_VALUE;

		handle = FindFirstFileA(parent.CString(), &findData);
		if(handle == INVALID_HANDLE_VALUE) return children;

		do{
			String tmp(DEFAULT_ALLOCATOR, findData.cFileName);
			children.PushBack(tmp);
		}while(FindNextFileA(handle, &findData) != 0);

		FindClose(handle);

		return children;
	}

	String SOLAIRE_EXPORT_CALL File::GetCurrentDirectory() throw() {
		char buf[256];
		const DWORD length = GetCurrentDirectoryA(256, buf);
		return String(DEFAULT_ALLOCATOR, buf, length);
	}

	String SOLAIRE_EXPORT_CALL File::GetTemporaryDirectory() throw() {
		char buf[256];
		const DWORD length = GetTempPathA(256, buf);
		return String(DEFAULT_ALLOCATOR, buf, length);
	}

	bool SOLAIRE_EXPORT_CALL File::Rename(const ConstStringFragment aOldName, const ConstStringFragment aNewName) throw() {
		//! \todo Optimise Rename
		if(! Copy(aOldName, aNewName)) return false;
		if(! Delete(aOldName)){
			Delete(aNewName);
			return false;
		}
		return true;
	}

	bool SOLAIRE_EXPORT_CALL File::Copy(const ConstStringFragment aSrc, const ConstStringFragment aDst) throw() {
		const String src(DEFAULT_ALLOCATOR, aSrc);
		const String dst(DEFAULT_ALLOCATOR, aDst);
		return CopyFileA(src.CString(), dst.CString(), FALSE);
	}

	bool SOLAIRE_EXPORT_CALL File::Move(const ConstStringFragment aFile, const ConstStringFragment aTarget) throw() {
		const String file(DEFAULT_ALLOCATOR, aFile);
		const String target(DEFAULT_ALLOCATOR, aTarget);
		return MoveFileA(file.CString(), target.CString());
	}

}

#endif