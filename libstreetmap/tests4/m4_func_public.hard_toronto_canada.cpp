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


SUITE(hard_toronto_canada_public) {
    TEST(hard_toronto_canada) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(134309, 12549), DeliveryInf(70077, 116527), DeliveryInf(50598, 138497), DeliveryInf(114749, 139057), DeliveryInf(144736, 131725), DeliveryInf(125464, 115497), DeliveryInf(32317, 92094), DeliveryInf(63694, 5503), DeliveryInf(113763, 47015), DeliveryInf(11108, 83481), DeliveryInf(87153, 89090), DeliveryInf(75474, 50466), DeliveryInf(136819, 89235), DeliveryInf(97700, 17097), DeliveryInf(9091, 27572), DeliveryInf(49683, 124009), DeliveryInf(29465, 146120), DeliveryInf(16878, 130519), DeliveryInf(94887, 114917), DeliveryInf(70858, 115629), DeliveryInf(36474, 52059), DeliveryInf(31690, 30193), DeliveryInf(138232, 27681), DeliveryInf(148245, 88762), DeliveryInf(53464, 78521), DeliveryInf(102473, 121308), DeliveryInf(82599, 147449), DeliveryInf(114469, 90306), DeliveryInf(71324, 25945), DeliveryInf(145875, 60419), DeliveryInf(10654, 14447), DeliveryInf(82270, 35746), DeliveryInf(146436, 129059), DeliveryInf(95739, 125804), DeliveryInf(144781, 26707), DeliveryInf(120266, 79420), DeliveryInf(126384, 48274), DeliveryInf(24695, 38734), DeliveryInf(17908, 10473), DeliveryInf(7256, 122900), DeliveryInf(86145, 30422), DeliveryInf(54359, 31561), DeliveryInf(67399, 146690), DeliveryInf(143962, 23193), DeliveryInf(51138, 42637), DeliveryInf(150157, 70239), DeliveryInf(135889, 81376), DeliveryInf(72768, 79021), DeliveryInf(57816, 102471), DeliveryInf(54403, 4831), DeliveryInf(67927, 3466), DeliveryInf(67619, 137899), DeliveryInf(151370, 102251), DeliveryInf(103348, 37524), DeliveryInf(131114, 10461), DeliveryInf(84740, 8820), DeliveryInf(121665, 130081), DeliveryInf(55111, 2381), DeliveryInf(145952, 124749), DeliveryInf(40202, 36838), DeliveryInf(105676, 31369), DeliveryInf(12665, 87984), DeliveryInf(122532, 130880), DeliveryInf(145581, 90374), DeliveryInf(24806, 4688), DeliveryInf(19761, 85361), DeliveryInf(143961, 49067), DeliveryInf(64132, 7136), DeliveryInf(147983, 142498), DeliveryInf(102290, 122349), DeliveryInf(56218, 135923), DeliveryInf(28294, 21571), DeliveryInf(132386, 84536), DeliveryInf(42314, 117240), DeliveryInf(135794, 135609), DeliveryInf(71023, 27321), DeliveryInf(79014, 1099), DeliveryInf(34080, 98597), DeliveryInf(74734, 1455), DeliveryInf(73316, 139297), DeliveryInf(53584, 139432), DeliveryInf(41731, 19590), DeliveryInf(124944, 74340), DeliveryInf(24470, 72513), DeliveryInf(81048, 33400), DeliveryInf(124344, 140589), DeliveryInf(137559, 116132), DeliveryInf(73657, 16849), DeliveryInf(109439, 146764), DeliveryInf(43986, 134916), DeliveryInf(106680, 111928), DeliveryInf(148957, 83453), DeliveryInf(150907, 81386), DeliveryInf(145536, 87241), DeliveryInf(86575, 46154), DeliveryInf(58131, 113093), DeliveryInf(7194, 100548), DeliveryInf(142386, 136851), DeliveryInf(112921, 18900), DeliveryInf(77019, 115997)};
        depots = {17, 64502, 1026, 69492, 127404, 143891, 87216, 150201, 47718, 108862};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR hard_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR hard_toronto_canada: INVALID" << std::endl;
        }

    } //hard_toronto_canada

} //hard_toronto_canada_public

