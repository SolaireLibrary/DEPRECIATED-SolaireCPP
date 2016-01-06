#ifndef SOLAIRE_HASH_CRC_HPP
#define SOLAIRE_HASH_CRC_HPP

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
\file Crc.hpp
\brief
\author
Created			: Adam Smith
Last modified	: Adam Smith
\version 1.0
\date
Created			: 1st October 2015
Last Modified	: 1st October 2015
*/

#include "HashFunction.hpp"
#include "..\Reflect.hpp"

namespace Solaire {

    template<class T, const T POLYNOMIAL, const T INITIAL_REMAINDER, const T FINAL_XOR_VALUE, const bool REFLECT_DATA, const bool REFLECT_REMAINDER>
    class Crc : public HashFunction<T> {
    private:
        enum : T{
            WIDTH = 8 * sizeof(T),
            TOPBIT  = static_cast<T>(1 << (WIDTH - 1))
        };

        static constexpr T CalculateCrcBit(const T aRemainder, const uint8_t aBit){
            return aRemainder & TOPBIT ?
                (aRemainder << 1) ^ POLYNOMIAL :
                aRemainder << 1;
        }

        static constexpr T CalculateCrc(const T aDividend){
            return
            CalculateCrcBit(
                CalculateCrcBit(
                    CalculateCrcBit(
                        CalculateCrcBit(
                            CalculateCrcBit(
                                CalculateCrcBit(
                                    CalculateCrcBit(
                                        CalculateCrcBit(
                                            aDividend << (WIDTH - 8),
                                        7),
                                    6),
                                5),
                            4),
                        3),
                    2),
                1),
            0);
        }

