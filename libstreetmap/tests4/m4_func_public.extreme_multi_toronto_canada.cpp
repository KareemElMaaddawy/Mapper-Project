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


SUITE(extreme_multi_toronto_canada_public) {
    TEST(extreme_multi_toronto_canada) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        float turn_penalty;
        std::vector<CourierSubPath> result_path;
        bool is_legal;

        deliveries = {DeliveryInf(141634, 127054), DeliveryInf(95416, 20671), DeliveryInf(109835, 43893), DeliveryInf(138416, 109151), DeliveryInf(48116, 122039), DeliveryInf(79938, 56365), DeliveryInf(90926, 3521), DeliveryInf(131063, 43893), DeliveryInf(24857, 133675), DeliveryInf(25981, 15792), DeliveryInf(89283, 140137), DeliveryInf(27925, 131465), DeliveryInf(11418, 102019), DeliveryInf(115213, 61167), DeliveryInf(12201, 89072), DeliveryInf(119680, 125833), DeliveryInf(78513, 22086), DeliveryInf(28016, 40673), DeliveryInf(53733, 102333), DeliveryInf(69495, 133675), DeliveryInf(143838, 105498), DeliveryInf(92505, 135909), DeliveryInf(26361, 142379), DeliveryInf(22307, 127054), DeliveryInf(72295, 46695), DeliveryInf(53434, 86214), DeliveryInf(53434, 125954), DeliveryInf(103416, 84652), DeliveryInf(73295, 56781), DeliveryInf(118383, 67981), DeliveryInf(54782, 106428), DeliveryInf(18075, 110023), DeliveryInf(113638, 133675), DeliveryInf(18075, 138296), DeliveryInf(72295, 66597), DeliveryInf(82041, 21160), DeliveryInf(123128, 60507), DeliveryInf(145746, 99349), DeliveryInf(82092, 3417), DeliveryInf(60106, 87285), DeliveryInf(94837, 61942), DeliveryInf(133611, 108087), DeliveryInf(95199, 91934), DeliveryInf(111188, 98980), DeliveryInf(98045, 83801), DeliveryInf(51708, 134254), DeliveryInf(102979, 63472), DeliveryInf(27925, 135443), DeliveryInf(83804, 59978), DeliveryInf(14215, 76286), DeliveryInf(86975, 150957), DeliveryInf(113638, 42084), DeliveryInf(69495, 59186), DeliveryInf(115213, 23536), DeliveryInf(8805, 50403), DeliveryInf(72295, 17680), DeliveryInf(115213, 22673), DeliveryInf(72295, 102582), DeliveryInf(69495, 53303), DeliveryInf(111183, 43893), DeliveryInf(130568, 100897), DeliveryInf(89646, 115510), DeliveryInf(132954, 125954), DeliveryInf(144599, 15792), DeliveryInf(72723, 55008), DeliveryInf(13704, 1255), DeliveryInf(87461, 10241), DeliveryInf(47239, 84703), DeliveryInf(61960, 147035), DeliveryInf(18075, 48832), DeliveryInf(17941, 84703), DeliveryInf(107, 59186), DeliveryInf(48166, 109973), DeliveryInf(57835, 83801), DeliveryInf(148375, 61563), DeliveryInf(113638, 108706), DeliveryInf(111188, 22009), DeliveryInf(73295, 17186), DeliveryInf(27373, 61764), DeliveryInf(69495, 125954), DeliveryInf(14215, 81139), DeliveryInf(14215, 69498), DeliveryInf(45270, 79133), DeliveryInf(4608, 116826), DeliveryInf(18075, 6620), DeliveryInf(61960, 117462), DeliveryInf(52769, 21234), DeliveryInf(139336, 98980), DeliveryInf(139586, 141853), DeliveryInf(105024, 57462), DeliveryInf(127073, 98954), DeliveryInf(73295, 133558), DeliveryInf(142373, 40673), DeliveryInf(59049, 52523), DeliveryInf(69533, 29198), DeliveryInf(69495, 135909), DeliveryInf(77045, 125954), DeliveryInf(27925, 69544), DeliveryInf(81146, 80764), DeliveryInf(33981, 135594), DeliveryInf(69495, 64076), DeliveryInf(106448, 102144), DeliveryInf(40821, 37810), DeliveryInf(83295, 135909), DeliveryInf(44231, 48929), DeliveryInf(111188, 149341), DeliveryInf(74458, 113090), DeliveryInf(49031, 63472), DeliveryInf(135955, 40673), DeliveryInf(97070, 20671), DeliveryInf(101910, 87999), DeliveryInf(63426, 88208), DeliveryInf(113638, 61563), DeliveryInf(59607, 56365), DeliveryInf(53733, 142859), DeliveryInf(95901, 75436), DeliveryInf(124838, 125954), DeliveryInf(51737, 49915), DeliveryInf(111188, 17186), DeliveryInf(137832, 96143), DeliveryInf(105024, 29621), DeliveryInf(48286, 149951), DeliveryInf(73295, 83801), DeliveryInf(73847, 10415), DeliveryInf(73295, 58342), DeliveryInf(131063, 52547), DeliveryInf(33884, 72325), DeliveryInf(31438, 113213), DeliveryInf(61960, 141853), DeliveryInf(112928, 10498), DeliveryInf(105024, 37414), DeliveryInf(143838, 31856), DeliveryInf(142466, 142710), DeliveryInf(115213, 61563), DeliveryInf(50190, 111059), DeliveryInf(77836, 8599), DeliveryInf(93980, 20651), DeliveryInf(60651, 17186), DeliveryInf(14215, 107829), DeliveryInf(126915, 61596), DeliveryInf(116430, 146433), DeliveryInf(107838, 84703), DeliveryInf(89232, 39794), DeliveryInf(41131, 136155), DeliveryInf(81047, 146700), DeliveryInf(51517, 59978), DeliveryInf(63013, 134587), DeliveryInf(99859, 61405), DeliveryInf(111188, 127054), DeliveryInf(74798, 73062), DeliveryInf(115213, 63472), DeliveryInf(48116, 146877), DeliveryInf(73295, 65581), DeliveryInf(126450, 15792), DeliveryInf(113638, 17186), DeliveryInf(53733, 23304), DeliveryInf(60039, 56365), DeliveryInf(27728, 35536), DeliveryInf(146419, 142859), DeliveryInf(101399, 17427), DeliveryInf(69495, 20651), DeliveryInf(112163, 50403), DeliveryInf(72295, 138241), DeliveryInf(48286, 1878), DeliveryInf(140551, 83801), DeliveryInf(5053, 15598), DeliveryInf(103572, 98980), DeliveryInf(36856, 78633), DeliveryInf(93524, 72325), DeliveryInf(72841, 146700), DeliveryInf(65263, 145043), DeliveryInf(115213, 127054), DeliveryInf(113638, 28111), DeliveryInf(42232, 134915), DeliveryInf(3757, 133675), DeliveryInf(98529, 141339), DeliveryInf(61974, 66173), DeliveryInf(55336, 39213), DeliveryInf(53733, 51364), DeliveryInf(111188, 65043), DeliveryInf(53434, 376), DeliveryInf(138126, 119087), DeliveryInf(89923, 114072), DeliveryInf(96053, 143586), DeliveryInf(77239, 97009), DeliveryInf(131063, 55457), DeliveryInf(6085, 50403), DeliveryInf(61960, 35081), DeliveryInf(108571, 149341), DeliveryInf(117878, 86796), DeliveryInf(140601, 10955), DeliveryInf(68383, 60869), DeliveryInf(48116, 111967), DeliveryInf(49319, 94882), DeliveryInf(144468, 98980), DeliveryInf(134741, 76673), DeliveryInf(48286, 59186), DeliveryInf(113638, 139587), DeliveryInf(113638, 142727), DeliveryInf(18075, 61563), DeliveryInf(115213, 48261), DeliveryInf(100375, 43893), DeliveryInf(14215, 76286), DeliveryInf(61960, 67183), DeliveryInf(53865, 141853), DeliveryInf(53733, 48929), DeliveryInf(73295, 61563), DeliveryInf(31887, 113483), DeliveryInf(111188, 59576), DeliveryInf(138107, 104853), DeliveryInf(48116, 59978), DeliveryInf(41512, 103362), DeliveryInf(18075, 35024), DeliveryInf(14215, 41015), DeliveryInf(88039, 41304), DeliveryInf(80957, 122376), DeliveryInf(53434, 120790), DeliveryInf(132065, 79057), DeliveryInf(39507, 48929), DeliveryInf(25934, 72325), DeliveryInf(88539, 13495), DeliveryInf(5643, 131465), DeliveryInf(36907, 141853), DeliveryInf(129612, 35844), DeliveryInf(133614, 131318), DeliveryInf(111188, 83801), DeliveryInf(79017, 103362), DeliveryInf(59397, 123941), DeliveryInf(111188, 59186), DeliveryInf(115213, 21193), DeliveryInf(9171, 48929), DeliveryInf(33981, 15792), DeliveryInf(14215, 3695), DeliveryInf(113373, 20651), DeliveryInf(48116, 131036), DeliveryInf(73295, 117860), DeliveryInf(72295, 60507), DeliveryInf(69495, 14854), DeliveryInf(45551, 100027), DeliveryInf(131063, 65592), DeliveryInf(145746, 128875), DeliveryInf(115213, 109843), DeliveryInf(5064, 59186), DeliveryInf(68219, 90765), DeliveryInf(26278, 24029), DeliveryInf(44888, 131465), DeliveryInf(88601, 148267), DeliveryInf(138739, 59978), DeliveryInf(86760, 10415), DeliveryInf(117987, 131465), DeliveryInf(64949, 1878), DeliveryInf(72295, 3609), DeliveryInf(1813, 41407), DeliveryInf(122371, 24561), DeliveryInf(33981, 72691), DeliveryInf(53733, 41494), DeliveryInf(69495, 72325), DeliveryInf(48116, 131465), DeliveryInf(14215, 15792), DeliveryInf(77486, 22009)};
        depots = {27, 103203, 123011, 20161, 112819};
        turn_penalty = 15.000000000;
        {
        	ECE297_TIME_CONSTRAINT(50000);
        	
        	result_path = travelingCourier(deliveries, depots, turn_penalty);
        }

        is_legal = courier_path_is_legal(deliveries, depots, result_path);
        CHECK(is_legal);

        if(is_legal) {
        	double path_cost = ece297test::compute_courier_path_travel_time(result_path, turn_penalty);
        	std::cout << "QoR extreme_multi_toronto_canada: " << path_cost << std::endl;
        } else {
        	std::cout << "QoR extreme_multi_toronto_canada: INVALID" << std::endl;
        }

    } //extreme_multi_toronto_canada

} //extreme_multi_toronto_canada_public

