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


SUITE(extreme_multi_new_york_usa_public) {
    TEST(extreme_multi_new_york_usa) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(99133, 133879), DeliveryInf(26828, 140293), DeliveryInf(184746, 31612), DeliveryInf(121297, 94086), DeliveryInf(36889, 28487), DeliveryInf(211986, 22143), DeliveryInf(184746, 168181), DeliveryInf(160888, 191276), DeliveryInf(207298, 1414), DeliveryInf(113155, 27489), DeliveryInf(7494, 167380), DeliveryInf(98614, 159359), DeliveryInf(36889, 112489), DeliveryInf(8566, 15796), DeliveryInf(48836, 54303), DeliveryInf(140986, 129116), DeliveryInf(84950, 147679), DeliveryInf(36889, 30060), DeliveryInf(6167, 162280), DeliveryInf(202042, 11076), DeliveryInf(189136, 127001), DeliveryInf(104866, 77420), DeliveryInf(24153, 108734), DeliveryInf(8566, 198105), DeliveryInf(51530, 98509), DeliveryInf(99988, 82545), DeliveryInf(15478, 172507), DeliveryInf(172972, 112489), DeliveryInf(614, 16965), DeliveryInf(26961, 27489), DeliveryInf(66576, 112777), DeliveryInf(184746, 167380), DeliveryInf(104866, 200527), DeliveryInf(36889, 122673), DeliveryInf(15478, 43819), DeliveryInf(31934, 77619), DeliveryInf(28978, 99942), DeliveryInf(90014, 66530), DeliveryInf(180069, 3930), DeliveryInf(157044, 101410), DeliveryInf(22481, 40813), DeliveryInf(110929, 99544), DeliveryInf(86996, 16525), DeliveryInf(7986, 40813), DeliveryInf(51530, 47062), DeliveryInf(151297, 99942), DeliveryInf(60865, 155887), DeliveryInf(34344, 77461), DeliveryInf(211986, 40813), DeliveryInf(180069, 34429), DeliveryInf(13265, 4576), DeliveryInf(79780, 155702), DeliveryInf(15478, 43819), DeliveryInf(140230, 136977), DeliveryInf(54672, 22143), DeliveryInf(184746, 94288), DeliveryInf(130566, 41552), DeliveryInf(98429, 3875), DeliveryInf(202141, 142309), DeliveryInf(158943, 127001), DeliveryInf(36889, 59430), DeliveryInf(201879, 208125), DeliveryInf(74811, 197244), DeliveryInf(110088, 34429), DeliveryInf(33695, 69192), DeliveryInf(125092, 179080), DeliveryInf(175728, 150319), DeliveryInf(152743, 91456), DeliveryInf(28978, 38280), DeliveryInf(95520, 57401), DeliveryInf(176699, 150319), DeliveryInf(191497, 78955), DeliveryInf(93041, 40813), DeliveryInf(8566, 27489), DeliveryInf(193771, 170294), DeliveryInf(207912, 7602), DeliveryInf(180069, 173243), DeliveryInf(211986, 99942), DeliveryInf(132513, 16525), DeliveryInf(140986, 112489), DeliveryInf(34344, 156339), DeliveryInf(51635, 175932), DeliveryInf(180632, 40527), DeliveryInf(22473, 22143), DeliveryInf(99886, 194823), DeliveryInf(139589, 150319), DeliveryInf(98614, 1873), DeliveryInf(49284, 67136), DeliveryInf(180069, 99942), DeliveryInf(155193, 59722), DeliveryInf(51530, 168784), DeliveryInf(174868, 50895), DeliveryInf(34344, 75139), DeliveryInf(206710, 63301), DeliveryInf(151011, 145483), DeliveryInf(180069, 150319), DeliveryInf(162445, 98509), DeliveryInf(67465, 165561), DeliveryInf(151786, 108734), DeliveryInf(8566, 157165), DeliveryInf(56376, 122396), DeliveryInf(45277, 16525), DeliveryInf(180069, 67136), DeliveryInf(48590, 24789), DeliveryInf(140986, 117463), DeliveryInf(8566, 74260), DeliveryInf(21414, 120770), DeliveryInf(89671, 98509), DeliveryInf(87437, 72946), DeliveryInf(104866, 26084), DeliveryInf(186323, 5288), DeliveryInf(189907, 98509), DeliveryInf(1266, 66530), DeliveryInf(40122, 200761), DeliveryInf(98614, 145483), DeliveryInf(8566, 131189), DeliveryInf(80449, 167380), DeliveryInf(34344, 121718), DeliveryInf(117564, 167380), DeliveryInf(166684, 69192), DeliveryInf(184746, 200527), DeliveryInf(83272, 53395), DeliveryInf(180069, 190752), DeliveryInf(210087, 200527), DeliveryInf(20496, 22143), DeliveryInf(147162, 111868), DeliveryInf(98614, 34429), DeliveryInf(7394, 171299), DeliveryInf(28978, 27489), DeliveryInf(36889, 40813), DeliveryInf(137738, 16525), DeliveryInf(19878, 186857), DeliveryInf(24522, 148383), DeliveryInf(51530, 175508), DeliveryInf(36889, 36467), DeliveryInf(15478, 157270), DeliveryInf(112935, 46548), DeliveryInf(15478, 2933), DeliveryInf(77159, 172507), DeliveryInf(184746, 1925), DeliveryInf(51530, 167217), DeliveryInf(134189, 108734), DeliveryInf(58341, 28102), DeliveryInf(36889, 66530), DeliveryInf(47397, 67136), DeliveryInf(104866, 151817), DeliveryInf(59036, 64614), DeliveryInf(150549, 80339), DeliveryInf(61142, 22143), DeliveryInf(98614, 145483), DeliveryInf(184746, 108952), DeliveryInf(15478, 69192), DeliveryInf(128485, 98509), DeliveryInf(15478, 175842), DeliveryInf(97524, 8317), DeliveryInf(177146, 172957), DeliveryInf(98614, 68056), DeliveryInf(28978, 112489), DeliveryInf(211986, 69192), DeliveryInf(179397, 99942), DeliveryInf(180069, 127001), DeliveryInf(181706, 8617), DeliveryInf(8566, 21388), DeliveryInf(3894, 107826), DeliveryInf(160591, 172507), DeliveryInf(27200, 85360), DeliveryInf(201330, 43819), DeliveryInf(36889, 201771), DeliveryInf(22473, 21328), DeliveryInf(98614, 5615), DeliveryInf(53170, 77012), DeliveryInf(22473, 192692), DeliveryInf(64132, 125372), DeliveryInf(31935, 66579), DeliveryInf(125000, 44901), DeliveryInf(28978, 166521), DeliveryInf(162276, 48623), DeliveryInf(156489, 108734), DeliveryInf(51865, 27489), DeliveryInf(140986, 147205), DeliveryInf(28978, 26470), DeliveryInf(72637, 172507), DeliveryInf(180069, 79215), DeliveryInf(150280, 61232), DeliveryInf(117695, 124172), DeliveryInf(128395, 150964), DeliveryInf(204721, 205735), DeliveryInf(198114, 188260), DeliveryInf(180069, 127001), DeliveryInf(161727, 112489), DeliveryInf(131673, 40377), DeliveryInf(164578, 16525)};
        depots = {7369, 179636};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR extreme_multi_new_york_usa: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_multi_new_york_usa: INVALID" << std::endl;
        }

    } //extreme_multi_new_york_usa

} //extreme_multi_new_york_usa_public

