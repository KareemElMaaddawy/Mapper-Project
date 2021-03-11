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
extern std::vector<std::vector<LatLon>> points_on_segments;

extern std::vector<std::vector<std::pair<float, float>>> xy_points_segments;
extern double aspVar;
extern double avg_lat;
extern double avg_lon;
extern std::string userInput;
std::vector<LatLon> vectorOfCoord(StreetIdx street_id);
double x_from_lon(double lon);
double y_from_lat(double lat);



