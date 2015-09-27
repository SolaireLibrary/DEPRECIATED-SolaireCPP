#ifndef SOLAIRE_GRAPHICS_DEFAULT_OBJ_PARSER_HPP
#define SOLAIRE_GRAPHICS_DEFAULT_OBJ_PARSER_HPP

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
\file DefaultObjParser.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 27th September 2015
Last Modified	: 27th September 2015
*/

#include <string>
#include <sstream>
#include "ObjDoc.hpp"
#include "../Logic/StateMachine.hpp"

namespace Solaire{ namespace Graphics{

    class DefaultObjParser : public Obj::Parser, private Logic::StateMachine<bool, DefaultObjParser>
    {
        private:
            enum State : StateID{
                STATE_LINE_ANALYSER,
                STATE_COMMENT,
                STATE_VERTEX,
                STATE_NORMAL,
                STATE_TEXTURE,
                STATE_FIXED_VERTEX,
                STATE_FIXED_NORMAL,
                STATE_FIXED_TEXTURE,
                STATE_FACE,
                STATE_FIXED_FACE
            };

            typedef Obj::FaceElement(*FaceElementReader)(const std::string&);

            std::string mLine;
            std::istream* mInput;
            Obj::Doc* mDoc;
            uint32_t mFixedCount;
            FaceElementReader mFaceReader;

            static FaceElementReader AnalyseFaceFormat(const std::string& aLine){
                const char* ptr = aLine.c_str();
                ++ptr; // Skip f tag
                while(*ptr == ' ') ++ptr;

                uint32_t count = 0;
                while(*ptr != ' ' && *ptr != '\n' && *ptr != '\0'){
                    if(*ptr == '/') ++count;
                    ++ptr;
                }

                switch(count){
                case 0:
                    return &ReadFaceV;
                case 1:
                    return &ReadFaceVN;
                case 2:
                    return &ReadFaceVTN;
                default:
                    throw std::runtime_error("Unrecognised face formatting");
                }
            }

            static Obj::Face ReadFace(const FaceElementReader aFn, const std::string aLine){
                std::stringstream ss;
                ss << aLine;
                string elementString;
                ss >> elementString; // f tag

                Obj::Face tmp;
                tmp.elementCount = 0;

                while(! ss.eof()){
                    ss >> elementString;
                    tmp.elements[tmp.elementCount++] = aFn(elementString);
                }

                return tmp;
            }

            static Obj::FaceElement ReadFaceV(const std::string& aElement){
                Obj::FaceElement tmp;
                tmp.v = std::atof(aElement.c_str());
                tmp.t = 0;
                tmp.n = 0;
                return tmp;
            }

            static Obj::FaceElement ReadFaceVN(const std::string& aElement){
                char buf[16];
                std::stringstream ss;
                ss << aElement;

                Obj::FaceElement tmp;
                ss.getline(buf, 16, '/');
                tmp.v = std::atof(buf);
                tmp.t = 0;
                ss >> tmp.n;
                return tmp;
            }

            static Obj::FaceElement ReadFaceVTN(const std::string& aElement){
                char buf[16];
                std::stringstream ss;
                ss << aElement;

                Obj::FaceElement tmp;
                ss.getline(buf, 16, '/');
                tmp.v = std::atof(buf);
                ss.getline(buf, 16, '/');
                tmp.t = std::atof(buf);
                ss >> tmp.n;
                return tmp;
            }

            static Obj::Vertex ReadVertex(const std::string& aLine){
                std::stringstream ss;
                ss << aLine;

                Obj::Vertex tmp;
                ss >> tmp[0];
                ss >> tmp[1];
                ss >> tmp[2];
                return tmp;
            }

            static Obj::Normal ReadNormal(const std::string& aLine){
                return ReadVertex(aLine);
            }

            static Obj::Texture ReadTexture(const std::string& aLine){
                std::stringstream ss;
                ss << aLine;

                Obj::Texture tmp;
                ss >> tmp[0];
                ss >> tmp[1];
                return tmp;
            }

            bool ReadNextLine(){
                if(mInput->eof()){
                    return false;
                }else{
                    std::getline(*mInput, mLine);
                    return true;
                }
            }

            void LineAnalyserFn(){
                if(! ReadNextLine()){
                    Accept(true);
                }

                switch(mLine[0]){
                case 'v':
                    StateTransition(STATE_VERTEX);
                    return;
                case 't':
                    StateTransition(STATE_TEXTURE);
                    return;
                case 'n':
                    StateTransition(STATE_NORMAL);
                    return;
                case '#':
                    StateTransition(STATE_COMMENT);
                    return;
                default:
                    StateTransition(STATE_LINE_ANALYSER);
                    return;
                }
            }

