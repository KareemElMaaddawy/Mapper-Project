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

extern void set_disable_event_loop (bool new_setting);

std::string map_name = "/cad2/ece297s/public/maps/saint-helena.streets.bin";

int main(int argc, char** argv) {
    //Disable interactive graphics
    set_disable_event_loop(true);

    bool load_success = loadMap(map_name);

    if(!load_success) {
        std::cout << "ERROR: Could not load map file: '" << map_name << "'!";
        std::cout << " Subsequent tests will likely fail." << std::endl;
        //Don't abort tests, since we still want to show that all
        //tests fail.
    }

    //Run the unit tests
    int num_failures = UnitTest::RunAllTests();

    closeMap();

    return num_failures;
}

SUITE(valgrind) {
    TEST(valgrind_saint_helena) {
        std::vector<DeliveryInf> deliveries;
        std::vector<IntersectionIdx> depots;
        std::vector<CourierSubPath> result_path;
        float turn_penalty;
        
        deliveries = {DeliveryInf(197, 292)};
        depots = {168};
        turn_penalty = 15.0;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));
        
        deliveries = {DeliveryInf(243, 167), DeliveryInf(326, 295), DeliveryInf(167, 96), DeliveryInf(273,62), DeliveryInf(314,324)};
        depots = {307, 101, 189};
        turn_penalty = 15.0;
        result_path = travelingCourier(deliveries, depots, turn_penalty);
        CHECK(courier_path_is_legal(deliveries, depots, result_path));
    }
}