        static constexpr T CRC_TABLE[256] ={
            CalculateCrc(0),	CalculateCrc(1),	CalculateCrc(2),	CalculateCrc(3),	CalculateCrc(4),	CalculateCrc(5),
            CalculateCrc(6),	CalculateCrc(7),	CalculateCrc(8),	CalculateCrc(9),	CalculateCrc(10),	CalculateCrc(11),
            CalculateCrc(12),	CalculateCrc(13),	CalculateCrc(14),	CalculateCrc(15),	CalculateCrc(16),	CalculateCrc(17),
            CalculateCrc(18),	CalculateCrc(19),	CalculateCrc(20),	CalculateCrc(21),	CalculateCrc(22),	CalculateCrc(23),
            CalculateCrc(24),	CalculateCrc(25),	CalculateCrc(26),	CalculateCrc(27),	CalculateCrc(28),	CalculateCrc(29),
            CalculateCrc(30),	CalculateCrc(31),	CalculateCrc(32),	CalculateCrc(33),	CalculateCrc(34),	CalculateCrc(35),
            CalculateCrc(36),	CalculateCrc(37),	CalculateCrc(38),	CalculateCrc(39),	CalculateCrc(40),	CalculateCrc(41),
            CalculateCrc(42),	CalculateCrc(43),	CalculateCrc(44),	CalculateCrc(45),	CalculateCrc(46),	CalculateCrc(47),
            CalculateCrc(48),	CalculateCrc(49),	CalculateCrc(50),	CalculateCrc(51),	CalculateCrc(52),	CalculateCrc(53),
            CalculateCrc(54),	CalculateCrc(55),	CalculateCrc(56),	CalculateCrc(57),	CalculateCrc(58),	CalculateCrc(59),
            CalculateCrc(60),	CalculateCrc(61),	CalculateCrc(62),	CalculateCrc(63),	CalculateCrc(64),	CalculateCrc(65),
            CalculateCrc(66),	CalculateCrc(67),	CalculateCrc(68),	CalculateCrc(69),	CalculateCrc(70),	CalculateCrc(71),
            CalculateCrc(72),	CalculateCrc(73),	CalculateCrc(74),	CalculateCrc(75),	CalculateCrc(76),	CalculateCrc(77),
            CalculateCrc(78),	CalculateCrc(79),	CalculateCrc(80),	CalculateCrc(81),	CalculateCrc(82),	CalculateCrc(83),
            CalculateCrc(84),	CalculateCrc(85),	CalculateCrc(86),	CalculateCrc(87),	CalculateCrc(88),	CalculateCrc(89),
            CalculateCrc(90),	CalculateCrc(91),	CalculateCrc(92),	CalculateCrc(93),	CalculateCrc(94),	CalculateCrc(95),
            CalculateCrc(96),	CalculateCrc(97),	CalculateCrc(98),	CalculateCrc(99),	CalculateCrc(100),	CalculateCrc(101),
            CalculateCrc(102),	CalculateCrc(103),	CalculateCrc(104),	CalculateCrc(105),	CalculateCrc(106),	CalculateCrc(107),
            CalculateCrc(108),	CalculateCrc(109),	CalculateCrc(110),	CalculateCrc(111),	CalculateCrc(112),	CalculateCrc(113),
            CalculateCrc(114),	CalculateCrc(115),	CalculateCrc(116),	CalculateCrc(117),	CalculateCrc(118),	CalculateCrc(119),
            CalculateCrc(120),	CalculateCrc(121),	CalculateCrc(122),	CalculateCrc(123),	CalculateCrc(124),	CalculateCrc(125),
            CalculateCrc(126),	CalculateCrc(127),	CalculateCrc(128),	CalculateCrc(129),	CalculateCrc(130),	CalculateCrc(131),
            CalculateCrc(132),	CalculateCrc(133),	CalculateCrc(134),	CalculateCrc(135),	CalculateCrc(136),	CalculateCrc(137),
            CalculateCrc(138),	CalculateCrc(139),	CalculateCrc(140),	CalculateCrc(141),	CalculateCrc(142),	CalculateCrc(143),
            CalculateCrc(144),	CalculateCrc(145),	CalculateCrc(146),	CalculateCrc(147),	CalculateCrc(148),	CalculateCrc(149),
            CalculateCrc(150),	CalculateCrc(151),	CalculateCrc(152),	CalculateCrc(153),	CalculateCrc(154),	CalculateCrc(155),
            CalculateCrc(156),	CalculateCrc(157),	CalculateCrc(158),	CalculateCrc(159),	CalculateCrc(160),	CalculateCrc(161),
            CalculateCrc(162),	CalculateCrc(163),	CalculateCrc(164),	CalculateCrc(165),	CalculateCrc(166),	CalculateCrc(167),
            CalculateCrc(168),	CalculateCrc(169),	CalculateCrc(170),	CalculateCrc(171),	CalculateCrc(172),	CalculateCrc(173),
            CalculateCrc(174),	CalculateCrc(175),	CalculateCrc(176),	CalculateCrc(177),	CalculateCrc(178),	CalculateCrc(179),
            CalculateCrc(180),	CalculateCrc(181),	CalculateCrc(182),	CalculateCrc(183),	CalculateCrc(184),	CalculateCrc(185),
            CalculateCrc(186),	CalculateCrc(187),	CalculateCrc(188),	CalculateCrc(189),	CalculateCrc(190),	CalculateCrc(191),
            CalculateCrc(192),	CalculateCrc(193),	CalculateCrc(194),	CalculateCrc(195),	CalculateCrc(196),	CalculateCrc(197),
            CalculateCrc(198),	CalculateCrc(199),	CalculateCrc(200),	CalculateCrc(201),	CalculateCrc(202),	CalculateCrc(203),
            CalculateCrc(204),	CalculateCrc(205),	CalculateCrc(206),	CalculateCrc(207),	CalculateCrc(208),	CalculateCrc(209),
            CalculateCrc(210),	CalculateCrc(211),	CalculateCrc(212),	CalculateCrc(213),	CalculateCrc(214),	CalculateCrc(215),
            CalculateCrc(216),	CalculateCrc(217),	CalculateCrc(218),	CalculateCrc(219),	CalculateCrc(220),	CalculateCrc(221),
            CalculateCrc(222),	CalculateCrc(223),	CalculateCrc(224),	CalculateCrc(225),	CalculateCrc(226),	CalculateCrc(227),
            CalculateCrc(228),	CalculateCrc(229),	CalculateCrc(230),	CalculateCrc(231),	CalculateCrc(232),	CalculateCrc(233),
            CalculateCrc(234),	CalculateCrc(235),	CalculateCrc(236),	CalculateCrc(237),	CalculateCrc(238),	CalculateCrc(239),
            CalculateCrc(240),	CalculateCrc(241),	CalculateCrc(242),	CalculateCrc(243),	CalculateCrc(244),	CalculateCrc(245),
            CalculateCrc(246),	CalculateCrc(247),	CalculateCrc(248),	CalculateCrc(249),	CalculateCrc(250),	CalculateCrc(251),
            CalculateCrc(252),	CalculateCrc(253),	CalculateCrc(254),	CalculateCrc(255)
        };
    public:
        // Inherited from HashFunction
        T SOLAIRE_EXPORT_CALL Hash(const void* const aValue, const size_t aBytes) const throw() override{
			const uint8_t* ptr = static_cast<const uint8_t*>(aValue);
			const uint8_t* const end = ptr + aBytes;

			T remainder = INITIAL_REMAINDER;

            while(ptr != end){
                uint8_t data = *(ptr++);
                if(REFLECT_DATA) data = Reflect<uint8_t>(data);
                data ^= remainder >> (WIDTH - 8);
                remainder = CRC_TABLE[data] ^ (remainder << 8);
            }

            return (REFLECT_REMAINDER ? Reflect<T>(remainder): remainder) ^ FINAL_XOR_VALUE;
        }
    };

    template<class T, const T POLYNOMIAL, const T INITIAL_REMAINDER, const T FINAL_XOR_VALUE, const bool REFLECT_DATA, const bool REFLECT_REMAINDER>
    constexpr T Crc<T, POLYNOMIAL, INITIAL_REMAINDER, FINAL_XOR_VALUE, REFLECT_DATA, REFLECT_REMAINDER>::CRC_TABLE[256];

    typedef Crc<uint16_t, 0x1021,       0xFFFF,     0x0000,         false,  false>  CrcCcitt;
    typedef Crc<uint16_t, 0x8005,       0x0000,     0x0000,         true,   true>   Crc16;
    typedef Crc<uint32_t, 0x04C11DB7,   0xFFFFFFFF, 0xFFFFFFFF,     true,   true>   Crc32;
}

#endif
