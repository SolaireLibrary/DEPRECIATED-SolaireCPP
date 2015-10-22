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
            Vector2F point;

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
            } obb;

            struct{
                Vector2F points[3];
            } triangle;
        };
        uint8_t type;

        static CollisionShape2D Point(const Vector2F aPoint){
            CollisionShape2D tmp;
            tmp.type = SHAPE2D_POINT;
            tmp.point = aPoint;
            return tmp;
        }

        static CollisionShape2D Line(const Vector2F aBegin, const Vector2F aEnd){
            CollisionShape2D tmp;
            tmp.type = SHAPE2D_LINE;
            tmp.line.begin = aBegin;
            tmp.line.end = aEnd;
            return tmp;
        }

        static CollisionShape2D Circle(const Vector2F aCentre, const float aRadius){
            CollisionShape2D tmp;
            tmp.type = SHAPE2D_CIRCLE;
            tmp.circle.centre = aCentre;
            tmp.circle.radius = aRadius;
            return tmp;
        }

        static CollisionShape2D Aabb(const Vector2F aCentre, const Vector2F aExtents){
            CollisionShape2D tmp;
            tmp.type = SHAPE2D_AABB;
            tmp.aabb.centre = aCentre;
            tmp.aabb.extents = aExtents;
            return tmp;
        }

        static CollisionShape2D Obb(const Vector2F aCentre, const Vector2F aExtents, const float aRotation){
            CollisionShape2D tmp;
            tmp.type = SHAPE2D_OBB;
            tmp.obb.centre = aCentre;
            tmp.obb.extents = aExtents;
            tmp.obb.rotation = aRotation;
            return tmp;
        }

        static CollisionShape2D Triangle(const Vector2F aPointA, const Vector2F aPointB, const Vector2F aPointC){
            CollisionShape2D tmp;
            tmp.type = SHAPE2D_TRIANGLE;
            tmp.triangle.points[0] = aPointA;
            tmp.triangle.points[1] = aPointB;
            tmp.triangle.points[2] = aPointC;
            return tmp;
        }

        CollisionShape2D():
            point({0.f, 0.f}),
            type(SHAPE2D_POINT)
        {}

        CollisionShape2D(const CollisionShape2D& aOther):
            type(aOther.type)
        {
            switch(aOther.type){
            case SHAPE2D_POINT:
                point = aOther.point;
                break;
            case SHAPE2D_LINE:
                line.begin = aOther.line.begin;
                line.end = aOther.line.end;
                break;
            case SHAPE2D_CIRCLE:
                circle.centre = aOther.circle.centre;
                circle.radius = aOther.circle.radius;
                break;
            case SHAPE2D_AABB:
                aabb.centre = aOther.aabb.centre;
                aabb.extents = aOther.aabb.extents;
                break;
            case SHAPE2D_OBB:
                obb.centre = aOther.obb.centre;
                obb.extents = aOther.obb.extents;
                obb.rotation = aOther.obb.rotation;
                break;
            case SHAPE2D_TRIANGLE:
                triangle.points[0] = aOther.triangle.points[0];
                triangle.points[1] = aOther.triangle.points[1];
                triangle.points[2] = aOther.triangle.points[2];
                break;
            default:
                throw std::runtime_error("CollisionShape2D : Unknown type");
            }
        }
    };


    struct CollisionData2D{
        CollisionShape2D shapeA;
        CollisionShape2D shapeB;
        Vector2F contactPoint;
        Vector2F penitration;
        bool collides;

        CollisionData2D(const CollisionShape2D& aShapeA, const CollisionShape2D& aShapeB):
            shapeA(aShapeA),
            shapeB(aShapeB),
            contactPoint({0.f, 0.f}),
            penitration({0.f, 0.f}),
            collides(false)
        {}

        CollisionData2D& Reverse(){
            std::swap(shapeA, shapeB);
            penitration *= -1.f;
            return *this;
        }
    };

    static void Collision2DLineVsPoint(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DLineVsLine(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DLineVsCircle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DLineVsAabb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DLineVsObb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DLineVsTriangle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DCircleVsPoint(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DCircleVsLine(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DCircleVsCircle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DCircleVsAabb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DCircleVsObb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DCircleVsTriangle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DAabbVsPoint(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DAabbVsLine(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DAabbVsCircle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DAabbVsAabb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DAabbVsObb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DAabbVsTriangle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DObbVsPoint(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DObbVsLine(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DObbVsCircle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DObbVsAabb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DObbVsObb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DObbVsTriangle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DTriangleVsPoint(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DTriangleVsLine(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DTriangleVsCircle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DTriangleVsAabb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DTriangleVsObb(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DTriangleVsTriangle(CollisionData2D& aData){
        //! \todo Implement Collision2D test
    }

    static void Collision2DPointVsPoint(CollisionData2D& aData){
        if(aData.shapeA.point == aData.shapeB.point){
            aData.collides = true;
            aData.contactPoint = aData.shapeA.point;
            aData.penitration = {1.f, 1.f};
        }else{
            aData.collides = false;
            aData.contactPoint = {0.f, 0.f};
            aData.penitration = {0.f, 0.f};
        }
    }

    static void Collision2DPointVsLine(CollisionData2D& aData){
        Collision2DLineVsPoint(aData);
        aData.Reverse();
    }

    static void Collision2DPointVsCircle(CollisionData2D& aData){
        Collision2DCircleVsPoint(aData);
        aData.Reverse();
    }

    static void Collision2DPointVsAabb(CollisionData2D& aData){
        Collision2DAabbVsPoint(aData);
        aData.Reverse();
    }

    static void Collision2DPointVsObb(CollisionData2D& aData){
        Collision2DObbVsPoint(aData);
        aData.Reverse();
    }

    static void Collision2DPointVsTriangle(CollisionData2D& aData){
        Collision2DTriangleVsPoint(aData);
        aData.Reverse();
    }


    typedef void (*Collision2AVsB)(CollisionData2D&);

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
        CollisionData2D tmp(aShapeA, aShapeB);

        COLLISION_TEST_TABLE[aShapeA.type][aShapeB.type](tmp);

        return tmp;
    }
}


#endif
