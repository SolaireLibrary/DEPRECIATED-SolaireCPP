#ifndef SOLAIRE_SWIZZLE_HPP
#define SOLAIRE_SWIZZLE_HPP

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
	\file Swizzle.hpp
	\brief
	\author
	Created			: Adam Smith
	Last modified	: Adam Smith
	\version 1.1
	\date
	Created			: 5th January 2015
	Last Modified	: 5th January 2015
*/

#include "Vector.hpp"

namespace Solaire {

    #define SOLAIRE_X 0
    #define SOLAIRE_Y 1
    #define SOLAIRE_Z 2
    #define SOLAIRE_W 3

    #define SOLAIRE_XX {0,0}
    #define SOLAIRE_XY {0,1}
    #define SOLAIRE_XZ {0,2}
    #define SOLAIRE_XW {0,3}
    #define SOLAIRE_YX {1,0}
    #define SOLAIRE_YY {1,1}
    #define SOLAIRE_YZ {1,2}
    #define SOLAIRE_YW {1,3}
    #define SOLAIRE_ZX {2,0}
    #define SOLAIRE_ZY {2,1}
    #define SOLAIRE_ZZ {2,2}
    #define SOLAIRE_ZW {2,3}
    #define SOLAIRE_WX {3,0}
    #define SOLAIRE_WY {3,1}
    #define SOLAIRE_WZ {3,2}
    #define SOLAIRE_WW {3,3}

    #define SOLAIRE_XXX {0,0,0}
    #define SOLAIRE_XXY {0,0,1}
    #define SOLAIRE_XXZ {0,0,2}
    #define SOLAIRE_XXW {0,0,3}
    #define SOLAIRE_XYX {0,1,0}
    #define SOLAIRE_XYY {0,1,1}
    #define SOLAIRE_XYZ {0,1,2}
    #define SOLAIRE_XYW {0,1,3}
    #define SOLAIRE_XZX {0,2,0}
    #define SOLAIRE_XZY {0,2,1}
    #define SOLAIRE_XZZ {0,2,2}
    #define SOLAIRE_XZW {0,2,3}
    #define SOLAIRE_XWX {0,3,0}
    #define SOLAIRE_XWY {0,3,1}
    #define SOLAIRE_XWZ {0,3,2}
    #define SOLAIRE_XWW {0,3,3}
    #define SOLAIRE_YXX {1,0,0}
    #define SOLAIRE_YXY {1,0,1}
    #define SOLAIRE_YXZ {1,0,2}
    #define SOLAIRE_YXW {1,0,3}
    #define SOLAIRE_YYX {1,1,0}
    #define SOLAIRE_YYY {1,1,1}
    #define SOLAIRE_YYZ {1,1,2}
    #define SOLAIRE_YYW {1,1,3}
    #define SOLAIRE_YZX {1,2,0}
    #define SOLAIRE_YZY {1,2,1}
    #define SOLAIRE_YZZ {1,2,2}
    #define SOLAIRE_YZW {1,2,3}
    #define SOLAIRE_YWX {1,3,0}
    #define SOLAIRE_YWY {1,3,1}
    #define SOLAIRE_YWZ {1,3,2}
    #define SOLAIRE_YWW {1,3,3}
    #define SOLAIRE_ZXX {2,0,0}
    #define SOLAIRE_ZXY {2,0,1}
    #define SOLAIRE_ZXZ {2,0,2}
    #define SOLAIRE_ZXW {2,0,3}
    #define SOLAIRE_ZYX {2,1,0}
    #define SOLAIRE_ZYY {2,1,1}
    #define SOLAIRE_ZYZ {2,1,2}
    #define SOLAIRE_ZYW {2,1,3}
    #define SOLAIRE_ZZX {2,2,0}
    #define SOLAIRE_ZZY {2,2,1}
    #define SOLAIRE_ZZZ {2,2,2}
    #define SOLAIRE_ZZW {2,2,3}
    #define SOLAIRE_ZWX {2,3,0}
    #define SOLAIRE_ZWY {2,3,1}
    #define SOLAIRE_ZWZ {2,3,2}
    #define SOLAIRE_ZWW {2,3,3}
    #define SOLAIRE_WXX {3,0,0}
    #define SOLAIRE_WXY {3,0,1}
    #define SOLAIRE_WXZ {3,0,2}
    #define SOLAIRE_WXW {3,0,3}
    #define SOLAIRE_WYX {3,1,0}
    #define SOLAIRE_WYY {3,1,1}
    #define SOLAIRE_WYZ {3,1,2}
    #define SOLAIRE_WYW {3,1,3}
    #define SOLAIRE_WZX {3,2,0}
    #define SOLAIRE_WZY {3,2,1}
    #define SOLAIRE_WZZ {3,2,2}
    #define SOLAIRE_WZW {3,2,3}
    #define SOLAIRE_WWX {3,3,0}
    #define SOLAIRE_WWY {3,3,1}
    #define SOLAIRE_WWZ {3,3,2}
    #define SOLAIRE_WWW {3,3,3}

