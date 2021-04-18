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


SUITE(hard_london_england_public) {
    TEST(hard_london_england) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(11726, 17950), DeliveryInf(1657, 14689), DeliveryInf(205868, 47611), DeliveryInf(121957, 235167), DeliveryInf(115250, 232625), DeliveryInf(104227, 39541), DeliveryInf(109761, 239544), DeliveryInf(77106, 102921), DeliveryInf(201894, 235839), DeliveryInf(93932, 139894), DeliveryInf(239121, 28937), DeliveryInf(71076, 113235), DeliveryInf(232508, 11625), DeliveryInf(136929, 170758), DeliveryInf(154701, 211863), DeliveryInf(204220, 221081), DeliveryInf(140929, 87838), DeliveryInf(175907, 132938), DeliveryInf(223364, 55069), DeliveryInf(31931, 114765), DeliveryInf(64962, 109263), DeliveryInf(119021, 140827), DeliveryInf(196595, 166998), DeliveryInf(197996, 244594), DeliveryInf(130962, 222278), DeliveryInf(124453, 133469), DeliveryInf(112290, 176839), DeliveryInf(235241, 20466)};
        depots = {27};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR hard_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_london_england: INVALID" << std::endl;
        }

    } //hard_london_england

} //hard_london_england_public

