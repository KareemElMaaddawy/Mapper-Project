/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "globalHeader.h"
#include <vector>
#include <unordered_map>
#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m2.h"
#include <cmath>
#include <string>
int selectedMap = 18;
bool colorBlind  = false;
bool poiFilterActive = false;
bool showPath = false;
std::vector<std::string> poiFilter;
std::string osmFilePath = "/cad2/ece297s/public/maps/toronto_canada.osm.bin";
std::unordered_map<OSMID, OSMWay*> idToWay;
std::unordered_map<StreetSegmentIdx, std::string> streetTypes;
std::vector<streetInfo> streetPositions;

double aspVar = 0;
std::string userInput = {};
std::vector<std::vector<LatLon>> points_on_segments;
std::vector<std::vector<std::pair<double, double>>> xy_points_segments;
void populateListStore();
double x_from_lon(double lon) {
    return lon * kDegreeToRadian * kEarthRadiusInMeters * std::cos(mapInfo[selectedMap].avgLat * kDegreeToRadian);
}

double y_from_lat(double lat) {
    return lat * kDegreeToRadian * kEarthRadiusInMeters;
}
