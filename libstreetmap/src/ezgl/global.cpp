/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "globalHeader.h"
#include <vector>
#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include <cmath>
#include <string>
double aspVar = 0;
double avg_lat = 0;
double avg_lon = 0;
std::string userInput = {};
std::vector<std::vector<LatLon>> points_on_segments;
std::vector<std::vector<std::pair<float, float>>> xy_points_segments;


double x_from_lon(double lon) {
    return lon * kDegreeToRadian * kEarthRadiusInMeters * std::cos(avg_lat * kDegreeToRadian);
}

double y_from_lat(double lat) {
    return lat * kDegreeToRadian * kEarthRadiusInMeters;
}