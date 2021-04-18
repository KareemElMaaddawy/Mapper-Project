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


SUITE(hard_multi_toronto_canada_public) {
    TEST(hard_multi_toronto_canada) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(150557, 95305), DeliveryInf(143341, 95186), DeliveryInf(96042, 72091), DeliveryInf(74015, 144050), DeliveryInf(137363, 92346), DeliveryInf(39415, 97191), DeliveryInf(149213, 147255), DeliveryInf(93676, 63128), DeliveryInf(144777, 26110), DeliveryInf(69950, 57929), DeliveryInf(20957, 139197), DeliveryInf(149933, 77216), DeliveryInf(84115, 105370), DeliveryInf(6777, 46480), DeliveryInf(56524, 37642), DeliveryInf(58871, 103580), DeliveryInf(103812, 93771), DeliveryInf(40300, 45488), DeliveryInf(1378, 149908), DeliveryInf(76433, 42160), DeliveryInf(72040, 51164), DeliveryInf(111281, 115432), DeliveryInf(69950, 130487), DeliveryInf(90616, 70423), DeliveryInf(54971, 137643), DeliveryInf(69950, 135854), DeliveryInf(10661, 108413), DeliveryInf(99660, 80879), DeliveryInf(147237, 31134), DeliveryInf(20957, 6358), DeliveryInf(134233, 78674), DeliveryInf(103374, 138709), DeliveryInf(130077, 120304), DeliveryInf(115931, 8564), DeliveryInf(139270, 114659), DeliveryInf(121319, 134314), DeliveryInf(28476, 50562), DeliveryInf(57636, 54658), DeliveryInf(118731, 36989), DeliveryInf(33958, 109490), DeliveryInf(81512, 137425), DeliveryInf(62081, 106472), DeliveryInf(117240, 86134), DeliveryInf(71925, 3798), DeliveryInf(20957, 35968), DeliveryInf(111462, 135137), DeliveryInf(84696, 120662), DeliveryInf(60718, 38781), DeliveryInf(33953, 120662), DeliveryInf(66915, 36966), DeliveryInf(90049, 66029), DeliveryInf(103498, 137699), DeliveryInf(96652, 28523), DeliveryInf(51832, 33199), DeliveryInf(22025, 103594), DeliveryInf(65231, 119356), DeliveryInf(129067, 3159), DeliveryInf(13359, 137699), DeliveryInf(22441, 104689), DeliveryInf(22931, 117358), DeliveryInf(85228, 137699), DeliveryInf(85029, 123116), DeliveryInf(54202, 45029), DeliveryInf(57379, 116254), DeliveryInf(70143, 71101), DeliveryInf(106225, 31674), DeliveryInf(69950, 1706), DeliveryInf(74851, 47570), DeliveryInf(51384, 29630), DeliveryInf(58871, 12552), DeliveryInf(10553, 19486), DeliveryInf(112070, 62283), DeliveryInf(5448, 50859), DeliveryInf(28996, 65596), DeliveryInf(79836, 23603), DeliveryInf(7152, 136977), DeliveryInf(113977, 103536), DeliveryInf(20957, 120662), DeliveryInf(83671, 120662), DeliveryInf(94499, 29595), DeliveryInf(95627, 87322), DeliveryInf(66126, 9150), DeliveryInf(20458, 105162), DeliveryInf(84398, 148659), DeliveryInf(68776, 124568), DeliveryInf(3789, 120662), DeliveryInf(80420, 62280), DeliveryInf(144707, 54542), DeliveryInf(71594, 57784), DeliveryInf(98713, 137699), DeliveryInf(116696, 66574), DeliveryInf(60227, 104890), DeliveryInf(58871, 7531), DeliveryInf(120403, 11019), DeliveryInf(20957, 131557), DeliveryInf(69379, 6604), DeliveryInf(122050, 65070), DeliveryInf(13556, 76565), DeliveryInf(151137, 137699), DeliveryInf(53048, 77679)};
        depots = {20, 77402, 92258};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR hard_multi_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_multi_toronto_canada: INVALID" << std::endl;
        }

    } //hard_multi_toronto_canada

} //hard_multi_toronto_canada_public