    #define SOLAIRE_XXXX {0,0,0,0}
    #define SOLAIRE_XXXY {0,0,0,1}
    #define SOLAIRE_XXXZ {0,0,0,2}
    #define SOLAIRE_XXXW {0,0,0,3}
    #define SOLAIRE_XXYX {0,0,1,0}
    #define SOLAIRE_XXYY {0,0,1,1}
    #define SOLAIRE_XXYZ {0,0,1,2}
    #define SOLAIRE_XXYW {0,0,1,3}
    #define SOLAIRE_XXZX {0,0,2,0}
    #define SOLAIRE_XXZY {0,0,2,1}
    #define SOLAIRE_XXZZ {0,0,2,2}
    #define SOLAIRE_XXZW {0,0,2,3}
    #define SOLAIRE_XXWX {0,0,3,0}
    #define SOLAIRE_XXWY {0,0,3,1}
    #define SOLAIRE_XXWZ {0,0,3,2}
    #define SOLAIRE_XXWW {0,0,3,3}
    #define SOLAIRE_XYXX {0,1,0,0}
    #define SOLAIRE_XYXY {0,1,0,1}
    #define SOLAIRE_XYXZ {0,1,0,2}
    #define SOLAIRE_XYXW {0,1,0,3}
    #define SOLAIRE_XYYX {0,1,1,0}
    #define SOLAIRE_XYYY {0,1,1,1}
    #define SOLAIRE_XYYZ {0,1,1,2}
    #define SOLAIRE_XYYW {0,1,1,3}
    #define SOLAIRE_XYZX {0,1,2,0}
    #define SOLAIRE_XYZY {0,1,2,1}
    #define SOLAIRE_XYZZ {0,1,2,2}
    #define SOLAIRE_XYZW {0,1,2,3}
    #define SOLAIRE_XYWX {0,1,3,0}
    #define SOLAIRE_XYWY {0,1,3,1}
    #define SOLAIRE_XYWZ {0,1,3,2}
    #define SOLAIRE_XYWW {0,1,3,3}
    #define SOLAIRE_XZXX {0,2,0,0}
    #define SOLAIRE_XZXY {0,2,0,1}
    #define SOLAIRE_XZXZ {0,2,0,2}
    #define SOLAIRE_XZXW {0,2,0,3}
    #define SOLAIRE_XZYX {0,2,1,0}
    #define SOLAIRE_XZYY {0,2,1,1}
    #define SOLAIRE_XZYZ {0,2,1,2}
    #define SOLAIRE_XZYW {0,2,1,3}
    #define SOLAIRE_XZZX {0,2,2,0}
    #define SOLAIRE_XZZY {0,2,2,1}
    #define SOLAIRE_XZZZ {0,2,2,2}
    #define SOLAIRE_XZZW {0,2,2,3}
    #define SOLAIRE_XZWX {0,2,3,0}
    #define SOLAIRE_XZWY {0,2,3,1}
    #define SOLAIRE_XZWZ {0,2,3,2}
    #define SOLAIRE_XZWW {0,2,3,3}
    #define SOLAIRE_XWXX {0,3,0,0}
    #define SOLAIRE_XWXY {0,3,0,1}
    #define SOLAIRE_XWXZ {0,3,0,2}
    #define SOLAIRE_XWXW {0,3,0,3}
    #define SOLAIRE_XWYX {0,3,1,0}
    #define SOLAIRE_XWYY {0,3,1,1}
    #define SOLAIRE_XWYZ {0,3,1,2}
    #define SOLAIRE_XWYW {0,3,1,3}
    #define SOLAIRE_XWZX {0,3,2,0}
    #define SOLAIRE_XWZY {0,3,2,1}
    #define SOLAIRE_XWZZ {0,3,2,2}
    #define SOLAIRE_XWZW {0,3,2,3}
    #define SOLAIRE_XWWX {0,3,3,0}
    #define SOLAIRE_XWWY {0,3,3,1}
    #define SOLAIRE_XWWZ {0,3,3,2}
    #define SOLAIRE_XWWW {0,3,3,3}
    #define SOLAIRE_YXXX {1,0,0,0}
    #define SOLAIRE_YXXY {1,0,0,1}
    #define SOLAIRE_YXXZ {1,0,0,2}
    #define SOLAIRE_YXXW {1,0,0,3}
    #define SOLAIRE_YXYX {1,0,1,0}
    #define SOLAIRE_YXYY {1,0,1,1}
    #define SOLAIRE_YXYZ {1,0,1,2}
    #define SOLAIRE_YXYW {1,0,1,3}
    #define SOLAIRE_YXZX {1,0,2,0}
    #define SOLAIRE_YXZY {1,0,2,1}
    #define SOLAIRE_YXZZ {1,0,2,2}
    #define SOLAIRE_YXZW {1,0,2,3}
    #define SOLAIRE_YXWX {1,0,3,0}
    #define SOLAIRE_YXWY {1,0,3,1}
    #define SOLAIRE_YXWZ {1,0,3,2}
    #define SOLAIRE_YXWW {1,0,3,3}
    #define SOLAIRE_YYXX {1,1,0,0}
    #define SOLAIRE_YYXY {1,1,0,1}
    #define SOLAIRE_YYXZ {1,1,0,2}
    #define SOLAIRE_YYXW {1,1,0,3}
    #define SOLAIRE_YYYX {1,1,1,0}
    #define SOLAIRE_YYYY {1,1,1,1}
    #define SOLAIRE_YYYZ {1,1,1,2}
    #define SOLAIRE_YYYW {1,1,1,3}
    #define SOLAIRE_YYZX {1,1,2,0}
    #define SOLAIRE_YYZY {1,1,2,1}
    #define SOLAIRE_YYZZ {1,1,2,2}
    #define SOLAIRE_YYZW {1,1,2,3}
    #define SOLAIRE_YYWX {1,1,3,0}
    #define SOLAIRE_YYWY {1,1,3,1}
    #define SOLAIRE_YYWZ {1,1,3,2}
    #define SOLAIRE_YYWW {1,1,3,3}
    #define SOLAIRE_YZXX {1,2,0,0}
    #define SOLAIRE_YZXY {1,2,0,1}
    #define SOLAIRE_YZXZ {1,2,0,2}
    #define SOLAIRE_YZXW {1,2,0,3}
    #define SOLAIRE_YZYX {1,2,1,0}
    #define SOLAIRE_YZYY {1,2,1,1}
    #define SOLAIRE_YZYZ {1,2,1,2}
    #define SOLAIRE_YZYW {1,2,1,3}
    #define SOLAIRE_YZZX {1,2,2,0}
    #define SOLAIRE_YZZY {1,2,2,1}
    #define SOLAIRE_YZZZ {1,2,2,2}
    #define SOLAIRE_YZZW {1,2,2,3}
    #define SOLAIRE_YZWX {1,2,3,0}
    #define SOLAIRE_YZWY {1,2,3,1}
    #define SOLAIRE_YZWZ {1,2,3,2}
    #define SOLAIRE_YZWW {1,2,3,3}
    #define SOLAIRE_YWXX {1,3,0,0}
    #define SOLAIRE_YWXY {1,3,0,1}
    #define SOLAIRE_YWXZ {1,3,0,2}
    #define SOLAIRE_YWXW {1,3,0,3}
    #define SOLAIRE_YWYX {1,3,1,0}
    #define SOLAIRE_YWYY {1,3,1,1}
    #define SOLAIRE_YWYZ {1,3,1,2}
    #define SOLAIRE_YWYW {1,3,1,3}
    #define SOLAIRE_YWZX {1,3,2,0}
    #define SOLAIRE_YWZY {1,3,2,1}
    #define SOLAIRE_YWZZ {1,3,2,2}
    #define SOLAIRE_YWZW {1,3,2,3}
    #define SOLAIRE_YWWX {1,3,3,0}
    #define SOLAIRE_YWWY {1,3,3,1}
    #define SOLAIRE_YWWZ {1,3,3,2}
    #define SOLAIRE_YWWW {1,3,3,3}
    #define SOLAIRE_ZXXX {2,0,0,0}
    #define SOLAIRE_ZXXY {2,0,0,1}
    #define SOLAIRE_ZXXZ {2,0,0,2}
    #define SOLAIRE_ZXXW {2,0,0,3}
    #define SOLAIRE_ZXYX {2,0,1,0}
    #define SOLAIRE_ZXYY {2,0,1,1}
    #define SOLAIRE_ZXYZ {2,0,1,2}
    #define SOLAIRE_ZXYW {2,0,1,3}
    #define SOLAIRE_ZXZX {2,0,2,0}
    #define SOLAIRE_ZXZY {2,0,2,1}
    #define SOLAIRE_ZXZZ {2,0,2,2}
    #define SOLAIRE_ZXZW {2,0,2,3}
    #define SOLAIRE_ZXWX {2,0,3,0}
    #define SOLAIRE_ZXWY {2,0,3,1}
    #define SOLAIRE_ZXWZ {2,0,3,2}
    #define SOLAIRE_ZXWW {2,0,3,3}
    #define SOLAIRE_ZYXX {2,1,0,0}
    #define SOLAIRE_ZYXY {2,1,0,1}
    #define SOLAIRE_ZYXZ {2,1,0,2}
    #define SOLAIRE_ZYXW {2,1,0,3}
    #define SOLAIRE_ZYYX {2,1,1,0}
    #define SOLAIRE_ZYYY {2,1,1,1}
    #define SOLAIRE_ZYYZ {2,1,1,2}
    #define SOLAIRE_ZYYW {2,1,1,3}
    #define SOLAIRE_ZYZX {2,1,2,0}
    #define SOLAIRE_ZYZY {2,1,2,1}
    #define SOLAIRE_ZYZZ {2,1,2,2}
    #define SOLAIRE_ZYZW {2,1,2,3}
    #define SOLAIRE_ZYWX {2,1,3,0}
    #define SOLAIRE_ZYWY {2,1,3,1}
    #define SOLAIRE_ZYWZ {2,1,3,2}
    #define SOLAIRE_ZYWW {2,1,3,3}
    #define SOLAIRE_ZZXX {2,2,0,0}
    #define SOLAIRE_ZZXY {2,2,0,1}
    #define SOLAIRE_ZZXZ {2,2,0,2}
    #define SOLAIRE_ZZXW {2,2,0,3}
    #define SOLAIRE_ZZYX {2,2,1,0}
    #define SOLAIRE_ZZYY {2,2,1,1}
    #define SOLAIRE_ZZYZ {2,2,1,2}
    #define SOLAIRE_ZZYW {2,2,1,3}
    #define SOLAIRE_ZZZX {2,2,2,0}
    #define SOLAIRE_ZZZY {2,2,2,1}
    #define SOLAIRE_ZZZZ {2,2,2,2}
    #define SOLAIRE_ZZZW {2,2,2,3}
    #define SOLAIRE_ZZWX {2,2,3,0}
    #define SOLAIRE_ZZWY {2,2,3,1}
    #define SOLAIRE_ZZWZ {2,2,3,2}
    #define SOLAIRE_ZZWW {2,2,3,3}
    #define SOLAIRE_ZWXX {2,3,0,0}
    #define SOLAIRE_ZWXY {2,3,0,1}
    #define SOLAIRE_ZWXZ {2,3,0,2}
    #define SOLAIRE_ZWXW {2,3,0,3}
    #define SOLAIRE_ZWYX {2,3,1,0}
    #define SOLAIRE_ZWYY {2,3,1,1}
    #define SOLAIRE_ZWYZ {2,3,1,2}
    #define SOLAIRE_ZWYW {2,3,1,3}
    #define SOLAIRE_ZWZX {2,3,2,0}
    #define SOLAIRE_ZWZY {2,3,2,1}
    #define SOLAIRE_ZWZZ {2,3,2,2}
    #define SOLAIRE_ZWZW {2,3,2,3}
    #define SOLAIRE_ZWWX {2,3,3,0}
    #define SOLAIRE_ZWWY {2,3,3,1}
    #define SOLAIRE_ZWWZ {2,3,3,2}
    #define SOLAIRE_ZWWW {2,3,3,3}
    #define SOLAIRE_WXXX {3,0,0,0}
    #define SOLAIRE_WXXY {3,0,0,1}
    #define SOLAIRE_WXXZ {3,0,0,2}
    #define SOLAIRE_WXXW {3,0,0,3}
    #define SOLAIRE_WXYX {3,0,1,0}
    #define SOLAIRE_WXYY {3,0,1,1}
    #define SOLAIRE_WXYZ {3,0,1,2}
    #define SOLAIRE_WXYW {3,0,1,3}
    #define SOLAIRE_WXZX {3,0,2,0}
    #define SOLAIRE_WXZY {3,0,2,1}
    #define SOLAIRE_WXZZ {3,0,2,2}
    #define SOLAIRE_WXZW {3,0,2,3}
    #define SOLAIRE_WXWX {3,0,3,0}
    #define SOLAIRE_WXWY {3,0,3,1}
    #define SOLAIRE_WXWZ {3,0,3,2}
    #define SOLAIRE_WXWW {3,0,3,3}
    #define SOLAIRE_WYXX {3,1,0,0}
    #define SOLAIRE_WYXY {3,1,0,1}
    #define SOLAIRE_WYXZ {3,1,0,2}
    #define SOLAIRE_WYXW {3,1,0,3}
    #define SOLAIRE_WYYX {3,1,1,0}
    #define SOLAIRE_WYYY {3,1,1,1}
    #define SOLAIRE_WYYZ {3,1,1,2}
    #define SOLAIRE_WYYW {3,1,1,3}
    #define SOLAIRE_WYZX {3,1,2,0}
    #define SOLAIRE_WYZY {3,1,2,1}
    #define SOLAIRE_WYZZ {3,1,2,2}
    #define SOLAIRE_WYZW {3,1,2,3}
    #define SOLAIRE_WYWX {3,1,3,0}
    #define SOLAIRE_WYWY {3,1,3,1}
    #define SOLAIRE_WYWZ {3,1,3,2}
    #define SOLAIRE_WYWW {3,1,3,3}
    #define SOLAIRE_WZXX {3,2,0,0}
    #define SOLAIRE_WZXY {3,2,0,1}
    #define SOLAIRE_WZXZ {3,2,0,2}
    #define SOLAIRE_WZXW {3,2,0,3}
    #define SOLAIRE_WZYX {3,2,1,0}
    #define SOLAIRE_WZYY {3,2,1,1}
    #define SOLAIRE_WZYZ {3,2,1,2}
    #define SOLAIRE_WZYW {3,2,1,3}
    #define SOLAIRE_WZZX {3,2,2,0}
    #define SOLAIRE_WZZY {3,2,2,1}
    #define SOLAIRE_WZZZ {3,2,2,2}
    #define SOLAIRE_WZZW {3,2,2,3}
    #define SOLAIRE_WZWX {3,2,3,0}
    #define SOLAIRE_WZWY {3,2,3,1}
    #define SOLAIRE_WZWZ {3,2,3,2}
    #define SOLAIRE_WZWW {3,2,3,3}
    #define SOLAIRE_WWXX {3,3,0,0}
    #define SOLAIRE_WWXY {3,3,0,1}
    #define SOLAIRE_WWXZ {3,3,0,2}
    #define SOLAIRE_WWXW {3,3,0,3}
    #define SOLAIRE_WWYX {3,3,1,0}
    #define SOLAIRE_WWYY {3,3,1,1}
    #define SOLAIRE_WWYZ {3,3,1,2}
    #define SOLAIRE_WWYW {3,3,1,3}
    #define SOLAIRE_WWZX {3,3,2,0}
    #define SOLAIRE_WWZY {3,3,2,1}
    #define SOLAIRE_WWZZ {3,3,2,2}
    #define SOLAIRE_WWZW {3,3,2,3}
    #define SOLAIRE_WWWX {3,3,3,0}
    #define SOLAIRE_WWWY {3,3,3,1}
    #define SOLAIRE_WWWZ {3,3,3,2}
    #define SOLAIRE_WWWW {3,3,3,3}
}


#endif
