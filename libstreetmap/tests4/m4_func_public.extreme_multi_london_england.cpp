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


SUITE(extreme_multi_london_england_public) {
    TEST(extreme_multi_london_england) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(211781, 59554), DeliveryInf(78024, 149477), DeliveryInf(126867, 158428), DeliveryInf(126867, 7447), DeliveryInf(232423, 125772), DeliveryInf(126867, 125772), DeliveryInf(118435, 59554), DeliveryInf(86342, 130962), DeliveryInf(198493, 155210), DeliveryInf(32577, 129169), DeliveryInf(120109, 97124), DeliveryInf(116820, 155210), DeliveryInf(179665, 183196), DeliveryInf(100119, 181241), DeliveryInf(126867, 155210), DeliveryInf(44805, 72533), DeliveryInf(186169, 59554), DeliveryInf(195233, 73151), DeliveryInf(126867, 172006), DeliveryInf(126867, 125772), DeliveryInf(125792, 143068), DeliveryInf(32577, 130816), DeliveryInf(118435, 93454), DeliveryInf(48044, 72533), DeliveryInf(126867, 42462), DeliveryInf(230206, 42596), DeliveryInf(29207, 163848), DeliveryInf(32577, 144188), DeliveryInf(45123, 227112), DeliveryInf(114643, 8164), DeliveryInf(86826, 40167), DeliveryInf(180268, 72533), DeliveryInf(183624, 28991), DeliveryInf(32577, 230056), DeliveryInf(118435, 125772), DeliveryInf(214718, 96318), DeliveryInf(118435, 95978), DeliveryInf(126867, 50800), DeliveryInf(112295, 14819), DeliveryInf(3204, 162194), DeliveryInf(45123, 110498), DeliveryInf(169706, 222719), DeliveryInf(118435, 230056), DeliveryInf(32577, 155210), DeliveryInf(32577, 72533), DeliveryInf(52038, 230056), DeliveryInf(32577, 236594), DeliveryInf(181226, 110498), DeliveryInf(166763, 110498), DeliveryInf(40066, 110498), DeliveryInf(32577, 131121), DeliveryInf(161266, 154963), DeliveryInf(62452, 182478), DeliveryInf(140541, 59554), DeliveryInf(45123, 155210), DeliveryInf(22970, 223662), DeliveryInf(77749, 45270), DeliveryInf(118435, 79228), DeliveryInf(77883, 41906), DeliveryInf(235506, 166401), DeliveryInf(118435, 9834), DeliveryInf(232423, 125772), DeliveryInf(54910, 59554), DeliveryInf(118435, 239755), DeliveryInf(92375, 151123), DeliveryInf(32577, 76333)};
        depots = {44};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR extreme_multi_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_multi_london_england: INVALID" << std::endl;
        }

    } //extreme_multi_london_england

} //extreme_multi_london_england_public

