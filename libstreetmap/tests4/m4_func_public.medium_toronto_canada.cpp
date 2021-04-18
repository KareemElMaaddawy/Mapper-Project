#include <random>
#include <iostream>
#include <UnitTest++/UnitTest++.h>

#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"
#include "m4.h"

#include "unit_test_util.h"
#include "courier_verify.h"

using ece297test::relative_error;
using ece297test::courier_path_is_legal;


SUITE(medium_toronto_canada_public) {
    TEST(medium_toronto_canada) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(18428, 13180), DeliveryInf(115112, 73209), DeliveryInf(7832, 68916), DeliveryInf(88253, 105659), DeliveryInf(114780, 99475), DeliveryInf(24143, 104773), DeliveryInf(36647, 40478), DeliveryInf(66440, 80441), DeliveryInf(60411, 3042), DeliveryInf(70817, 74187), DeliveryInf(116429, 58215), DeliveryInf(148729, 120472), DeliveryInf(43487, 120679), DeliveryInf(48026, 29698), DeliveryInf(147089, 89864), DeliveryInf(95180, 100371), DeliveryInf(56819, 114599), DeliveryInf(99955, 38918), DeliveryInf(137276, 51489), DeliveryInf(9037, 116379), DeliveryInf(113021, 88823), DeliveryInf(147775, 46252), DeliveryInf(57606, 124878), DeliveryInf(65531, 24659), DeliveryInf(9088, 78160), DeliveryInf(50955, 56686), DeliveryInf(145225, 12428), DeliveryInf(36147, 131937), DeliveryInf(26717, 146131), DeliveryInf(76847, 22635), DeliveryInf(28583, 47950), DeliveryInf(56077, 115825), DeliveryInf(90722, 96421), DeliveryInf(106836, 53224), DeliveryInf(49900, 53919), DeliveryInf(55780, 91575), DeliveryInf(110047, 147042), DeliveryInf(128368, 75909), DeliveryInf(26866, 84826), DeliveryInf(91958, 50187), DeliveryInf(135696, 120125), DeliveryInf(13864, 69208), DeliveryInf(89818, 105909), DeliveryInf(131450, 91799), DeliveryInf(110603, 126555), DeliveryInf(92846, 56464), DeliveryInf(7107, 88995), DeliveryInf(35474, 46762), DeliveryInf(54341, 2526), DeliveryInf(99914, 98300)};
        depots = {13, 51601, 61505, 85936, 132265};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR medium_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR medium_toronto_canada: INVALID" << std::endl;
        }

    } //medium_toronto_canada

} //medium_toronto_canada_public

