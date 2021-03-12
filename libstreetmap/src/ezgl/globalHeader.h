/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   globalClasses.h
 * Author: saeedgha
 *
 * Created on March 6, 2021, 1:50 AM
 */
#include <vector>
#include "LatLon.h" 
#include "OSMID.h"
#include "m1.h"
#include "m2.h"
#include <string>
#pragma once

const std::string MAP_NAMES[] = {
        "Beijing, China",
        "Cairo, Egypt",
        "Cape Town, South Africa",
        "Golden Horseshoe, Canada",
        "Hamilton, Canada",
        "Hong Kong, China",
        "Iceland",
        "Interlaken, Switzerland",
        "London, England",
        "Moscow, Russia",
        "New Delhi, India",
        "New York, USA",
        "Rio de Janeiro, Brazil",
        "Saint Helena",
        "Singapore",
        "Sydney, Australia",
        "Tehran, Iran",
        "Tokyo, Japan",
        "Toronto, Canada"
};

const std::string MAP_PATHS[] = {
        "/cad2/ece297s/public/maps/beijing_china.streets.bin",
        "/cad2/ece297s/public/maps/cairo_egypt.streets.bin",
        "/cad2/ece297s/public/maps/cape-town_south-africa.streets.bin",
        "/cad2/ece297s/public/maps/golden-horseshoe_canada.streets.bin",
        "/cad2/ece297s/public/maps/hamilton_canada.streets.bin",
        "/cad2/ece297s/public/maps/hong-kong_china.streets.bin",
        "/cad2/ece297s/public/maps/iceland.streets.bin",
        "/cad2/ece297s/public/maps/interlaken_switzerland.streets.bin",
        "/cad2/ece297s/public/maps/london_england.streets.bin",
        "/cad2/ece297s/public/maps/moscow_russia.streets.bin",
        "/cad2/ece297s/public/maps/new-delhi_india.streets.bin",
        "/cad2/ece297s/public/maps/new-york_usa.streets.bin",
        "/cad2/ece297s/public/maps/rio-de-janeiro_brazil.streets.bin",
        "/cad2/ece297s/public/maps/saint-helena.streets.bin",
        "/cad2/ece297s/public/maps/singapore.streets.bin",
        "/cad2/ece297s/public/maps/sydney_australia.streets.bin",
        "/cad2/ece297s/public/maps/tehran_iran.streets.bin",
        "/cad2/ece297s/public/maps/tokyo_japan.streets.bin",
        "/cad2/ece297s/public/maps/toronto_canada.streets.bin"

};

struct streetInfo{
    std::string name;
    std::vector<LatLon> positions;
};
extern std::vector<std::vector<LatLon>> points_on_segments;
extern std::vector<streetInfo> streetPositions;
extern std::vector<std::vector<std::pair<double, double>>> xy_points_segments;
extern double aspVar;
extern double avg_lat;
extern double avg_lon;
extern std::string userInput;
std::vector<LatLon> vectorOfCoord(StreetIdx street_id);
double x_from_lon(double lon);
double y_from_lat(double lat);



