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
	Last Modified	: 10th October 2015
*/

#include "..\Maths\Vector.hpp"

namespace Solaire{
    class Collidable2D;

    struct CollisionData2D{
        bool Collides;
        Vector2F PenitrationDepth;
    };

    class Collidable2D{
    public:
        virtual ~Collidable2D(){}

        virtual CollisionData2D CollisionWithPoint(const Vector2F aCentre) const = 0;
        virtual CollisionData2D CollisionWithCircle(const Vector2F aCentre, const float aRadius) const = 0;
        virtual CollisionData2D CollisionWithAABB(const Vector2F aCentre, const Vector2F aExtents) const = 0;
        virtual CollisionData2D CollisionWithOBB(const Vector2F aCentre, const Vector2F aExtents, const float aRotation) const = 0;
        virtual CollisionData2D CollisionWithTriangle(const Vector2F aPoint0, const Vector2F aPoint1, const Vector2F aPoint2) const = 0;
    };

    static CollisionData2D CircleVsCircle(const Vector2F aCentre0, const float aRadius0, const Vector2F aCentre1, const float aRadius1){
        CollisionData2D tmp;

        //tmp.PenitrationDepth = aCentre1 - aCentre0;
       // const float radiusSum = aRadius0 + aRadius1;
        //tmp.Collides = tmp.PenitrationDepth.MagnitudeSquared() < (radiusSum * radiusSum);
        //tmp.PenitrationDepth -= aRadius1;

        return tmp;
    }
}


#endif
