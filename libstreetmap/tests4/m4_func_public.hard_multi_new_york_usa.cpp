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


SUITE(hard_multi_new_york_usa_public) {
    TEST(hard_multi_new_york_usa) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(99149, 161110), DeliveryInf(83216, 9335), DeliveryInf(204646, 162073), DeliveryInf(135758, 196069), DeliveryInf(98876, 211899), DeliveryInf(47993, 71891), DeliveryInf(183867, 190537), DeliveryInf(97216, 146351), DeliveryInf(98876, 41833), DeliveryInf(132413, 64299), DeliveryInf(204547, 40318), DeliveryInf(77703, 31133), DeliveryInf(213636, 170193), DeliveryInf(32414, 194641), DeliveryInf(73265, 208149), DeliveryInf(101668, 113676), DeliveryInf(139533, 52285), DeliveryInf(31722, 161110), DeliveryInf(120472, 146122), DeliveryInf(146296, 77097), DeliveryInf(208123, 91977), DeliveryInf(28917, 109147), DeliveryInf(9580, 203618), DeliveryInf(77703, 112850), DeliveryInf(196861, 189856), DeliveryInf(140872, 196758), DeliveryInf(98876, 81106), DeliveryInf(171487, 161110), DeliveryInf(76615, 40252), DeliveryInf(120191, 146122), DeliveryInf(108039, 12105), DeliveryInf(158414, 33363), DeliveryInf(118899, 88038), DeliveryInf(77703, 81470), DeliveryInf(150152, 123432), DeliveryInf(85132, 147980), DeliveryInf(127287, 121752), DeliveryInf(101830, 133576), DeliveryInf(210917, 146741), DeliveryInf(157553, 148650), DeliveryInf(85826, 146122), DeliveryInf(77703, 170053), DeliveryInf(163872, 81679), DeliveryInf(211934, 4465), DeliveryInf(94586, 53209), DeliveryInf(98069, 194165), DeliveryInf(92206, 50842), DeliveryInf(55714, 114324), DeliveryInf(1949, 165888), DeliveryInf(164953, 8988), DeliveryInf(101199, 157298), DeliveryInf(14918, 12934), DeliveryInf(19162, 135170), DeliveryInf(136620, 109801), DeliveryInf(79898, 48000), DeliveryInf(72632, 41883), DeliveryInf(93471, 160950), DeliveryInf(165722, 99545), DeliveryInf(5356, 130533), DeliveryInf(119298, 170558), DeliveryInf(105804, 161110), DeliveryInf(182439, 54818), DeliveryInf(189741, 7702), DeliveryInf(10109, 101902), DeliveryInf(56965, 150500), DeliveryInf(15070, 153244), DeliveryInf(172521, 146122), DeliveryInf(40987, 92721), DeliveryInf(167830, 146122), DeliveryInf(87753, 111207), DeliveryInf(98876, 161110), DeliveryInf(104622, 119720), DeliveryInf(118270, 134548), DeliveryInf(128088, 194562), DeliveryInf(18884, 77260)};
        depots = {28, 109410, 130409, 74985, 173217, 29623, 19269};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR hard_multi_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_multi_new_york_usa: INVALID" << std::endl;
        }

    } //hard_multi_new_york_usa

} //hard_multi_new_york_usa_public

