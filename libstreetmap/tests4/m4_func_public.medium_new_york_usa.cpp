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


SUITE(medium_new_york_usa_public) {
    TEST(medium_new_york_usa) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(207913, 79813), DeliveryInf(185807, 164505), DeliveryInf(108625, 57217), DeliveryInf(159758, 17568), DeliveryInf(37976, 66099), DeliveryInf(205279, 104865), DeliveryInf(51095, 169800), DeliveryInf(151015, 129760), DeliveryInf(85392, 103483), DeliveryInf(156340, 170582), DeliveryInf(191809, 201190), DeliveryInf(124748, 170290), DeliveryInf(72026, 119904), DeliveryInf(10046, 75233), DeliveryInf(181451, 4301), DeliveryInf(19597, 186496), DeliveryInf(136294, 82288), DeliveryInf(194042, 141877), DeliveryInf(80315, 201657), DeliveryInf(164575, 110481), DeliveryInf(70536, 41979), DeliveryInf(55011, 42866), DeliveryInf(131241, 80127), DeliveryInf(12775, 65379), DeliveryInf(129984, 184851), DeliveryInf(50143, 148100), DeliveryInf(67886, 176518), DeliveryInf(161988, 140611), DeliveryInf(61471, 163721), DeliveryInf(78847, 72782), DeliveryInf(26048, 3571), DeliveryInf(93915, 18631), DeliveryInf(128238, 149705), DeliveryInf(134540, 129443), DeliveryInf(162244, 36545), DeliveryInf(138949, 34857), DeliveryInf(207847, 149352), DeliveryInf(92630, 113705), DeliveryInf(141230, 76216), DeliveryInf(76813, 67779), DeliveryInf(81428, 178889), DeliveryInf(141289, 125796), DeliveryInf(51802, 97414), DeliveryInf(210231, 127024), DeliveryInf(100101, 107300), DeliveryInf(37765, 125553), DeliveryInf(40404, 70941), DeliveryInf(34126, 206560), DeliveryInf(79266, 97827), DeliveryInf(155554, 31995)};
        depots = {19, 72940, 86939, 121473, 186960, 162714, 12846, 126960, 11070, 208883};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR medium_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR medium_new_york_usa: INVALID" << std::endl;
        }

    } //medium_new_york_usa

} //medium_new_york_usa_public

