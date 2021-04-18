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


SUITE(easy_new_york_usa_public) {
    TEST(easy_new_york_usa) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(205654, 73148), DeliveryInf(105967, 158139), DeliveryInf(166359, 198443), DeliveryInf(38307, 52311), DeliveryInf(41608, 182300), DeliveryInf(123085, 46103), DeliveryInf(57610, 68310), DeliveryInf(192967, 116805), DeliveryInf(189700, 135706), DeliveryInf(135081, 189160), DeliveryInf(211285, 54019), DeliveryInf(114759, 173430), DeliveryInf(154517, 39949), DeliveryInf(149790, 63643), DeliveryInf(191139, 177660), DeliveryInf(91933, 14458), DeliveryInf(169139, 79207), DeliveryInf(146075, 166673), DeliveryInf(193832, 102323), DeliveryInf(122035, 114683), DeliveryInf(59649, 123431), DeliveryInf(49438, 171268), DeliveryInf(151100, 116666), DeliveryInf(116858, 143856), DeliveryInf(160126, 60236)};
        depots = {14, 54705, 172428, 144716};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR easy_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR easy_new_york_usa: INVALID" << std::endl;
        }

    } //easy_new_york_usa

} //easy_new_york_usa_public

