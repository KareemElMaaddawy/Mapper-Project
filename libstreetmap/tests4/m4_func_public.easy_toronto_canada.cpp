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


SUITE(easy_toronto_canada_public) {
    TEST(easy_toronto_canada) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(134204, 32615), DeliveryInf(113281, 117913), DeliveryInf(74966, 45024), DeliveryInf(135221, 51749), DeliveryInf(40756, 38216), DeliveryInf(102380, 48326), DeliveryInf(86334, 101772), DeliveryInf(34975, 81132), DeliveryInf(82672, 87076), DeliveryInf(42199, 87321), DeliveryInf(27101, 82634), DeliveryInf(119658, 56035), DeliveryInf(29435, 28262), DeliveryInf(136515, 72388), DeliveryInf(65038, 96006), DeliveryInf(149474, 140389), DeliveryInf(81186, 125686), DeliveryInf(105969, 37007), DeliveryInf(103341, 133821), DeliveryInf(137127, 122694), DeliveryInf(117691, 82535), DeliveryInf(95563, 121164), DeliveryInf(145491, 10229), DeliveryInf(121985, 42614), DeliveryInf(106896, 109313)};
        depots = {10, 38701};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR easy_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR easy_toronto_canada: INVALID" << std::endl;
        }

    } //easy_toronto_canada

} //easy_toronto_canada_public

