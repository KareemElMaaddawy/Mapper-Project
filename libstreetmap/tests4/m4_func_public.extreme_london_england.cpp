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


SUITE(extreme_london_england_public) {
    TEST(extreme_london_england) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(190157, 17627), DeliveryInf(135738, 13019), DeliveryInf(236711, 144089), DeliveryInf(1449, 36148), DeliveryInf(224184, 88798), DeliveryInf(226203, 138570), DeliveryInf(48255, 66656), DeliveryInf(192699, 64007), DeliveryInf(198546, 114464), DeliveryInf(142672, 77681), DeliveryInf(240821, 23159), DeliveryInf(36000, 203206), DeliveryInf(160671, 9660), DeliveryInf(206007, 221019), DeliveryInf(72681, 129717), DeliveryInf(14263, 54340), DeliveryInf(20465, 176208), DeliveryInf(139976, 84315), DeliveryInf(153666, 66043), DeliveryInf(39026, 231942), DeliveryInf(145917, 109500), DeliveryInf(188274, 82379), DeliveryInf(51350, 113396), DeliveryInf(221247, 130792), DeliveryInf(85289, 48764), DeliveryInf(59148, 178583), DeliveryInf(18535, 73131), DeliveryInf(80365, 38681), DeliveryInf(201998, 84088), DeliveryInf(129557, 22661), DeliveryInf(35161, 100491), DeliveryInf(50153, 218094), DeliveryInf(187850, 174011), DeliveryInf(65305, 160296), DeliveryInf(148129, 194879), DeliveryInf(19542, 173703), DeliveryInf(148272, 216683), DeliveryInf(37505, 122973), DeliveryInf(76176, 46694), DeliveryInf(179136, 13235), DeliveryInf(6328, 229366), DeliveryInf(33447, 87212), DeliveryInf(163504, 42945)};
        depots = {38};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR extreme_london_england: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_london_england: INVALID" << std::endl;
        }

    } //extreme_london_england

} //extreme_london_england_public

