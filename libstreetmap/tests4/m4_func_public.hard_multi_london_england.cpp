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


SUITE(hard_multi_london_england_public) {
    TEST(hard_multi_london_england) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(191855, 65119), DeliveryInf(198013, 208556), DeliveryInf(113030, 105406), DeliveryInf(87583, 37054), DeliveryInf(136376, 151368), DeliveryInf(210188, 181091), DeliveryInf(135201, 145508), DeliveryInf(198013, 171647), DeliveryInf(22027, 83753), DeliveryInf(33864, 108126), DeliveryInf(141556, 151368), DeliveryInf(188566, 91336), DeliveryInf(196035, 119599), DeliveryInf(149077, 151368), DeliveryInf(17227, 146425), DeliveryInf(198013, 151368), DeliveryInf(97319, 161038), DeliveryInf(120949, 135920), DeliveryInf(95129, 233941), DeliveryInf(237916, 17053), DeliveryInf(242272, 21587), DeliveryInf(225042, 123505), DeliveryInf(63689, 151368), DeliveryInf(131713, 137718), DeliveryInf(36263, 167239), DeliveryInf(100315, 33057), DeliveryInf(198013, 21905), DeliveryInf(88826, 156177)};
        depots = {33, 125072};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR hard_multi_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_multi_london_england: INVALID" << std::endl;
        }

    } //hard_multi_london_england

} //hard_multi_london_england_public

