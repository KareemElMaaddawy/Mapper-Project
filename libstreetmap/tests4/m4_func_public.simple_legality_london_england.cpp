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


SUITE(simple_legality_london_england_public) {
    TEST(simple_legality_london_england) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        std::vector<CourierSubPath> result_path;
        float turn_penalty;

        deliveries = {DeliveryInf(41690, 49692)};
        depots = {11};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(35048, 83417), DeliveryInf(56388, 80744), DeliveryInf(56388, 119647), DeliveryInf(192871, 159304), DeliveryInf(179371, 123306), DeliveryInf(20275, 85140), DeliveryInf(56388, 97078), DeliveryInf(35048, 51269)};
        depots = {3557, 109109, 101089};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(5742, 184660), DeliveryInf(242736, 110910)};
        depots = {40316};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(43904, 29608)};
        depots = {119392};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(45906, 53679)};
        depots = {21706, 45306};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(59938, 84650), DeliveryInf(55915, 43001), DeliveryInf(73986, 125031)};
        depots = {240637, 55679, 181721};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(63148, 85567), DeliveryInf(216600, 18287), DeliveryInf(2041, 229170)};
        depots = {164173, 223466, 37369};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(142079, 92734)};
        depots = {7301, 193876};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(32704, 163121), DeliveryInf(191107, 93578), DeliveryInf(81093, 224822)};
        depots = {38740, 85366, 45799};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(148213, 76434), DeliveryInf(213076, 83028), DeliveryInf(69274, 83028), DeliveryInf(69274, 147986), DeliveryInf(69274, 200290), DeliveryInf(213076, 164328), DeliveryInf(69274, 83028), DeliveryInf(121615, 164328), DeliveryInf(231137, 164328)};
        depots = {54201, 156426, 96344};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(38802, 117332), DeliveryInf(154016, 215161)};
        depots = {88911, 57215};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(94067, 109633), DeliveryInf(137461, 41168)};
        depots = {133774, 35135};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(111354, 120441), DeliveryInf(157636, 157400), DeliveryInf(71208, 115788), DeliveryInf(116289, 24805), DeliveryInf(103904, 116001)};
        depots = {23568, 193019, 199700};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(128109, 144927), DeliveryInf(41840, 139063), DeliveryInf(35331, 4612), DeliveryInf(41840, 144333), DeliveryInf(72344, 143473), DeliveryInf(72344, 221824), DeliveryInf(41840, 202261), DeliveryInf(74974, 4928)};
        depots = {161030, 15959, 137482};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(133222, 194336), DeliveryInf(97053, 72604), DeliveryInf(133222, 132615), DeliveryInf(97053, 47033), DeliveryInf(19923, 20888), DeliveryInf(61330, 74168), DeliveryInf(43563, 220074), DeliveryInf(97053, 64991)};
        depots = {146567, 45619, 194475};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(141436, 214833), DeliveryInf(42993, 185352), DeliveryInf(41339, 241376), DeliveryInf(158122, 86169), DeliveryInf(82514, 139335)};
        depots = {8922, 211953, 17000};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(200545, 196728), DeliveryInf(58004, 102045), DeliveryInf(119879, 11356), DeliveryInf(23212, 185282), DeliveryInf(86309, 225299)};
        depots = {62744, 220526, 24589};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(144158, 213887), DeliveryInf(203297, 151234)};
        depots = {80757};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(168416, 97268), DeliveryInf(216637, 101420), DeliveryInf(87779, 89361), DeliveryInf(224267, 193368), DeliveryInf(114334, 69848)};
        depots = {122196, 67666, 226217};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(177494, 222405), DeliveryInf(99597, 115261), DeliveryInf(164350, 17060), DeliveryInf(155735, 87650), DeliveryInf(101626, 234290), DeliveryInf(10495, 17060), DeliveryInf(215780, 87650), DeliveryInf(161786, 87650)};
        depots = {25622, 51333, 220185};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(193454, 123332), DeliveryInf(156790, 85515), DeliveryInf(190802, 32743), DeliveryInf(156790, 81052), DeliveryInf(227766, 32743), DeliveryInf(86287, 85515), DeliveryInf(156790, 85515), DeliveryInf(86287, 32743), DeliveryInf(156790, 160905)};
        depots = {115287, 197903, 118997};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(205077, 47194), DeliveryInf(206081, 198008), DeliveryInf(57096, 162547), DeliveryInf(175507, 149070), DeliveryInf(232655, 85996)};
        depots = {75642, 115521, 215827};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(219927, 31202), DeliveryInf(55790, 31202), DeliveryInf(213938, 237209), DeliveryInf(115717, 148028), DeliveryInf(227719, 236605), DeliveryInf(108949, 236605), DeliveryInf(148810, 31202), DeliveryInf(204671, 27558)};
        depots = {201728, 151998, 143890};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(231967, 223048), DeliveryInf(177348, 41212), DeliveryInf(151, 227523), DeliveryInf(231471, 78014), DeliveryInf(118943, 198973)};
        depots = {146640, 105732, 104846};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(234741, 51325), DeliveryInf(234741, 101271), DeliveryInf(170303, 51325), DeliveryInf(170303, 34017), DeliveryInf(215558, 34017), DeliveryInf(234741, 94031), DeliveryInf(234741, 51325), DeliveryInf(241163, 34017), DeliveryInf(3994, 120548)};
        depots = {146482, 97451, 201591};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(241373, 37780), DeliveryInf(80370, 97333)};
        depots = {201993, 200475};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(169115, 241411)};
        depots = {211933, 48808};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(195870, 75014), DeliveryInf(196869, 226286)};
        depots = {228776};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(199240, 58413), DeliveryInf(177476, 58367), DeliveryInf(150662, 152789), DeliveryInf(110720, 58413), DeliveryInf(236967, 124807), DeliveryInf(92103, 203956), DeliveryInf(155199, 58413), DeliveryInf(234941, 152789)};
        depots = {236219, 29832, 33648};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(229435, 93003)};
        depots = {192004};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

    } //simple_legality_london_england

} //simple_legality_london_england_public

