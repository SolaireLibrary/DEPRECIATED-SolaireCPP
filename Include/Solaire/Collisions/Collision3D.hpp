#ifndef SOLAIRE_COLLISION_3D_HPP
#define SOLAIRE_COLLISION_3D_HPP

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
	\file Collision3D.hpp
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.0
	\date
	Created			: 19th October 2015
	Last Modified	: 19th October 2015
*/

#include "..\Maths\Vector.hpp"

namespace Solaire{

    typedef Vector3F CollisionPoint3D;
    class CollisionLine3D;
    class CollisionSphere;
    class CollisionCuboid;
    class CollisionCapsule;


    struct CollisionData3D{
        bool Collides;
        Vector3F PenitrationDepth;
    };

    class Collidable3D{
    public:
        virtual ~Collidable3D(){}

        virtual void GetRotation() const = 0; //! \TODO Implement rotation

        virtual CollisionData2D CollisionWith(const CollisionPoint3D aShape) const = 0;
        virtual CollisionData2D CollisionWith(const CollisionLine3D& aShape) const = 0;
        virtual CollisionData2D CollisionWith(const CollisionSphere& aShape) const = 0;
        virtual CollisionData2D CollisionWith(const CollisionCuboid& aShape) const = 0;
        virtual CollisionData2D CollisionWith(const CollisionCapsule& aShape) const = 0;
    };
}


#endif
