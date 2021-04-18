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


SUITE(simple_legality_new_york_usa_public) {
    TEST(simple_legality_new_york_usa) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        std::vector<CourierSubPath> result_path;
        float turn_penalty;

        deliveries = {DeliveryInf(36470, 43469)};
        depots = {9};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(36013, 77777), DeliveryInf(50050, 33943)};
        depots = {95904, 120248};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(81122, 18988)};
        depots = {169598, 124287};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(21433, 100828), DeliveryInf(207819, 199649), DeliveryInf(56852, 38108), DeliveryInf(3111, 95446), DeliveryInf(53650, 17428), DeliveryInf(53650, 192515), DeliveryInf(53650, 18272), DeliveryInf(56852, 64880)};
        depots = {170000, 41144, 63512};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(132295, 6387)};
        depots = {187103, 177839};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(170704, 112759)};
        depots = {6423};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(20305, 162080), DeliveryInf(75501, 197086), DeliveryInf(7805, 185411), DeliveryInf(14871, 123725), DeliveryInf(187931, 37609)};
        depots = {89267, 104867, 9934};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(30907, 126779), DeliveryInf(126258, 65586), DeliveryInf(30907, 4311), DeliveryInf(43511, 25699), DeliveryInf(176466, 132964), DeliveryInf(30907, 112066), DeliveryInf(148880, 1789), DeliveryInf(126258, 4035)};
        depots = {194046, 176933, 121649};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(35268, 132771), DeliveryInf(39423, 5023)};
        depots = {104441};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(7297, 141782), DeliveryInf(71423, 15651), DeliveryInf(51058, 174290), DeliveryInf(178415, 155251), DeliveryInf(207293, 174290), DeliveryInf(109178, 155251), DeliveryInf(80570, 155251), DeliveryInf(100850, 173120)};
        depots = {131795, 96855, 135764};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(46957, 185394)};
        depots = {39633, 40157};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(128213, 39906), DeliveryInf(170122, 116539), DeliveryInf(84900, 116008)};
        depots = {37616, 64721, 109374};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(202485, 68244), DeliveryInf(104049, 174057), DeliveryInf(66169, 101055), DeliveryInf(188800, 179396), DeliveryInf(41284, 180274)};
        depots = {36051, 132, 199032};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(211180, 117022), DeliveryInf(30735, 82287)};
        depots = {42696, 147938};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(59192, 188565), DeliveryInf(3494, 210964), DeliveryInf(58182, 128606), DeliveryInf(105453, 210964), DeliveryInf(105453, 167103), DeliveryInf(105453, 210964), DeliveryInf(105453, 7304), DeliveryInf(106894, 188565), DeliveryInf(3494, 188565)};
        depots = {44898, 82256, 88589};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(70644, 153539), DeliveryInf(174494, 126106)};
        depots = {65620};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(85248, 106386), DeliveryInf(128139, 106386), DeliveryInf(66863, 202193), DeliveryInf(66863, 202193), DeliveryInf(129653, 202193), DeliveryInf(110904, 211865), DeliveryInf(66863, 109680), DeliveryInf(66863, 128979), DeliveryInf(129653, 106386)};
        depots = {72631, 175209, 129455};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(87125, 136233), DeliveryInf(14669, 21216), DeliveryInf(204951, 136233), DeliveryInf(155268, 88900), DeliveryInf(100827, 88900), DeliveryInf(206638, 26096), DeliveryInf(162830, 31111), DeliveryInf(194554, 88900)};
        depots = {143769, 141526, 188759};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(140865, 13961), DeliveryInf(70633, 156909), DeliveryInf(70633, 74478), DeliveryInf(17736, 168719), DeliveryInf(82603, 136839), DeliveryInf(27226, 108551), DeliveryInf(17736, 107865), DeliveryInf(17736, 139355)};
        depots = {72971, 84922, 104664};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(101726, 21699), DeliveryInf(90892, 101475), DeliveryInf(92428, 25687), DeliveryInf(33529, 71763), DeliveryInf(108431, 76597)};
        depots = {137690, 62291, 101288};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(143615, 195483), DeliveryInf(32689, 55240), DeliveryInf(74852, 189476)};
        depots = {33049, 70305, 85145};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(169229, 166909), DeliveryInf(107887, 19918), DeliveryInf(107887, 166909), DeliveryInf(136837, 199244), DeliveryInf(110440, 115001), DeliveryInf(47414, 199244), DeliveryInf(107887, 78603), DeliveryInf(169229, 199244), DeliveryInf(107887, 166909)};
        depots = {74807, 140755, 70902};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(210504, 48707), DeliveryInf(158965, 52432), DeliveryInf(74050, 48913)};
        depots = {81860, 70938, 196668};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(138321, 75378), DeliveryInf(72181, 121887), DeliveryInf(20617, 168848), DeliveryInf(174693, 97410), DeliveryInf(105358, 137896)};
        depots = {162141, 36162, 211150};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(146514, 102850), DeliveryInf(207505, 179041), DeliveryInf(22413, 44905), DeliveryInf(129491, 179041), DeliveryInf(48811, 21900), DeliveryInf(101227, 130175), DeliveryInf(187148, 179041), DeliveryInf(24107, 130175)};
        depots = {95306, 192387, 48804};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(196183, 169153), DeliveryInf(100017, 61101), DeliveryInf(128277, 92492), DeliveryInf(91716, 202919), DeliveryInf(195117, 155140)};
        depots = {88720, 76787, 78171};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(188218, 176698), DeliveryInf(175371, 211147)};
        depots = {102639, 134729};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(200127, 127426), DeliveryInf(182731, 171343)};
        depots = {161536};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(200704, 81357)};
        depots = {167960};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

        deliveries = {DeliveryInf(153529, 130403), DeliveryInf(203520, 75227), DeliveryInf(54887, 192910), DeliveryInf(21510, 175432), DeliveryInf(172093, 50740)};
        depots = {173213, 49946, 142192};
        turn_penalty = 15.000000000;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));

    } //simple_legality_new_york_usa

} //simple_legality_new_york_usa_public

