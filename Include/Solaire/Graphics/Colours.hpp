#ifndef SOLAIRE_GRAPHICS_COLOURS_HPP
#define SOLAIRE_GRAPHICS_COLOURS_HPP

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
\file Image.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 21st September 2015
Last Modified	: 21st September 2015
*/

#include "Graphics.inl"

namespace Solaire{ namespace Graphics{ namespace Colours{

    enum{
        COUNT = 552
    };

    // Colour values and names come from table at http://cloford.com/resources/colours/500col.htm

    static constexpr ColourRGB IndianRed(205,92,92);
    static constexpr ColourRGB IndianRed1(255,106,106);
    static constexpr ColourRGB IndianRed2(238,99,99);
    static constexpr ColourRGB IndianRed4(139,58,58);
    static constexpr ColourRGB IndianRed3(205,85,85);
    static constexpr ColourRGB Crimson(220,20,60);
    static constexpr ColourRGB LightPink(255,182,193);
    static constexpr ColourRGB LightPink1(255,174,185);
    static constexpr ColourRGB LightPink2(238,162,173);
    static constexpr ColourRGB LightPink3(205,140,149);
    static constexpr ColourRGB LightPink4(139,95,101);
    static constexpr ColourRGB Pink(255,192,203);
    static constexpr ColourRGB Pink1(255,181,197);
    static constexpr ColourRGB Pink2(238,169,184);
    static constexpr ColourRGB Pink3(205,145,158);
    static constexpr ColourRGB Pink4(139,99,108);
    static constexpr ColourRGB PaleVioletRed(219,112,147);
    static constexpr ColourRGB PaleVioletRed1(255,130,171);
    static constexpr ColourRGB PaleVioletRed2(238,121,159);
    static constexpr ColourRGB PaleVioletRed3(205,104,137);
    static constexpr ColourRGB PaleVioletRed4(139,71,93);
    static constexpr ColourRGB lavenderblush1(255,240,245);
    static constexpr ColourRGB lavenderblush2(238,224,229);
    static constexpr ColourRGB lavenderblush3(205,193,197);
    static constexpr ColourRGB lavenderblush4(139,131,134);
    static constexpr ColourRGB VioletRed1(255,62,150);
    static constexpr ColourRGB VioletRed2(238,58,140);
    static constexpr ColourRGB VioletRed3(205,50,120);
    static constexpr ColourRGB VioletRed4(139,34,82);
    static constexpr ColourRGB HotPink(255,105,180);
    static constexpr ColourRGB HotPink1(255,110,180);
    static constexpr ColourRGB HotPink2(238,106,167);
    static constexpr ColourRGB HotPink3(205,96,144);
    static constexpr ColourRGB HotPink4(139,58,98);
    static constexpr ColourRGB Raspberry(135,38,87);
    static constexpr ColourRGB DeepPink1(255,20,147);
    static constexpr ColourRGB DeepPink2(238,18,137);
    static constexpr ColourRGB DeepPink3(205,16,118);
    static constexpr ColourRGB DeepPink4(139,10,80);
    static constexpr ColourRGB Maroon1(255,52,179);
    static constexpr ColourRGB Maroon2(238,48,167);
    static constexpr ColourRGB Maroon3(205,41,144);
    static constexpr ColourRGB Maroon4(139,28,98);
    static constexpr ColourRGB MediumVioletRed(199,21,133);
    static constexpr ColourRGB VioletRed(208,32,144);
    static constexpr ColourRGB Orchid(218,112,214);
    static constexpr ColourRGB Orchid1(255,131,250);
    static constexpr ColourRGB Orchid2(238,122,233);
    static constexpr ColourRGB Orchid3(205,105,201);
    static constexpr ColourRGB Orchid4(139,71,137);
    static constexpr ColourRGB Thistle(216,191,216);
    static constexpr ColourRGB Thistle1(255,225,255);
    static constexpr ColourRGB Thistle2(238,210,238);
    static constexpr ColourRGB Thistle3(205,181,205);
    static constexpr ColourRGB Thistle4(139,123,139);
    static constexpr ColourRGB Plum1(255,187,255);
    static constexpr ColourRGB Plum2(238,174,238);
    static constexpr ColourRGB Plum3(205,150,205);
    static constexpr ColourRGB Plum4(139,102,139);
    static constexpr ColourRGB Plum(221,160,221);
    static constexpr ColourRGB Violet(238,130,238);
    static constexpr ColourRGB Magenta(255,0,255);
    static constexpr ColourRGB Magenta2(238,0,238);
    static constexpr ColourRGB Magenta3(205,0,205);
    static constexpr ColourRGB Magenta4(139,0,139);
    static constexpr ColourRGB Purple(128,0,128);
    static constexpr ColourRGB MediumOrchid(186,85,211);
    static constexpr ColourRGB MediumOrchid1(224,102,255);
    static constexpr ColourRGB MediumOrchid2(209,95,238);
    static constexpr ColourRGB MediumOrchid3(180,82,205);
    static constexpr ColourRGB MediumOrchid4(122,55,139);
    static constexpr ColourRGB DarkViolet(148,0,211);
    static constexpr ColourRGB DarkOrchid(153,50,204);
    static constexpr ColourRGB DarkOrchid1(191,62,255);
    static constexpr ColourRGB DarkOrchid2(178,58,238);
    static constexpr ColourRGB DarkOrchid3(154,50,205);
    static constexpr ColourRGB DarkOrchid4(104,34,139);
    static constexpr ColourRGB Indigo(75,0,130);
    static constexpr ColourRGB BlueViolet(138,43,226);
    static constexpr ColourRGB Purple1(155,48,255);
    static constexpr ColourRGB Purple2(145,44,238);
    static constexpr ColourRGB Purple3(125,38,205);
    static constexpr ColourRGB Purple4(85,26,139);
    static constexpr ColourRGB MediumPurple(147,112,219);
    static constexpr ColourRGB MediumPurple1(171,130,255);
    static constexpr ColourRGB MediumPurple2(159,121,238);
    static constexpr ColourRGB MediumPurple3(137,104,205);
    static constexpr ColourRGB MediumPurple4(93,71,139);
    static constexpr ColourRGB DarkSlateBlue(72,61,139);
    static constexpr ColourRGB LightSlateBlue(132,112,255);
    static constexpr ColourRGB MediumSlateBlue(123,104,238);
    static constexpr ColourRGB SlateBlue(106,90,205);
    static constexpr ColourRGB SlateBlue1(131,111,255);
    static constexpr ColourRGB SlateBlue2(122,103,238);
    static constexpr ColourRGB SlateBlue3(105,89,205);
    static constexpr ColourRGB SlateBlue4(71,60,139);
    static constexpr ColourRGB GhostWhite(248,248,255);
    static constexpr ColourRGB lavender(230,230,250);
    static constexpr ColourRGB Blue(0,0,255);
    static constexpr ColourRGB Blue2(0,0,238);
    static constexpr ColourRGB Blue3(0,0,205);
    static constexpr ColourRGB Blue4(0,0,139);
    static constexpr ColourRGB Navy(0,0,128);
    static constexpr ColourRGB MidnightBlue(25,25,112);
    static constexpr ColourRGB Cobalt(61,89,171);
    static constexpr ColourRGB RoyalBlue(65,105,225);
    static constexpr ColourRGB RoyalBlue1(72,118,255);
    static constexpr ColourRGB RoyalBlue2(67,110,238);
    static constexpr ColourRGB RoyalBlue3(58,95,205);
    static constexpr ColourRGB RoyalBlue4(39,64,139);
    static constexpr ColourRGB cornflowerBlue(100,149,237);
    static constexpr ColourRGB LightSteelBlue(176,196,222);
    static constexpr ColourRGB LightSteelBlue1(202,225,255);
    static constexpr ColourRGB LightSteelBlue2(188,210,238);
    static constexpr ColourRGB LightSteelBlue3(162,181,205);
    static constexpr ColourRGB LightSteelBlue4(110,123,139);
    static constexpr ColourRGB LightSlateGrey(119,136,153);
    static constexpr ColourRGB SlateGrey(112,128,144);
    static constexpr ColourRGB SlateGrey1(198,226,255);
    static constexpr ColourRGB SlateGrey2(185,211,238);
    static constexpr ColourRGB SlateGrey3(159,182,205);
    static constexpr ColourRGB SlateGrey4(108,123,139);
    static constexpr ColourRGB DodgerBlue1(30,144,255);
    static constexpr ColourRGB DodgerBlue2(28,134,238);
    static constexpr ColourRGB DodgerBlue3(24,116,205);
    static constexpr ColourRGB DodgerBlue4(16,78,139);
    static constexpr ColourRGB AliceBlue(240,248,255);
    static constexpr ColourRGB SteelBlue(70,130,180);
    static constexpr ColourRGB SteelBlue1(99,184,255);
    static constexpr ColourRGB SteelBlue2(92,172,238);
    static constexpr ColourRGB SteelBlue3(79,148,205);
    static constexpr ColourRGB SteelBlue4(54,100,139);
    static constexpr ColourRGB LightSkyBlue(135,206,250);
    static constexpr ColourRGB LightSkyBlue1(176,226,255);
    static constexpr ColourRGB LightSkyBlue2(164,211,238);
    static constexpr ColourRGB LightSkyBlue3(141,182,205);
    static constexpr ColourRGB LightSkyBlue4(96,123,139);
    static constexpr ColourRGB SkyBlue1(135,206,255);
    static constexpr ColourRGB SkyBlue2(126,192,238);
    static constexpr ColourRGB SkyBlue3(108,166,205);
    static constexpr ColourRGB SkyBlue4(74,112,139);
    static constexpr ColourRGB SkyBlue(135,206,235);
    static constexpr ColourRGB DeepSkyBlue1(0,191,255);
    static constexpr ColourRGB DeepSkyBlue2(0,178,238);
    static constexpr ColourRGB DeepSkyBlue3(0,154,205);
    static constexpr ColourRGB DeepSkyBlue4(0,104,139);
    static constexpr ColourRGB Peacock(51,161,201);
    static constexpr ColourRGB LightBlue(173,216,230);
    static constexpr ColourRGB LightBlue1(191,239,255);
    static constexpr ColourRGB LightBlue2(178,223,238);
    static constexpr ColourRGB LightBlue3(154,192,205);
    static constexpr ColourRGB LightBlue4(104,131,139);
    static constexpr ColourRGB PowderBlue(176,224,230);
    static constexpr ColourRGB CadetBlue1(152,245,255);
    static constexpr ColourRGB CadetBlue2(142,229,238);
    static constexpr ColourRGB CadetBlue3(122,197,205);
    static constexpr ColourRGB CadetBlue4(83,134,139);
    static constexpr ColourRGB Turquoise1(0,245,255);
    static constexpr ColourRGB Turquoise2(0,229,238);
    static constexpr ColourRGB Turquoise3(0,197,205);
    static constexpr ColourRGB Turquoise4(0,134,139);
    static constexpr ColourRGB CadetBlue(95,158,160);
    static constexpr ColourRGB DarkTurquoise(0,206,209);
    static constexpr ColourRGB Azure1(240,255,255);
    static constexpr ColourRGB Azure2(224,238,238);
    static constexpr ColourRGB Azure3(193,205,205);
    static constexpr ColourRGB Azure4(131,139,139);
    static constexpr ColourRGB LightCyan1(224,255,255);
    static constexpr ColourRGB LightCyan2(209,238,238);
    static constexpr ColourRGB LightCyan3(180,205,205);
    static constexpr ColourRGB LightCyan4(122,139,139);
    static constexpr ColourRGB PaleTurquoise1(187,255,255);
    static constexpr ColourRGB PaleTurquoise2(174,238,238);
    static constexpr ColourRGB PaleTurquoise3(150,205,205);
    static constexpr ColourRGB PaleTurquoise4(102,139,139);
    static constexpr ColourRGB DarkSlateGrey(47,79,79);
    static constexpr ColourRGB DarkSlateGrey1(151,255,255);
    static constexpr ColourRGB DarkSlateGrey2(141,238,238);
    static constexpr ColourRGB DarkSlateGrey3(121,205,205);
    static constexpr ColourRGB DarkSlateGrey4(82,139,139);
    static constexpr ColourRGB Cyan1(0,255,255);
    static constexpr ColourRGB cyan2(0,238,238);
    static constexpr ColourRGB Cyan3(0,205,205);
    static constexpr ColourRGB Cyan4(0,139,139);
    static constexpr ColourRGB Teal(0,128,128);
    static constexpr ColourRGB MediumTurquoise(72,209,204);
    static constexpr ColourRGB LightSeaGreen(32,178,170);
    static constexpr ColourRGB ManganeseBlue(3,168,158);
    static constexpr ColourRGB Turquoise(64,224,208);
    static constexpr ColourRGB ColdGrey(128,138,135);
    static constexpr ColourRGB TurquoiseBlue(0,199,140);
    static constexpr ColourRGB Aquamarine1(127,255,212);
    static constexpr ColourRGB Aquamarine2(118,238,198);
    static constexpr ColourRGB Aquamarine3(102,205,170);
    static constexpr ColourRGB Aquamarine4(69,139,116);
    static constexpr ColourRGB MediumspringGreen(0,250,154);
    static constexpr ColourRGB MintCream(245,255,250);
    static constexpr ColourRGB SpringGreen(0,255,127);
    static constexpr ColourRGB SpringGreen1(0,238,118);
    static constexpr ColourRGB SpringGreen2(0,205,102);
    static constexpr ColourRGB SpringGreen3(0,139,69);
    static constexpr ColourRGB MediumSeaGreen(60,179,113);
    static constexpr ColourRGB SeaGreen1(84,255,159);
    static constexpr ColourRGB SeaGreen2(78,238,148);
    static constexpr ColourRGB SeaGreen3(67,205,128);
    static constexpr ColourRGB SeaGreen4(46,139,87);
    static constexpr ColourRGB EmeraldGreen(0,201,87);
    static constexpr ColourRGB Mint(189,252,201);
    static constexpr ColourRGB CobaltGreen(61,145,64);
    static constexpr ColourRGB Honeydew1(240,255,240);
    static constexpr ColourRGB Honeydew2(224,238,224);
    static constexpr ColourRGB Honeydew3(193,205,193);
    static constexpr ColourRGB Honeydew4(131,139,131);
    static constexpr ColourRGB DarkSeaGreen(143,188,143);
    static constexpr ColourRGB DarkSeaGreen1(193,255,193);
    static constexpr ColourRGB DarkSeaGreen2(180,238,180);
    static constexpr ColourRGB DarkSeaGreen3(155,205,155);
    static constexpr ColourRGB DarkSeaGreen4(105,139,105);
    static constexpr ColourRGB PaleGreen(152,251,152);
    static constexpr ColourRGB PaleGreen1(154,255,154);
    static constexpr ColourRGB PaleGreen2(144,238,144);
    static constexpr ColourRGB PaleGreen3(124,205,124);
    static constexpr ColourRGB PaleGreen4(84,139,84);
    static constexpr ColourRGB LimeGreen(50,205,50);
    static constexpr ColourRGB ForestGreen(34,139,34);
    static constexpr ColourRGB Green1(0,255,0);
    static constexpr ColourRGB Green2(0,238,0);
    static constexpr ColourRGB Green3(0,205,0);
    static constexpr ColourRGB Green4(0,139,0);
    static constexpr ColourRGB Green(0,128,0);
    static constexpr ColourRGB DarkGreen(0,100,0);
    static constexpr ColourRGB SapGreen(48,128,20);
    static constexpr ColourRGB LawnGreen(124,252,0);
    static constexpr ColourRGB Chartreuse1(127,255,0);
    static constexpr ColourRGB Chartreuse2(118,238,0);
    static constexpr ColourRGB Chartreuse3(102,205,0);
    static constexpr ColourRGB Chartreuse4(69,139,0);
    static constexpr ColourRGB GreenYellow(173,255,47);
    static constexpr ColourRGB DarkOliveGreen1(202,255,112);
    static constexpr ColourRGB DarkOliveGreen2(188,238,104);
    static constexpr ColourRGB DarkOliveGreen3(162,205,90);
    static constexpr ColourRGB DarkOliveGreen4(110,139,61);
    static constexpr ColourRGB DarkOliveGreen(85,107,47);
    static constexpr ColourRGB OliveDrab(107,142,35);
    static constexpr ColourRGB OliveDrab1(192,255,62);
    static constexpr ColourRGB OliveDrab2(179,238,58);
    static constexpr ColourRGB OliveDrab3(154,205,50);
    static constexpr ColourRGB OliveDrab4(105,139,34);
    static constexpr ColourRGB Ivory1(255,255,240);
    static constexpr ColourRGB Ivory2(238,238,224);
    static constexpr ColourRGB Ivory3(205,205,193);
    static constexpr ColourRGB Ivory4(139,139,131);
    static constexpr ColourRGB beige(245,245,220);
    static constexpr ColourRGB LightYellow1(255,255,224);
    static constexpr ColourRGB LightYellow2(238,238,209);
    static constexpr ColourRGB LightYellow3(205,205,180);
    static constexpr ColourRGB LightYellow4(139,139,122);
    static constexpr ColourRGB LightGoldenrodYellow(250,250,210);
    static constexpr ColourRGB Yellow1(255,255,0);
    static constexpr ColourRGB Yellow2(238,238,0);
    static constexpr ColourRGB Yellow3(205,205,0);
    static constexpr ColourRGB Yellow4(139,139,0);
    static constexpr ColourRGB WarmGrey(128,128,105);
    static constexpr ColourRGB Olive(128,128,0);
    static constexpr ColourRGB DarkKhaki(189,183,107);
    static constexpr ColourRGB Khaki1(255,246,143);
    static constexpr ColourRGB Khaki2(238,230,133);
    static constexpr ColourRGB Khaki3(205,198,115);
    static constexpr ColourRGB Khaki4(139,134,78);
    static constexpr ColourRGB Khaki(240,230,140);
    static constexpr ColourRGB PaleGoldenrod(238,232,170);
    static constexpr ColourRGB Lemonchiffon1(255,250,205);
    static constexpr ColourRGB Lemonchiffon2(238,233,191);
    static constexpr ColourRGB Lemonchiffon3(205,201,165);
    static constexpr ColourRGB Lemonchiffon4(139,137,112);
    static constexpr ColourRGB LightGoldenrod1(255,236,139);
    static constexpr ColourRGB LightGoldenrod2(238,220,130);
    static constexpr ColourRGB LightGoldenrod3(205,190,112);
    static constexpr ColourRGB LightGoldenrod4(139,129,76);
    static constexpr ColourRGB Banana(227,207,87);
    static constexpr ColourRGB Gold1(255,215,0);
    static constexpr ColourRGB Gold2(238,201,0);
    static constexpr ColourRGB Gold3(205,173,0);
    static constexpr ColourRGB Gold4(139,117,0);
    static constexpr ColourRGB Cornsilk1(255,248,220);
    static constexpr ColourRGB Cornsilk2(238,232,205);
    static constexpr ColourRGB Cornsilk3(205,200,177);
    static constexpr ColourRGB Cornsilk4(139,136,120);
    static constexpr ColourRGB Goldenrod(218,165,32);
    static constexpr ColourRGB Goldenrod1(255,193,37);
    static constexpr ColourRGB Goldenrod2(238,180,34);
    static constexpr ColourRGB Goldenrod3(205,155,29);
    static constexpr ColourRGB Goldenrod4(139,105,20);
    static constexpr ColourRGB DarkGoldenrod(184,134,11);
    static constexpr ColourRGB DarkGoldenrod1(255,185,15);
    static constexpr ColourRGB DarkGoldenrod2(238,173,14);
    static constexpr ColourRGB DarkGoldenrod3(205,149,12);
    static constexpr ColourRGB DarkGoldenrod4(139,101,8);
    static constexpr ColourRGB Orange1(255,165,0);
    static constexpr ColourRGB Orange2(238,154,0);
    static constexpr ColourRGB Orange3(205,133,0);
    static constexpr ColourRGB Orange4(139,90,0);
    static constexpr ColourRGB FloralWhite(255,250,240);
    static constexpr ColourRGB Oldlace(253,245,230);
    static constexpr ColourRGB Wheat(245,222,179);
    static constexpr ColourRGB Wheat1(255,231,186);
    static constexpr ColourRGB Wheat2(238,216,174);
    static constexpr ColourRGB Wheat3(205,186,150);
    static constexpr ColourRGB Wheat4(139,126,102);
    static constexpr ColourRGB Moccasin(255,228,181);
    static constexpr ColourRGB Papayawhip(255,239,213);
    static constexpr ColourRGB Blanchedalmond(255,235,205);
    static constexpr ColourRGB NavajoWhite1(255,222,173);
    static constexpr ColourRGB NavajoWhite2(238,207,161);
    static constexpr ColourRGB NavajoWhite3(205,179,139);
    static constexpr ColourRGB NavajoWhite4(139,121,94);
    static constexpr ColourRGB Eggshell(252,230,201);
    static constexpr ColourRGB Tan(210,180,140);
    static constexpr ColourRGB brick(156,102,31);
    static constexpr ColourRGB CadmiumYellow(255,153,18);
    static constexpr ColourRGB AntiqueWhite(250,235,215);
    static constexpr ColourRGB AntiqueWhite1(255,239,219);
    static constexpr ColourRGB AntiqueWhite2(238,223,204);
    static constexpr ColourRGB AntiqueWhite3(205,192,176);
    static constexpr ColourRGB AntiqueWhite4(139,131,120);
    static constexpr ColourRGB BurlyWood(222,184,135);
    static constexpr ColourRGB BurlyWood1(255,211,155);
    static constexpr ColourRGB BurlyWood2(238,197,145);
    static constexpr ColourRGB BurlyWood3(205,170,125);
    static constexpr ColourRGB BurlyWood4(139,115,85);
    static constexpr ColourRGB Bisque1(255,228,196);
    static constexpr ColourRGB Bisque2(238,213,183);
    static constexpr ColourRGB Bisque3(205,183,158);
    static constexpr ColourRGB Bisque4(139,125,107);
    static constexpr ColourRGB Melon(227,168,105);
    static constexpr ColourRGB Carrot(237,145,33);
    static constexpr ColourRGB DarkOrange(255,140,0);
    static constexpr ColourRGB DarkOrange1(255,127,0);
    static constexpr ColourRGB DarkOrange2(238,118,0);
    static constexpr ColourRGB DarkOrange3(205,102,0);
    static constexpr ColourRGB DarkOrange4(139,69,0);
    static constexpr ColourRGB Orange(255,128,0);
    static constexpr ColourRGB Tan1(255,165,79);
    static constexpr ColourRGB Tan2(238,154,73);
    static constexpr ColourRGB Tan3(205,133,63);
    static constexpr ColourRGB Tan4(139,90,43);
    static constexpr ColourRGB Linen(250,240,230);
    static constexpr ColourRGB Peachpuff1(255,218,185);
    static constexpr ColourRGB Peachpuff2(238,203,173);
    static constexpr ColourRGB Peachpuff3(205,175,149);
    static constexpr ColourRGB Peachpuff4(139,119,101);
    static constexpr ColourRGB Seashell1(255,245,238);
    static constexpr ColourRGB Seashell2(238,229,222);
    static constexpr ColourRGB Seashell3(205,197,191);
    static constexpr ColourRGB Seashell4(139,134,130);
    static constexpr ColourRGB SandyBrown(244,164,96);
    static constexpr ColourRGB RawSienna(199,97,20);
    static constexpr ColourRGB Chocolate(210,105,30);
    static constexpr ColourRGB Chocolate1(255,127,36);
    static constexpr ColourRGB Chocolate2(238,118,33);
    static constexpr ColourRGB Chocolate3(205,102,29);
    static constexpr ColourRGB Chocolate4(139,69,19);
    static constexpr ColourRGB IvoryBlack(41,36,33);
    static constexpr ColourRGB Flesh(255,125,64);
    static constexpr ColourRGB CadmiumOrange(255,97,3);
    static constexpr ColourRGB BurntSienna(138,54,15);
    static constexpr ColourRGB Sienna(160,82,45);
    static constexpr ColourRGB Sienna1(255,130,71);
    static constexpr ColourRGB Sienna2(238,121,66);
    static constexpr ColourRGB Sienna3(205,104,57);
    static constexpr ColourRGB Sienna4(139,71,38);
    static constexpr ColourRGB LightSalmon1(255,160,122);
    static constexpr ColourRGB LightSalmon2(238,149,114);
    static constexpr ColourRGB LightSalmon3(205,129,98);
    static constexpr ColourRGB LightSalmon4(139,87,66);
    static constexpr ColourRGB Coral(255,127,80);
    static constexpr ColourRGB OrangeRed1(255,69,0);
    static constexpr ColourRGB OrangeRed2(238,64,0);
    static constexpr ColourRGB OrangeRed3(205,55,0);
    static constexpr ColourRGB orangeRed4(139,37,0);
    static constexpr ColourRGB Sepia(94,38,18);
    static constexpr ColourRGB DarkSalmon(233,150,122);
    static constexpr ColourRGB Salmon1(255,140,105);
    static constexpr ColourRGB Salmon2(238,130,98);
    static constexpr ColourRGB Salmon3(205,112,84);
    static constexpr ColourRGB Salmon4(139,76,57);
    static constexpr ColourRGB Coral1(255,114,86);
    static constexpr ColourRGB Coral2(238,106,80);
    static constexpr ColourRGB Coral3(205,91,69);
    static constexpr ColourRGB Coral4(139,62,47);
    static constexpr ColourRGB BurnTumber(138,51,36);
    static constexpr ColourRGB Tomato1(255,99,71);
    static constexpr ColourRGB Tomato2(238,92,66);
    static constexpr ColourRGB Tomato3(205,79,57);
    static constexpr ColourRGB Tomato4(139,54,38);
    static constexpr ColourRGB Salmon(250,128,114);
    static constexpr ColourRGB MistyRose1(255,228,225);
    static constexpr ColourRGB MistyRose2(238,213,210);
    static constexpr ColourRGB MistyRose3(205,183,181);
    static constexpr ColourRGB MistyRose4(139,125,123);
    static constexpr ColourRGB Snow1(255,250,250);
    static constexpr ColourRGB Snow2(238,233,233);
    static constexpr ColourRGB Snow3(205,201,201);
    static constexpr ColourRGB Snow4(139,137,137);
    static constexpr ColourRGB RosyBrown(188,143,143);
    static constexpr ColourRGB RosyBrown1(255,193,193);
    static constexpr ColourRGB RosyBrown2(238,180,180);
    static constexpr ColourRGB RosyBrown3(205,155,155);
    static constexpr ColourRGB RosyBrown4(139,105,105);
    static constexpr ColourRGB LightCoral(240,128,128);
    static constexpr ColourRGB Brown(165,42,42);
    static constexpr ColourRGB Brown1(255,64,64);
    static constexpr ColourRGB Brown2(238,59,59);
    static constexpr ColourRGB Brown3(205,51,51);
    static constexpr ColourRGB Brown4(139,35,35);
    static constexpr ColourRGB FireBrick(178,34,34);
    static constexpr ColourRGB FireBrick1(255,48,48);
    static constexpr ColourRGB FireBrick2(238,44,44);
    static constexpr ColourRGB FireBrick3(205,38,38);
    static constexpr ColourRGB FireBrick4(139,26,26);
    static constexpr ColourRGB Red1(255,0,0);
    static constexpr ColourRGB Red2(238,0,0);
    static constexpr ColourRGB Red3(205,0,0);
    static constexpr ColourRGB Red4(139,0,0);
    static constexpr ColourRGB Maroon(128,0,0);
    static constexpr ColourRGB SGIBeet(142,56,142);
    static constexpr ColourRGB SGISlateBlue(113,113,198);
    static constexpr ColourRGB SGILightBlue(125,158,192);
    static constexpr ColourRGB SGITeal(56,142,142);
    static constexpr ColourRGB SGIChartreuse(113,198,113);
    static constexpr ColourRGB SGIOliveDrab(142,142,56);
    static constexpr ColourRGB SGIbrightGrey(197,193,170);
    static constexpr ColourRGB SGISalmon(198,113,113);
    static constexpr ColourRGB SGIDarkGrey(85,85,85);
    static constexpr ColourRGB SGIGrey12(30,30,30);
    static constexpr ColourRGB SGIGrey16(40,40,40);
    static constexpr ColourRGB SGIGrey32(81,81,81);
    static constexpr ColourRGB SGIGrey36(91,91,91);
    static constexpr ColourRGB SGIGrey52(132,132,132);
    static constexpr ColourRGB SGIGrey56(142,142,142);
    static constexpr ColourRGB SGILightGrey(170,170,170);
    static constexpr ColourRGB SGIGrey72(183,183,183);
    static constexpr ColourRGB SGIGrey76(193,193,193);
    static constexpr ColourRGB SGIGrey92(234,234,234);
    static constexpr ColourRGB SGIGrey96(244,244,244);
    static constexpr ColourRGB White(255,255,255);
    static constexpr ColourRGB WhiteSmoke(245,245,245);
    static constexpr ColourRGB Gainsboro(220,220,220);
    static constexpr ColourRGB LightGrey(211,211,211);
    static constexpr ColourRGB Silver(192,192,192);
    static constexpr ColourRGB DarkGrey(169,169,169);
    static constexpr ColourRGB Grey(128,128,128);
    static constexpr ColourRGB DimGrey(105,105,105);
    static constexpr ColourRGB Black(0,0,0);
    static constexpr ColourRGB Grey99(252,252,252);
    static constexpr ColourRGB Grey98(250,250,250);
    static constexpr ColourRGB Grey97(247,247,247);
    static constexpr ColourRGB Grey95(242,242,242);
    static constexpr ColourRGB Grey94(240,240,240);
    static constexpr ColourRGB Grey93(237,237,237);
    static constexpr ColourRGB Grey92(235,235,235);
    static constexpr ColourRGB Grey91(232,232,232);
    static constexpr ColourRGB Grey90(229,229,229);
    static constexpr ColourRGB Grey89(227,227,227);
    static constexpr ColourRGB Grey88(224,224,224);
    static constexpr ColourRGB Grey87(222,222,222);
    static constexpr ColourRGB Grey86(219,219,219);
    static constexpr ColourRGB Grey85(217,217,217);
    static constexpr ColourRGB Grey84(214,214,214);
    static constexpr ColourRGB Grey83(212,212,212);
    static constexpr ColourRGB Grey82(209,209,209);
    static constexpr ColourRGB Grey81(207,207,207);
    static constexpr ColourRGB Grey80(204,204,204);
    static constexpr ColourRGB Grey79(201,201,201);
    static constexpr ColourRGB Grey78(199,199,199);
    static constexpr ColourRGB Grey77(196,196,196);
    static constexpr ColourRGB Grey76(194,194,194);
    static constexpr ColourRGB Grey75(191,191,191);
    static constexpr ColourRGB Grey74(189,189,189);
    static constexpr ColourRGB Grey73(186,186,186);
    static constexpr ColourRGB Grey72(184,184,184);
    static constexpr ColourRGB Grey71(181,181,181);
    static constexpr ColourRGB Grey70(179,179,179);
    static constexpr ColourRGB Grey69(176,176,176);
    static constexpr ColourRGB Grey68(173,173,173);
    static constexpr ColourRGB Grey67(171,171,171);
    static constexpr ColourRGB Grey66(168,168,168);
    static constexpr ColourRGB Grey65(166,166,166);
    static constexpr ColourRGB Grey64(163,163,163);
    static constexpr ColourRGB Grey63(161,161,161);
    static constexpr ColourRGB Grey62(158,158,158);
    static constexpr ColourRGB Grey61(156,156,156);
    static constexpr ColourRGB Grey60(153,153,153);
    static constexpr ColourRGB Grey59(150,150,150);
    static constexpr ColourRGB Grey58(148,148,148);
    static constexpr ColourRGB Grey57(145,145,145);
    static constexpr ColourRGB Grey56(143,143,143);
    static constexpr ColourRGB Grey55(140,140,140);
    static constexpr ColourRGB Grey54(138,138,138);
    static constexpr ColourRGB Grey53(135,135,135);
    static constexpr ColourRGB Grey52(133,133,133);
    static constexpr ColourRGB Grey51(130,130,130);
    static constexpr ColourRGB Grey50(127,127,127);
    static constexpr ColourRGB Grey49(125,125,125);
    static constexpr ColourRGB Grey48(122,122,122);
    static constexpr ColourRGB Grey47(120,120,120);
    static constexpr ColourRGB Grey46(117,117,117);
    static constexpr ColourRGB Grey45(115,115,115);
    static constexpr ColourRGB Grey44(112,112,112);
    static constexpr ColourRGB Grey43(110,110,110);
    static constexpr ColourRGB Grey42(107,107,107);
    static constexpr ColourRGB Grey41(105,105,105);
    static constexpr ColourRGB Grey40(102,102,102);
    static constexpr ColourRGB Grey39(99,99,99);
    static constexpr ColourRGB Grey38(97,97,97);
    static constexpr ColourRGB Grey37(94,94,94);
    static constexpr ColourRGB Grey36(92,92,92);
    static constexpr ColourRGB Grey35(89,89,89);
    static constexpr ColourRGB Grey34(87,87,87);
    static constexpr ColourRGB Grey33(84,84,84);
    static constexpr ColourRGB Grey32(82,82,82);
    static constexpr ColourRGB Grey31(79,79,79);
    static constexpr ColourRGB Grey30(77,77,77);
    static constexpr ColourRGB Grey29(74,74,74);
    static constexpr ColourRGB Grey28(71,71,71);
    static constexpr ColourRGB Grey27(69,69,69);
    static constexpr ColourRGB Grey26(66,66,66);
    static constexpr ColourRGB Grey25(64,64,64);
    static constexpr ColourRGB Grey24(61,61,61);
    static constexpr ColourRGB Grey23(59,59,59);
    static constexpr ColourRGB Grey22(56,56,56);
    static constexpr ColourRGB Grey21(54,54,54);
    static constexpr ColourRGB Grey20(51,51,51);
    static constexpr ColourRGB Grey19(48,48,48);
    static constexpr ColourRGB Grey18(46,46,46);
    static constexpr ColourRGB Grey17(43,43,43);
    static constexpr ColourRGB Grey16(41,41,41);
    static constexpr ColourRGB Grey15(38,38,38);
    static constexpr ColourRGB Grey14(36,36,36);
    static constexpr ColourRGB Grey13(33,33,33);
    static constexpr ColourRGB Grey12(31,31,31);
    static constexpr ColourRGB Grey11(28,28,28);
    static constexpr ColourRGB Grey10(26,26,26);
    static constexpr ColourRGB Grey9(23,23,23);
    static constexpr ColourRGB Grey8(20,20,20);
    static constexpr ColourRGB Grey7(18,18,18);
    static constexpr ColourRGB Grey6(15,15,15);
    static constexpr ColourRGB Grey5(13,13,13);
    static constexpr ColourRGB Grey4(10,10,10);
    static constexpr ColourRGB Grey3(8,8,8);
    static constexpr ColourRGB Grey2(5,5,5);
    static constexpr ColourRGB Grey1(3,3,3);
}}}

#endif
