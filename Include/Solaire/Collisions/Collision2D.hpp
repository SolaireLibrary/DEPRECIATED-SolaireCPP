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
	\version 4.0
	\date
	Created			: 10th October 2015
	Last Modified	: 22nd October 2015
*/

#include "..\Maths\Vector.hpp"

namespace Solaire{

    typedef Vector2F CollisionPoint2D;
    class CollisionLine2D;
    class CollisionCircle;
    class CollisionAabb;
    class CollisionObb;
    class CollisionTriangle;

    enum : uint8_t{
        SHAPE2D_POINT,
        SHAPE2D_LINE,
        SHAPE2D_CIRCLE,
        SHAPE2D_AABB,
        SHAPE2D_OBB,
        SHAPE2D_TRIANGLE
    };

    struct CollisionShape2D{
        union{
            struct{
                Vector2F point;
            } point;

            struct{
                Vector2F begin;
                Vector2F end;
            } line;

            struct{
                Vector2F centre;
                float radius;
            } circle;

            struct{
                Vector2F centre;
                Vector2F extents;
            } aabb;

            struct{
                Vector2F centre;
                Vector2F extents;
                float rotation;
            } oob;

            struct{
                Vector2F points[3];
            } triangle;
        };
        uint8_t type;
    };


    struct CollisionData2D{
        bool collides;
        Vector2F contactPoint;
        Vector2F penitration;

        CollisionData2D& Reverse(){
            penitration *= -1.f;
            return *this;
        }
    };

    static CollisionData2D Collision2DPointVsPoint(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DPointVsLine(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DPointVsCircle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DPointVsAabb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DPointVsObb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DPointVsTriangle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DLineVsPoint(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DLineVsLine(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DLineVsCircle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DLineVsAabb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DLineVsObb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DLineVsTriangle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DCircleVsPoint(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DCircleVsLine(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DCircleVsCircle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DCircleVsAabb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DCircleVsObb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DCircleVsTriangle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DAabbVsPoint(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DAabbVsLine(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DAabbVsCircle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DAabbVsAabb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DAabbVsObb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DAabbVsTriangle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DObbVsPoint(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DObbVsLine(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DObbVsCircle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DObbVsAabb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DObbVsObb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DObbVsTriangle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DTriangleVsPoint(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DTriangleVsLine(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DTriangleVsCircle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DTriangleVsAabb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DTriangleVsObb(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }

    static CollisionData2D Collision2DTriangleVsTriangle(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        //! \todo Implement Collision2D test
        return CollisionData2D();
    }


    typedef CollisionData2D (*Collision2AVsB)(const CollisionShape2D&, const CollisionShape2D&);

    static constexpr Collision2AVsB COLLISION_TEST_TABLE[6][6] = {
        {
            &Collision2DPointVsPoint,
            &Collision2DPointVsLine,
            &Collision2DPointVsCircle,
            &Collision2DPointVsAabb,
            &Collision2DPointVsObb,
            &Collision2DPointVsTriangle
        },
        {
            &Collision2DLineVsPoint,
            &Collision2DLineVsLine,
            &Collision2DLineVsCircle,
            &Collision2DLineVsAabb,
            &Collision2DLineVsObb,
            &Collision2DLineVsTriangle
        },
        {
            &Collision2DCircleVsPoint,
            &Collision2DCircleVsLine,
            &Collision2DCircleVsCircle,
            &Collision2DCircleVsAabb,
            &Collision2DCircleVsObb,
            &Collision2DCircleVsTriangle
        },
        {
            &Collision2DAabbVsPoint,
            &Collision2DAabbVsLine,
            &Collision2DAabbVsCircle,
            &Collision2DAabbVsAabb,
            &Collision2DAabbVsObb,
            &Collision2DAabbVsTriangle
        },
        {
            &Collision2DObbVsPoint,
            &Collision2DObbVsLine,
            &Collision2DObbVsCircle,
            &Collision2DObbVsAabb,
            &Collision2DObbVsObb,
            &Collision2DObbVsTriangle
        },
        {
            &Collision2DTriangleVsPoint,
            &Collision2DTriangleVsLine,
            &Collision2DTriangleVsCircle,
            &Collision2DTriangleVsAabb,
            &Collision2DTriangleVsObb,
            &Collision2DTriangleVsTriangle
        }
    };

    static CollisionData2D CollisionTest(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB){
        return COLLISION_TEST_TABLE[aShapeA.type][aShapeB.type](aShapeA, aShapeB);
    }
}


#endif
