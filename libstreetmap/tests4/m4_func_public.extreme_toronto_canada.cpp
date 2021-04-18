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


SUITE(extreme_toronto_canada_public) {
    TEST(extreme_toronto_canada) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(136850, 88138), DeliveryInf(39515, 41847), DeliveryInf(119109, 129608), DeliveryInf(22090, 86944), DeliveryInf(8544, 111172), DeliveryInf(38141, 13410), DeliveryInf(21231, 53318), DeliveryInf(48074, 90057), DeliveryInf(59, 11768), DeliveryInf(43916, 143499), DeliveryInf(85756, 25980), DeliveryInf(70222, 135906), DeliveryInf(22956, 115427), DeliveryInf(33629, 26367), DeliveryInf(112809, 20083), DeliveryInf(143270, 49610), DeliveryInf(121227, 35188), DeliveryInf(86531, 26189), DeliveryInf(85098, 30182), DeliveryInf(73646, 2930), DeliveryInf(80277, 17269), DeliveryInf(81032, 129117), DeliveryInf(672, 88205), DeliveryInf(116516, 17610), DeliveryInf(107446, 133996), DeliveryInf(141071, 148864), DeliveryInf(8827, 38419), DeliveryInf(117488, 97901), DeliveryInf(107498, 98053), DeliveryInf(12093, 48657), DeliveryInf(47008, 48799), DeliveryInf(54227, 19651), DeliveryInf(64041, 34854), DeliveryInf(54953, 151292), DeliveryInf(64616, 22652), DeliveryInf(89171, 119867), DeliveryInf(45258, 127557), DeliveryInf(8937, 23692), DeliveryInf(67765, 146778), DeliveryInf(111189, 67096), DeliveryInf(122873, 111709), DeliveryInf(3916, 139461), DeliveryInf(131354, 40918), DeliveryInf(80178, 38803), DeliveryInf(5978, 1450), DeliveryInf(40871, 22528), DeliveryInf(52534, 23231), DeliveryInf(83198, 49382), DeliveryInf(139488, 115414), DeliveryInf(97646, 69320), DeliveryInf(58508, 33309), DeliveryInf(107688, 38019), DeliveryInf(91672, 76728), DeliveryInf(32703, 45512), DeliveryInf(52179, 110413), DeliveryInf(896, 8651), DeliveryInf(57666, 100172), DeliveryInf(33902, 49782), DeliveryInf(143540, 134830), DeliveryInf(85060, 110788), DeliveryInf(135213, 86053), DeliveryInf(102955, 77918), DeliveryInf(45004, 13846), DeliveryInf(44988, 87768), DeliveryInf(109048, 144065), DeliveryInf(25808, 87986), DeliveryInf(84003, 141313), DeliveryInf(98589, 93279), DeliveryInf(30178, 6392), DeliveryInf(110518, 109858), DeliveryInf(41215, 108723), DeliveryInf(8259, 62492), DeliveryInf(26577, 67689), DeliveryInf(99433, 112680), DeliveryInf(39611, 2527), DeliveryInf(10908, 19325), DeliveryInf(47142, 135225), DeliveryInf(62190, 110338), DeliveryInf(97404, 56971), DeliveryInf(24152, 329), DeliveryInf(14332, 92974), DeliveryInf(120603, 77149), DeliveryInf(22370, 113335), DeliveryInf(64018, 32092), DeliveryInf(49919, 93123), DeliveryInf(59442, 101748), DeliveryInf(133211, 137225), DeliveryInf(18902, 72747), DeliveryInf(45245, 141709), DeliveryInf(84101, 112766), DeliveryInf(94358, 38210), DeliveryInf(28971, 106487), DeliveryInf(76103, 54553), DeliveryInf(128678, 57370), DeliveryInf(110424, 103984), DeliveryInf(136921, 123671), DeliveryInf(40414, 93925), DeliveryInf(70837, 111091), DeliveryInf(9991, 3959), DeliveryInf(80942, 15016), DeliveryInf(4988, 19973), DeliveryInf(52039, 75387), DeliveryInf(41251, 111254), DeliveryInf(67249, 67229), DeliveryInf(119010, 140778), DeliveryInf(29863, 41545), DeliveryInf(98934, 126905), DeliveryInf(136780, 89961), DeliveryInf(86626, 13628), DeliveryInf(99037, 128133), DeliveryInf(14024, 122897), DeliveryInf(12248, 38754), DeliveryInf(7705, 103846), DeliveryInf(134970, 109399), DeliveryInf(8057, 41908), DeliveryInf(99201, 39071), DeliveryInf(117512, 53729), DeliveryInf(53972, 22913), DeliveryInf(23901, 27014), DeliveryInf(23938, 36187), DeliveryInf(101197, 113365), DeliveryInf(22279, 51961), DeliveryInf(50981, 99705), DeliveryInf(149035, 47786), DeliveryInf(35195, 115602), DeliveryInf(84621, 50569), DeliveryInf(134521, 115110), DeliveryInf(1178, 141862), DeliveryInf(102453, 42766), DeliveryInf(128197, 5136), DeliveryInf(141946, 150050), DeliveryInf(134097, 108596), DeliveryInf(22092, 50279), DeliveryInf(17286, 67966), DeliveryInf(14770, 45099), DeliveryInf(70177, 130961), DeliveryInf(41957, 125175), DeliveryInf(134418, 105922), DeliveryInf(52782, 7166), DeliveryInf(144269, 100995), DeliveryInf(125009, 122091), DeliveryInf(132842, 13859), DeliveryInf(81406, 80716), DeliveryInf(23210, 18087), DeliveryInf(72883, 70710), DeliveryInf(132374, 52514), DeliveryInf(89900, 50187), DeliveryInf(138739, 47309), DeliveryInf(122137, 22146), DeliveryInf(44980, 80656), DeliveryInf(126524, 123498), DeliveryInf(110860, 33517), DeliveryInf(114338, 15940), DeliveryInf(125756, 145316), DeliveryInf(36323, 73346), DeliveryInf(84274, 88110), DeliveryInf(39214, 74235), DeliveryInf(10684, 70412), DeliveryInf(113964, 113540), DeliveryInf(67584, 122694), DeliveryInf(93065, 42916), DeliveryInf(103376, 112967), DeliveryInf(101876, 57186), DeliveryInf(59692, 72362), DeliveryInf(135534, 19465), DeliveryInf(28897, 53275), DeliveryInf(8191, 66478), DeliveryInf(128926, 100613), DeliveryInf(80287, 73681), DeliveryInf(64916, 13685), DeliveryInf(58821, 57410), DeliveryInf(31881, 37949), DeliveryInf(35401, 98954), DeliveryInf(10491, 121748), DeliveryInf(128417, 24597)};
        depots = {23, 90303, 31778, 36604, 117681, 49735, 91760, 119254, 127490, 31038, 95098, 139989, 11470, 101186, 21760, 88294, 20699, 146491, 12665, 116253};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR extreme_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_toronto_canada: INVALID" << std::endl;
        }

    } //extreme_toronto_canada

} //extreme_toronto_canada_public

