/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   test_main.cpp
 * Author: ahmadda4
 *
 * Created on February 6, 2021, 1:24 PM
 */

#include <iostream>
#include "m1.h"
#include "LatLon.h"

int testClosestPOI();

int main() {
    
    std::string map_path = "/cad2/ece297s/public/maps/saint-helena.streets.bin";
    
    bool load_success = loadMap(map_path);
    if(!load_success) {
        std::cerr << "Failed to load map '" << map_path << "'\n";
        return 0;
    }
    std::cout << "Successfully loaded map '" << map_path << "'\n";
    
    int numOfError = 0;
    
    numOfError += testClosestPOI();
    
    std::cout << "number of errors: " << numOfError << std::endl;
    return 0;
}

int testFindStreetNamesOfIntersection(){
    
}


int testClosestPOI(){
//    LatLon *testPositions = new LatLon[3];
//    std::string *testNames = new std::string[3];
    
    LatLon position = new LatLon(-15.9498389,-5.6811358);
    std::string name = "St.Helena Golf Club";
    POIIdx found = findClosestPOI(position, name);
    std::cout << found << std::endl;
}
