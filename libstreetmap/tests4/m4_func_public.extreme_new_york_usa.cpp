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


SUITE(extreme_new_york_usa_public) {
    TEST(extreme_new_york_usa) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(161620, 42663), DeliveryInf(118741, 9036), DeliveryInf(139991, 203927), DeliveryInf(49748, 119124), DeliveryInf(73558, 63591), DeliveryInf(84023, 191144), DeliveryInf(156704, 53913), DeliveryInf(121218, 157168), DeliveryInf(67954, 183204), DeliveryInf(73756, 45065), DeliveryInf(200644, 131421), DeliveryInf(58259, 139358), DeliveryInf(5536, 159458), DeliveryInf(168363, 104101), DeliveryInf(55855, 7050), DeliveryInf(57127, 138025), DeliveryInf(137683, 172094), DeliveryInf(16213, 91337), DeliveryInf(1267, 190148), DeliveryInf(127076, 30010), DeliveryInf(113334, 74229), DeliveryInf(12633, 113487), DeliveryInf(12477, 64333), DeliveryInf(187775, 197170), DeliveryInf(156220, 202839), DeliveryInf(197877, 103022), DeliveryInf(82702, 31303), DeliveryInf(176703, 90491), DeliveryInf(11389, 189406), DeliveryInf(196111, 161090), DeliveryInf(164697, 153504), DeliveryInf(144004, 187114), DeliveryInf(170475, 139846), DeliveryInf(42658, 155287), DeliveryInf(63954, 74258), DeliveryInf(62077, 124546), DeliveryInf(8450, 88334), DeliveryInf(133378, 1665), DeliveryInf(57772, 63749), DeliveryInf(33837, 49740), DeliveryInf(127645, 14830), DeliveryInf(59308, 154637), DeliveryInf(15419, 117602), DeliveryInf(107574, 84376), DeliveryInf(172644, 157904), DeliveryInf(91761, 54779), DeliveryInf(77678, 12229), DeliveryInf(134423, 70561), DeliveryInf(173683, 118878), DeliveryInf(202897, 207474), DeliveryInf(66637, 47921), DeliveryInf(63580, 22531), DeliveryInf(193342, 951), DeliveryInf(166345, 199407), DeliveryInf(72063, 157260), DeliveryInf(11578, 71480), DeliveryInf(207069, 132765), DeliveryInf(193542, 171358), DeliveryInf(181889, 163159), DeliveryInf(168568, 77112), DeliveryInf(37567, 19591), DeliveryInf(95058, 104150), DeliveryInf(47536, 69802), DeliveryInf(74609, 81094), DeliveryInf(140223, 99260), DeliveryInf(17902, 181521), DeliveryInf(210664, 146124), DeliveryInf(164327, 163406), DeliveryInf(46227, 76651), DeliveryInf(31228, 58725), DeliveryInf(44920, 32389), DeliveryInf(17094, 73449), DeliveryInf(168224, 80834), DeliveryInf(124806, 212099), DeliveryInf(63973, 156087), DeliveryInf(180210, 47377), DeliveryInf(191581, 16635), DeliveryInf(120288, 157144), DeliveryInf(189549, 114541), DeliveryInf(119614, 176938), DeliveryInf(166072, 122314), DeliveryInf(115069, 66448), DeliveryInf(12078, 36480), DeliveryInf(122447, 146984), DeliveryInf(182240, 24892), DeliveryInf(87907, 143044), DeliveryInf(190003, 181210), DeliveryInf(40847, 90523), DeliveryInf(31491, 60662), DeliveryInf(10891, 140935), DeliveryInf(24434, 51344), DeliveryInf(129580, 191127), DeliveryInf(177760, 20878), DeliveryInf(20259, 15102), DeliveryInf(152220, 169434), DeliveryInf(43873, 95532), DeliveryInf(58309, 33785), DeliveryInf(42213, 205407), DeliveryInf(140551, 63614), DeliveryInf(202516, 174567), DeliveryInf(51741, 120234), DeliveryInf(114413, 145529), DeliveryInf(70302, 14123), DeliveryInf(190784, 31224), DeliveryInf(29258, 173718), DeliveryInf(100130, 11674), DeliveryInf(151877, 26719), DeliveryInf(126046, 185672), DeliveryInf(166106, 131549), DeliveryInf(95788, 95030), DeliveryInf(55430, 7260), DeliveryInf(151951, 17313), DeliveryInf(30758, 51151), DeliveryInf(32808, 188296), DeliveryInf(34139, 50040), DeliveryInf(99196, 144820), DeliveryInf(129704, 19572), DeliveryInf(113473, 83145), DeliveryInf(143029, 67546), DeliveryInf(31621, 54011), DeliveryInf(19823, 199749), DeliveryInf(32449, 80530), DeliveryInf(83, 142759), DeliveryInf(40951, 60451), DeliveryInf(193440, 181119), DeliveryInf(154142, 81513), DeliveryInf(55992, 178844), DeliveryInf(76291, 75366)};
        depots = {33};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR extreme_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_new_york_usa: INVALID" << std::endl;
        }

    } //extreme_new_york_usa

} //extreme_new_york_usa_public

