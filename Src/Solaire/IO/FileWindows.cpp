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

#include <string>
#include <map>
#include "Solaire\IO\File.hpp"
#include "Solaire\Memory\DefaultAllocator.hpp"

#if SOLAIRE_OS == SOLAIRE_WINDOWS

namespace Solaire { namespace File {

	static std::map<std::string, HANDLE> FILE_TABLE;

	bool ImplementationOpenFile(const std::string& aFilename){
		if(FILE_TABLE.find(aFilename) != FILE_TABLE.end()) return true;

		//! \bug read-only files are not handled
		const HANDLE handle = CreateFileA(
			aFilename.c_str(),
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

	bool ImplementationCloseFile(const std::string& aFilename) {
		const auto it = FILE_TABLE.find(aFilename);
		if(it == FILE_TABLE.end()) return false;
		if(! CloseHandle(it->second)) return false;
		FILE_TABLE.erase(it);
		return true;
	}

	bool ImplementationOpenFile(const char* const aFilename){
		return ImplementationOpenFile(std::string(aFilename));
	}

	bool ImplementationCloseFile(const char* aFilename) {
		return ImplementationCloseFile(std::string(aFilename));
	}

	// File

	extern "C" {
		SOLAIRE_EXPORT_API AttributeFlags SOLAIRE_EXPORT_CALL GetAttributes(const char* const aFilename) throw() {
			const DWORD attributes = GetFileAttributesA(aFilename);
			DWORD binaryType;
			const BOOL executable = GetBinaryTypeA(aFilename, &binaryType);

			AttributeFlags tmp = FLAG_NONE;
			if(attributes & FILE_ATTRIBUTE_HIDDEN) tmp |= FLAG_HIDDEN;
			if(attributes & FILE_ATTRIBUTE_DIRECTORY) tmp |= FLAG_DIRECTORY;
			else tmp |= FLAG_FILE;
			if(attributes & FILE_ATTRIBUTE_READONLY) tmp |= FLAG_READ;
			else tmp |= FLAG_READ | FLAG_WRITE;
			if(executable) tmp |= FLAG_EXECUTABLE;
			if(tmp != FLAG_NONE) tmp |= FLAG_EXISTS;

			return tmp;
		}

		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL CreateFile(const char* const aFilename, const AttributeFlags aAttributes) throw() {
			DWORD access = 0;
			if (aAttributes & FLAG_READ) access |= GENERIC_READ;
			if (aAttributes & FLAG_WRITE) access |= GENERIC_WRITE;

			DWORD flags = 0;
			if (aAttributes & FLAG_HIDDEN) flags |= FILE_ATTRIBUTE_HIDDEN;
			if ((aAttributes & FLAG_READ) != 0 && (aAttributes & FLAG_WRITE) == 0) flags |= FILE_ATTRIBUTE_READONLY;
			if (flags == 0) flags = FILE_ATTRIBUTE_NORMAL;

			const HANDLE handle = CreateFileA(
				aFilename,
				access,
				0,
				nullptr,
				CREATE_NEW,
				flags,
				nullptr
				);

			if (handle != INVALID_HANDLE_VALUE) {
				CloseHandle(handle);
				return true;
			}
			else {
				return false;
			}
		}

		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL CreateDirectory(const char* const aFilename) throw() {
			return CreateDirectoryA(aFilename, nullptr);
		}

		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL Delete(const char* const aFilename) throw() {
			return DeleteFileA(aFilename);
		}

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetParent(const char* const aFilename, char* const aOutput) throw() {
			const uint32_t length = std::strlen(aFilename);

			const char* j = std::strrchr(aFilename, FILE_SEPERATOR);
			const char* i = i;
			while (j != nullptr) {
				j = std::strrchr(j, FILE_SEPERATOR);
				if (j != nullptr) i = j;
			}

			if (i == nullptr) {
				return 0;
			}
			else {
				const uint32_t outputLength = i - aFilename;
				if (outputLength >= MAX_PATH_LENGTH) return 0;
				std::memcpy(aOutput, aFilename, outputLength);
				aOutput[outputLength] = '\0';
				return outputLength;
			}
		}

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetName(const char* const aFilename, char* const aOutput) throw() {
			const char* i = std::strrchr(aFilename, FILE_SEPERATOR);
			if (i == nullptr) {
				return 0;
			}
			else {
				const uint32_t length = i - aFilename;
				std::memcpy(aOutput, aFilename, length);
				aOutput[length] = '\0';
				return length;
			}
		}

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetExtension(const char* const aFilename, char* const aOutput) throw() {
			const char* i = std::strrchr(aFilename, '.');
			if (i == nullptr) {
				return 0;
			}
			else {
				const uint32_t length = i - aFilename;
				std::memcpy(aOutput, aFilename, length);
				aOutput[length] = '\0';
				return length;
			}
		}

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL Size(const char* const aFilename) throw() {
			const std::string filename(aFilename);

			bool opened = false;
			auto it = FILE_TABLE.find(filename);
			if (it == FILE_TABLE.end()) {
				opened = true;
				if (!ImplementationOpenFile(filename)) return 0;
				it = FILE_TABLE.find(filename);
			}

			const uint32_t size = GetFileSize(it->second, nullptr);

			if (opened) {
				ImplementationCloseFile(filename);
			}

			return size;
		}

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetFileList(const char* const aDirectory, const char** const aFiles, const uint32_t aMaxFiles) throw() {
			WIN32_FIND_DATAA findData;
			HANDLE handle = INVALID_HANDLE_VALUE;

			uint32_t fileCount = 0;

			handle = FindFirstFileA(aDirectory, &findData);
			if (handle == INVALID_HANDLE_VALUE) return 0;

			do {
				std::strcpy(findData.cFileName, aFiles[fileCount++]);
			} while (FindNextFileA(handle, &findData) != 0);

			FindClose(handle);

			return fileCount;
		}

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetCurrentDirectory(char* const aOutput) throw() {
			return GetCurrentDirectoryA(MAX_PATH_LENGTH, aOutput);
		}

		SOLAIRE_EXPORT_API uint32_t SOLAIRE_EXPORT_CALL GetTemporaryDirectory(char* const aOutput) throw() {
			return GetTempPathA(MAX_PATH_LENGTH, aOutput);
		}

		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL Rename(const char* const aOldName, const char* const aNewName) throw() {
			//! \todo Optimise Rename
			if (!Copy(aOldName, aNewName)) return false;
			if (!Delete(aOldName)) {
				return Delete(aNewName);
			}
			return true;
		}

		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL Copy(const char* const aSrc, const char* const aDst) throw() {
			return CopyFileA(aSrc, aDst, FALSE);
		}

		SOLAIRE_EXPORT_API bool SOLAIRE_EXPORT_CALL Move(const char* const aFile, const char* const aTarget) throw() {
			return MoveFileA(aFile, aTarget);
		}
	}
}}

#endif