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


SUITE(medium_london_england_public) {
    TEST(medium_london_england) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(150028, 142606), DeliveryInf(105890, 178721), DeliveryInf(182627, 107359), DeliveryInf(172633, 43413), DeliveryInf(146595, 219266), DeliveryInf(148592, 29777), DeliveryInf(43171, 39012), DeliveryInf(46187, 185469), DeliveryInf(153799, 70270), DeliveryInf(57321, 93084), DeliveryInf(161515, 91812), DeliveryInf(14603, 22402), DeliveryInf(161448, 90613), DeliveryInf(207426, 97616)};
        depots = {22, 83381, 99384, 138862, 213724, 186007, 14685, 145134, 12655, 238784, 87809, 59217, 80633, 58410, 90134, 11485, 124175, 240326, 221820, 212406};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR medium_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR medium_london_england: INVALID" << std::endl;
        }

    } //medium_london_england

} //medium_london_england_public

