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


SUITE(easy_london_england_public) {
    TEST(easy_london_england) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(165433, 56515), DeliveryInf(197111, 65856), DeliveryInf(139505, 190173), DeliveryInf(62536, 166986), DeliveryInf(221579, 193351), DeliveryInf(133587, 105094), DeliveryInf(47564, 183048)};
        depots = {16};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR easy_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR easy_london_england: INVALID" << std::endl;
        }

    } //easy_london_england

} //easy_london_england_public

