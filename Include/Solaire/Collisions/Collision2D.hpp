#ifndef SOLAIRE_COLLISION_2D_HPP
#define SOLAIRE_COLLISION_2D_HPP

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
	\file Collision2D.hpp
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 3.0
	\date
	Created			: 10th October 2015
	Last Modified	: 19th October 2015
*/

#include "..\Maths\Vector.hpp"

namespace Solaire{

    typedef Vector2F CollisionPoint2D;
    class CollisionLine2D;
    class CollisionCircle;
    class CollisionAabb;
    class CollisionObb;
    class CollisionTriangle;


    struct CollisionData2D{
        bool collides;
        Vector2F contactPoint;
        Vector2F penitration;
    };

    class Collidable2D{
    public:
        virtual ~Collidable2D(){}

        virtual CollisionData2D CollisionWith(const CollisionPoint2D aShape) const = 0;
        virtual CollisionData2D CollisionWith(const CollisionLine2D aShape) const = 0;
        virtual CollisionData2D CollisionWith(const CollisionCircle aShape) const = 0;
        virtual CollisionData2D CollisionWith(const CollisionAabb aShape) const = 0;
        virtual CollisionData2D CollisionWith(const CollisionObb aShape) const = 0;
        virtual CollisionData2D CollisionWith(const Collisiontriangle aShape) const = 0;
    };
}


#endif
