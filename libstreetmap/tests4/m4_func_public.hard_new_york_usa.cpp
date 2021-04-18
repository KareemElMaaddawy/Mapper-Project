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


SUITE(hard_new_york_usa_public) {
    TEST(hard_new_york_usa) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(175764, 10088), DeliveryInf(149375, 69358), DeliveryInf(82169, 4899), DeliveryInf(173202, 207455), DeliveryInf(90033, 34908), DeliveryInf(203393, 182428), DeliveryInf(203494, 6627), DeliveryInf(209547, 183872), DeliveryInf(35065, 111698), DeliveryInf(17903, 177347), DeliveryInf(44795, 7779), DeliveryInf(39994, 201265), DeliveryInf(213964, 70228), DeliveryInf(195394, 75742), DeliveryInf(138101, 169999), DeliveryInf(96016, 196899), DeliveryInf(72284, 206543), DeliveryInf(135329, 162438), DeliveryInf(100393, 198725), DeliveryInf(204651, 213310), DeliveryInf(106685, 50527), DeliveryInf(146085, 44340), DeliveryInf(62176, 26716), DeliveryInf(76838, 77901), DeliveryInf(171976, 159616), DeliveryInf(154695, 120660), DeliveryInf(205718, 160806), DeliveryInf(79466, 47212), DeliveryInf(12850, 208423), DeliveryInf(119782, 23858), DeliveryInf(122376, 76901), DeliveryInf(41649, 177826), DeliveryInf(59812, 102499), DeliveryInf(114563, 158213), DeliveryInf(15702, 118003), DeliveryInf(178647, 127649), DeliveryInf(123192, 164155), DeliveryInf(185333, 75572), DeliveryInf(95581, 117963), DeliveryInf(99055, 162200), DeliveryInf(144589, 207349), DeliveryInf(108869, 99285), DeliveryInf(176611, 95270), DeliveryInf(48173, 105081), DeliveryInf(10169, 90652), DeliveryInf(189848, 30492), DeliveryInf(116291, 134125), DeliveryInf(25313, 161804), DeliveryInf(153879, 45682), DeliveryInf(187131, 1554), DeliveryInf(81724, 24167), DeliveryInf(206306, 212250), DeliveryInf(194443, 58988), DeliveryInf(105638, 121768), DeliveryInf(34589, 171471), DeliveryInf(67450, 142126), DeliveryInf(191948, 163445), DeliveryInf(56827, 206991), DeliveryInf(116755, 15060), DeliveryInf(27932, 103633), DeliveryInf(180088, 119494), DeliveryInf(100818, 150795), DeliveryInf(71521, 210554), DeliveryInf(206197, 175290), DeliveryInf(192082, 193442), DeliveryInf(123281, 163965), DeliveryInf(10257, 186196), DeliveryInf(203492, 195768), DeliveryInf(104117, 51556), DeliveryInf(205783, 184491), DeliveryInf(212312, 144847), DeliveryInf(100159, 172943), DeliveryInf(193396, 102860), DeliveryInf(209177, 204588), DeliveryInf(111688, 123317)};
        depots = {24, 91175, 1450, 98229};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR hard_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_new_york_usa: INVALID" << std::endl;
        }

    } //hard_new_york_usa

} //hard_new_york_usa_public