            void CommentFn(){
                if(std::memcmp(mLine.c_str() + 1, "solaire_", 8) == 0){
                    const char* tag = mLine.c_str() + 9;
                    if(std::memcmp(tag, "vertex", 6) == 0){
                        tag += 6;
                        mFixedCount = std::atoi(tag);
                        StateTransition(STATE_FIXED_VERTEX);
                        return;
                    }else if(std::memcmp(tag, "texture", 7) == 0){
                        tag += 7;
                        mFixedCount = std::atoi(tag);
                        StateTransition(STATE_FIXED_TEXTURE);
                        return;
                    }else if(std::memcmp(tag, "normal", 6) == 0){
                        tag += 6;
                        mFixedCount = std::atoi(tag);
                        StateTransition(STATE_FIXED_NORMAL);
                        return;
                    }else if(std::memcmp(tag, "face", 4) == 0){
                        tag += 4;
                        if(std::memcmp(tag, "v", 1) == 0){
                            tag += 1;
                            mFaceReader = &ReadFaceV;
                        }else if(std::memcmp(tag, "vt", 2) == 0){
                            tag += 2;
                            mFaceReader = &ReadFaceVN;
                        }else if(std::memcmp(tag, "vtn", 3) == 0){
                            tag += 3;
                            mFaceReader = &ReadFaceVTN;
                        }else{
                            StateTransition(STATE_LINE_ANALYSER);
                            return;
                        }

                        mFixedCount = std::atoi(tag);
                        StateTransition(STATE_FIXED_FACE);
                        return;
                    }
                }
                StateTransition(STATE_LINE_ANALYSER);
            }

            void VertexFn(){
                mDoc->vertices.PushBack(ReadVertex(mLine));
                StateTransition(STATE_LINE_ANALYSER);
            }

            void NormalFn(){
                mDoc->normals.PushBack(ReadNormal(mLine));
                StateTransition(STATE_LINE_ANALYSER);
            }

            void TextureFn(){
                mDoc->textures.PushBack(ReadTexture(mLine));
                StateTransition(STATE_LINE_ANALYSER);
            }

            void FaceFn(){
                mDoc->faces.PushBack(ReadFace(AnalyseFaceFormat(mLine), mLine));
                StateTransition(STATE_LINE_ANALYSER);
            }

            void FixedVertexFn(){
                for(uint32_t i = 0; i < mFixedCount; ++i){
                    mDoc->vertices.PushBack(ReadVertex(mLine));
                    if(i != mFixedCount - 1){
                        if(! ReadNextLine()){
                            Accept(false);
                            return;
                        }
                    }
                }
                StateTransition(STATE_LINE_ANALYSER);
            }

            void FixedNormalFn(){
                for(uint32_t i = 0; i < mFixedCount; ++i){
                    mDoc->normals.PushBack(ReadNormal(mLine));
                    if(i != mFixedCount - 1){
                        if(! ReadNextLine()){
                            Accept(false);
                            return;
                        }
                    }
                }
                StateTransition(STATE_LINE_ANALYSER);
            }

            void FixedTextureFn(){
                for(uint32_t i = 0; i < mFixedCount; ++i){
                    mDoc->textures.PushBack(ReadTexture(mLine));
                    if(i != mFixedCount - 1){
                        if(! ReadNextLine()){
                            Accept(false);
                            return;
                        }
                    }
                }
                StateTransition(STATE_LINE_ANALYSER);
            }

            void FixedFaceFn(){
                for(uint32_t i = 0; i < mFixedCount; ++i){
                    mDoc->faces.PushBack(ReadFace(mFaceReader, mLine));
                    if(i != mFixedCount - 1){
                        if(! ReadNextLine()){
                            Accept(false);
                            return;
                        }
                    }
                }
                StateTransition(STATE_LINE_ANALYSER);
            }

            // Inherited from StateMachine

            void Reset() override{
                mInput = nullptr;
                mDoc = nullptr;
            }
        public:
            DefaultObjParser(){
                SetInitialState(STATE_LINE_ANALYSER);

                AddState(&DefaultObjParser::LineAnalyserFn);
                AddState(&DefaultObjParser::CommentFn);
                AddState(&DefaultObjParser::VertexFn);
                AddState(&DefaultObjParser::NormalFn);
                AddState(&DefaultObjParser::TextureFn);
                AddState(&DefaultObjParser::FixedVertexFn);
                AddState(&DefaultObjParser::FixedNormalFn);
                AddState(&DefaultObjParser::FixedTextureFn);
                AddState(&DefaultObjParser::FaceFn);
                AddState(&DefaultObjParser::FixedFaceFn);
            }

            // Inherited from Parser

            bool Read(Obj::Doc& aDoc, std::istream& aStream) override{
                mInput = &aStream;
                mDoc = &aDoc;
                return StateMachine::operator()();
            }

            bool Write(const Obj::Doc& aDoc, std::ostream& aStream) override{
                using namespace Obj;

                aStream << "#solaire_vertex " << aDoc.vertices.Size() << std::endl;
                for(const Vertex& vertex : aDoc.vertices){
                    aStream << "v " << vertex.X() << ' ' << vertex.Y() << ' ' << vertex.Z() << std::endl;
                }

                aStream << "#solaire_normal " << aDoc.normals.Size() << std::endl;
                for(const Normal& normal : aDoc.normals){
                    aStream << "n " << normal.X() << ' ' << normal.Y() << ' ' << normal.Z() << std::endl;
                }

                aStream << "#solaire_texture " << aDoc.textures.Size() << std::endl;
                for(const Texture& texture : aDoc.textures){
                    aStream << "t " << texture.X() << ' ' << texture.Y() << std::endl;
                }

                aStream << "#solaire_face vtn " << aDoc.faces.Size() << std::endl;
                for(const Face& face : aDoc.faces){
                    aStream << "f " << std::endl;
                    for(uint32_t i = 0; i < face.elementCount; ++i){
                        const FaceElement& element = face.elements[i];
                        aStream << element.v << '/' << element.t << '/' << element.n;
                        if(i != face.elementCount - 1) aStream << ' ';
                    }
                    aStream << std::endl;
                }

                return true;
            }

    };

}}

#endif
