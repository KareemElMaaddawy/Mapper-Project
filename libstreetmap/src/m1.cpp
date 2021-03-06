/* 
 * Copyright 2021 University of Toronto
 *
 * Permission is hereby granted, to use this software and associated 
 * documentation files (the "Software") in course work at the University 
 * of Toronto, or for personal use. Other uses are prohibited, in 
 * particular the distribution of the Software either publicly or to third 
 * parties.
 *
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <iostream>
#include <cmath> // gain access to math constants and functions such as M_PI
#include "m1.h"
#include "StreetsDatabaseAPI.h"
#include "LatLon.h" // required to use the Latlon parameters (Latitude and longitdue)
#include <algorithm>
#include <utility>
#include "globalHeader.h"
#include <vector>
#include <map>
#include "Node.h"
#include <boost/functional/hash.hpp>
#include <unordered_map>
#include <bits/stdc++.h>

#define NO_EDGE -1
// loadMap will be called with the name of the file that stores the "layer-2"
// map data accessed through StreetsDatabaseAPI: the street and intersection
// data that is higher-level than the raw OSM data).
// This file name will always end in ".streets.bin" and you
// can call loadStreetsDatabaseBIN with this filename to initialize the
// layer 2 (StreetsDatabase) API.
// If you need data from the lower level, layer 1, API that provides raw OSM
// data (nodes, ways, etc.) you will also need to initialize the layer 1
// OSMDatabaseAPI by calling loadOSMDatabaseBIN. That function needs the
// name of the ".osm.bin" file that matches your map -- just change
// ".streets" to ".osm" in the map_streets_database_filename to get the proper
// name.

int numOfStreets;
std::string *streetNames;
std::multimap<std::string, int> streetNameMap;

int numOfStreetSegments;
double *segLength;
double *segTime;
double *stLength;
void fillNodes();
void loadAdjacentIntersections();
// Vector Declerations



std::vector<std::vector<std::string>> street_names_of_intersection; //stores the street names for each intersection
//Includes repetition!!
std::vector<std::vector<IntersectionIdx>> intersections_of_a_street;

std::vector<std::vector<StreetSegmentIdx>> segments_of_an_intersection;

std::vector<std::vector<IntersectionIdx>> adjacentIntersections;


std::vector<StreetSegmentInfo> street_segment_info; //vector that holds info struct of each street segment

typedef std::pair<StreetIdx, StreetIdx> pair;
std::unordered_map<std::pair<StreetIdx, StreetIdx>, std::vector<IntersectionIdx>, boost::hash<pair>>  intersections_of_two_streets;

double lengthHelper(StreetSegmentIdx street_segment_id);
double streetLengthHelper(StreetIdx street_id);
std::vector<double> fillVector(StreetIdx street_id);
// std::vector<LatLon> vectorOfCoord(StreetIdx street_id); ///defined in global declared in globalHeader

void loadAdjacentIntersections(){
    int numOfIntersections = getNumIntersections();
    adjacentIntersections.resize(numOfIntersections);
    for(int i = 0; i < numOfIntersections; ++i){
        int numOfSegments = getNumIntersectionStreetSegment(i);
        for(int j = 0; j < numOfSegments; ++j){
            StreetSegmentInfo segInfo = getStreetSegmentInfo(getIntersectionStreetSegment(i, j));
            if(segInfo.oneWay){
                if(i == segInfo.from){
                    adjacentIntersections[i].push_back(segInfo.to);
                }
            }else{
                if(i == segInfo.from){
                    adjacentIntersections[i].push_back(segInfo.to);
                }else{
                    adjacentIntersections[i].push_back(segInfo.from);
                }
            }
        }
    }
}

void fillNodes(){
    nodes.resize(getNumIntersections());

    for(int i = 0; i < getNumIntersections(); i++){
        nodes[i] = Node(i, NO_EDGE, std::numeric_limits<int>::max());
    }
}

bool loadMap(std::string map_streets_database_filename) {
    bool load_successful = loadStreetsDatabaseBIN(
            map_streets_database_filename); //Indicates whether the map has loaded successfully

    std::cout << "loadMap: " << map_streets_database_filename << std::endl;

    if (load_successful) {


        loadAdjacentIntersections();

        numOfStreetSegments = getNumStreetSegments();
        segLength = new double[numOfStreetSegments];
        segTime = new double[numOfStreetSegments];
        for(int i = 0; i < numOfStreetSegments; i++){
            double length = lengthHelper(i);
            segLength[i] = length;
            segTime[i] = length / getStreetSegmentInfo(i).speedLimit;
            street_segment_info.push_back(getStreetSegmentInfo(i));
        }
        stLength = new double[numOfStreets];
        for(int i = 0; i < numOfStreets; i++){
            double stlength = streetLengthHelper(i);
            stLength[i] = stlength;
        }

        streetNames = new std::string[numOfStreets]; //container to store streetnames

        for (int i = 0; i < numOfStreets; i++) { //formatting and storing street names w/o spaces and all lowercase
            streetNames[i] = getStreetName(i);
            streetNames[i].erase(std::remove(streetNames[i].begin(), streetNames[i].end(), ' '), streetNames[i].end());
            std::transform(streetNames[i].begin(), streetNames[i].end(), streetNames[i].begin(),
                           [](unsigned char c) { return std::tolower(c); });
            streetNameMap.insert(std::make_pair(streetNames[i], i));
        }

        segments_of_an_intersection.resize(getNumIntersections());

        street_names_of_intersection.resize(getNumIntersections());

        intersections_of_a_street.resize(getNumStreets());

        for (int intersection = 0; intersection < getNumIntersections(); intersection++) {
            for (int segment = 0; segment < getNumIntersectionStreetSegment(intersection); segment++) {

//                /*SEGMENTS OF AN INTERSECTION*/
//                //get the index of the segment you're on
                int streetSeg_id = getIntersectionStreetSegment(intersection, segment);
//                //for each intersection add each segment id to that intersection's associated sub vector
                segments_of_an_intersection[intersection].push_back(streetSeg_id);
//
//                /*FINDING THE STREET NAMES THAT PASS THROUGH AN INTERSECTION*/
//                //gets the street id of a specific segment
                StreetIdx street_ID_of_segment = getStreetSegmentInfo(streetSeg_id).streetID;
//                //stores the name at specified street id in the street names function
//                street_names_of_intersection[intersection].push_back(getStreetName(street_ID_of_segment));

                /*FINDING INTERSECTIONS OF A STREET*/
                ///if the intersection does not exists on the list of intersections of a street, then add it
                if (!(std::find(intersections_of_a_street[street_ID_of_segment].begin(),
                                intersections_of_a_street[street_ID_of_segment].end(), intersection) !=
                      intersections_of_a_street[street_ID_of_segment].end())) {
                    intersections_of_a_street[street_ID_of_segment].push_back(intersection);
                }



            }

        }




        /*TOO FEW CONNECTIONS*/
        points_on_segments.resize(getNumStreetSegments());
        for (StreetSegmentIdx segment = 0; segment < getNumStreetSegments(); ++segment){
            int total_number_of_points = getStreetSegmentInfo(segment).numCurvePoints;
            if(total_number_of_points == 0){
                IntersectionIdx toIntersection = getStreetSegmentInfo(segment).to;
                IntersectionIdx fromIntersection = getStreetSegmentInfo(segment).from;
                LatLon toPoint = getIntersectionPosition(toIntersection);
                LatLon fromPoint = getIntersectionPosition(fromIntersection);

                points_on_segments[segment].push_back(toPoint);
                points_on_segments[segment].push_back(fromPoint);
            }
            for(int point = 0; point < total_number_of_points; ++point){
                points_on_segments[segment].push_back(getStreetSegmentCurvePoint(segment, point));
            }
        }
        xy_points_segments.resize(getNumStreetSegments());

        for (StreetSegmentIdx segment = 0; segment < getNumStreetSegments(); segment++){
            for(int point = 0; point < points_on_segments[segment].size(); ++point){
                float lon = points_on_segments[segment][point].longitude();
                float lat = points_on_segments[segment][point].latitude();
                float x = x_from_lon(lon);
                float y = y_from_lat(lat);
                    std::pair<float, float> xy = {x, y};
                xy_points_segments[segment].push_back(xy);

            }
        }

        /*TOO MANY CONNECTIONS*/
//        points_on_segments.resize(numOfStreets);
//        xy_points_segments.resize(numOfStreets);
//        for(int street = 0; street < numOfStreets; ++street){
//            std::vector<LatLon> allStreetPoints = vectorOfCoord(street);
//            int sizeOfStreet = allStreetPoints.size();
//            for(int point = 0; point < sizeOfStreet; ++point){
//                float lon = allStreetPoints[point].longitude();
//                float lat = allStreetPoints[point].latitude();
//                float x = x_from_lon(lon);
//                float y = y_from_lat(lat);
//                    std::pair<float, float> xy = {x, y};
//                    xy_points_segments[street].push_back(xy);
//            }
//        }
        //create vector that holds streets and it's positionss
        streetPositions.resize(getNumStreets());
        for(int i = 0; i < getNumStreets(); i++){
            streetPositions[i].name = getStreetName(i);
            for(int j = 0; j < intersections_of_a_street[i].size() ; j++){

                streetPositions[i].positions.push_back(getIntersectionPosition(intersections_of_a_street[i][j]));
            }
        }

        fillNodes();

    }return load_successful;
}

void closeMap() {
    //Clean-up your map related data structures here
    closeStreetDatabase();

    street_names_of_intersection.clear();
    intersections_of_a_street.clear();
    segments_of_an_intersection.clear();
    adjacentIntersections.clear();

    streetNameMap.clear();

    delete[] streetNames;
    delete[] segLength;
    delete[] segTime;
}

// Returns the nearest point of interest of the given name to the given position
// Speed Requirement --> none
POIIdx findClosestPOI(LatLon my_position, std::string POIname) {
    double smallestDistance = 99999999999;
    double distanceToCurrentPOI = 0;
    int totalNumOfPOI = getNumPointsOfInterest();
    POIIdx closestPOIIdx = -1;

    for (int i = 0; i < totalNumOfPOI; i++) {
        if (getPOIName(i) == POIname) {
            distanceToCurrentPOI = findDistanceBetweenTwoPoints(std::make_pair(my_position, getPOIPosition(i)));

            if (distanceToCurrentPOI <
                smallestDistance) { //stores value of smallest distance and the POI index for it
                smallestDistance = distanceToCurrentPOI;
                closestPOIIdx = i;
            }
        }
    }
    return closestPOIIdx;
}


std::vector<double> fillVector(StreetIdx street_id){
    std::vector<double> filledUpVector;
    for(int i = 0; i < getNumStreetSegments(); i++){
        if(getStreetSegmentInfo(i).streetID == street_id){
            filledUpVector.push_back(i);
        }
    }
    return filledUpVector;
}

std::vector<LatLon> vectorOfCoord(StreetIdx street_id){
    std::vector<double> streetSegs = fillVector(street_id);
    std::vector<LatLon> tempVec;
    for(std::vector<double>::iterator it = streetSegs.begin(); it != streetSegs.end(); it++){
        if(getStreetSegmentInfo(*it).numCurvePoints == 0){
            tempVec.push_back(getIntersectionPosition(getStreetSegmentInfo(*it).from));
            tempVec.push_back(getIntersectionPosition(getStreetSegmentInfo(*it).to));
        }else if(getStreetSegmentInfo(*it).numCurvePoints > 0){
            tempVec.push_back(getIntersectionPosition(getStreetSegmentInfo(*it).from));
            tempVec.push_back(getIntersectionPosition(getStreetSegmentInfo(*it).to));
            for(int i = 0; i < getStreetSegmentInfo(*it).numCurvePoints; i++){
                tempVec.push_back(getStreetSegmentCurvePoint(*it,i));
            }
        }
    }
    return tempVec;
}



// Returns the area of the given closed feature in square meters
// Assume a non self-intersecting polygon (i.e. no holes)
// Return 0 if this feature is not a closed polygon.
// Speed Requirement --> moderate
double findFeatureArea(FeatureIdx feature_id) {
    double area = 0;
    double yMax = -999999;
    double avgLat = 0;

    int numOfPoints = getNumFeaturePoints(feature_id);//finds number of points on the feature

    LatLon *featurePoints = new LatLon[numOfPoints];

    for (int i = 0; i < numOfPoints; i++) {
        featurePoints[i] = getFeaturePoint(feature_id, i);//creates array of featurePoints
        avgLat += kDegreeToRadian *featurePoints[i].latitude();
    }

    avgLat = avgLat / numOfPoints;

    double *x = new double[numOfPoints];
    double *y = new double[numOfPoints];
    double radLat, radLong;

    for (int i = 0; i < numOfPoints; i++) {
        radLat = kDegreeToRadian *featurePoints[i].latitude();
        radLong = kDegreeToRadian *featurePoints[i].longitude();

        x[i] = kEarthRadiusInMeters * radLong * cos(avgLat);
        y[i] = kEarthRadiusInMeters * radLat;

        if (y[i] > yMax) yMax = y[i];
    }
    if (x[0] == x[numOfPoints - 1] && y[0] == y[numOfPoints - 1]) {
        for (int i = 0; i < numOfPoints; i++) {
            if (i == numOfPoints - 1) {
                area += x[i] * y[0] - y[i] * x[0];
            } else {
                area += x[i] * y[i + 1] - y[i] * x[i + 1];
            }
        }
        area = fabs(area / 2);

        delete[] x;
        delete[] y;
        delete[] featurePoints;

        return area;
    } else {
        delete[] x;
        delete[] y;
        delete[] featurePoints;

        return 0;
    }
}

// Returns all street ids corresponding to street names that start with the
// given prefix
// The function should be case-insensitive to the street prefix.
// The function should ignore spaces.
//  For example, both "bloor " and "BloOrst" are prefixes to
// "Bloor Street East".
// If no street names match the given prefix, this routine returns an empty
// (length 0) vector.
// You can choose what to return if the street prefix passed in is an empty
// (length 0) string, but your program must not crash if street_prefix is a
// length 0 string.
// Speed Requirement --> high
std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix) {
    street_prefix.erase(std::remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    std::vector<StreetIdx> prefixStreetIds;

    int prefixLength = street_prefix.length();
    for(auto key = streetNameMap.lower_bound(street_prefix); key != streetNameMap.end() && key->first[0] == street_prefix[0]; ++key){
        if(key->first.substr(0,prefixLength) == street_prefix){
            prefixStreetIds.push_back(key->second);
        }
    }

    return prefixStreetIds;
}

double findDistanceBetweenTwoPoints(std::pair<LatLon, LatLon> points) {
    // Converting latitude and longitudes from degrees to radians
    double lat1 = kDegreeToRadian * (points.first.latitude());
    double long1 = kDegreeToRadian * (points.first.longitude());
    double lat2 = kDegreeToRadian * (points.second.latitude());
    double long2 = kDegreeToRadian * (points.second.longitude());

    double latAvg = (lat2 + lat1) / 2;

    return (sqrt(pow(kEarthRadiusInMeters*(lat2-lat1), 2) + pow(kEarthRadiusInMeters*cos(latAvg)*(long2-long1), 2)));
}

//LatLonBounds findStreetBoundingBox(StreetIdx street_id) {
  //  return 0;
//}



double streetLengthHelper(StreetIdx street_id){
    double length = 0;
    std::vector<double> tempVec = fillVector(street_id);
    for(std::vector<double>::iterator it = tempVec.begin(); it != tempVec.end(); it++){
        length += findStreetSegmentLength(*it);
    }
    return length;
}

double findStreetLength(StreetIdx street_id){
    return stLength[street_id];
}
double findStreetSegmentLength(StreetSegmentIdx street_segment_id){
    return segLength[street_segment_id];
}

double lengthHelper(StreetSegmentIdx street_segment_id) {
    //case if no curve points exist
    double length = 0;
    IntersectionIdx from = getStreetSegmentInfo(street_segment_id).from;
    IntersectionIdx to = getStreetSegmentInfo(street_segment_id).to;
    int numOfCurvePoints = getStreetSegmentInfo(street_segment_id).numCurvePoints;
    if (numOfCurvePoints == 0) {
        std::pair<LatLon, LatLon> point;
        point = std::make_pair(getIntersectionPosition(from), getIntersectionPosition(to));
        length = findDistanceBetweenTwoPoints(point);
    }
        //case if there is only one curve point
    else if (numOfCurvePoints == 1) {
        std::pair<LatLon, LatLon> pair1;
        std::pair<LatLon, LatLon> pair2;
        pair1 = std::make_pair(getIntersectionPosition(from),
                               getStreetSegmentCurvePoint(street_segment_id, 0));
        pair2 = std::make_pair(getStreetSegmentCurvePoint(street_segment_id, 0),
                               getIntersectionPosition(to));
        length = findDistanceBetweenTwoPoints(pair1) + findDistanceBetweenTwoPoints(pair2);
    } else {
        LatLon previousPoint;
        for (int i = 0; i < numOfCurvePoints; i++) {
            if(i == 0) {
                std::pair<LatLon, LatLon> fromToFirstCurve = std::make_pair(getIntersectionPosition(from),
                                                                            getStreetSegmentCurvePoint(
                                                                                    street_segment_id, 0));
                length += findDistanceBetweenTwoPoints(fromToFirstCurve);
                previousPoint = getStreetSegmentCurvePoint(street_segment_id, 0);
            }else{
                std::pair<LatLon, LatLon> curveToCurve = std::make_pair(getStreetSegmentCurvePoint(street_segment_id, i), previousPoint);
                length += findDistanceBetweenTwoPoints(curveToCurve);
                previousPoint = getStreetSegmentCurvePoint(street_segment_id, i);
            }
        }
        std::pair<LatLon, LatLon> lastCurvePointToTo = std::make_pair(getIntersectionPosition(to), previousPoint);
        length += findDistanceBetweenTwoPoints(lastCurvePointToTo);
    }
    return length;
}

double findStreetSegmentTravelTime(StreetSegmentIdx street_segment_id) {
    // return the speed which is length/time
    return segTime[street_segment_id];
}




//GHAMR'S FUNCTIONS////////////////////////////////////////////////////////////////////



// Return all intersection ids at which the two given streets intersect
// This function will typically return one intersection id for streets
// that intersect and a length 0 vector for streets that do not. For unusual
// curved streets it is possible to have more than one intersection at which
// two streets cross.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high
std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(std::pair<StreetIdx, StreetIdx> street_ids){
     IntersectionIdx first_street_idx = street_ids.first;
    IntersectionIdx second_street_idx = street_ids.second;
    if(first_street_idx != second_street_idx) {
        //get a vector of the intersections of the first street
        std::vector<IntersectionIdx> intersections_of_first_street = intersections_of_a_street[first_street_idx];
        //get a vector of intersections of the second street
        std::vector<IntersectionIdx> intersections_of_second_street = intersections_of_a_street[second_street_idx];
        std::vector<int>::iterator it;
        //sort the 2 vectors
        std::sort(intersections_of_first_street.begin(), intersections_of_first_street.end());
        std::sort(intersections_of_second_street.begin(), intersections_of_second_street.end());

        //make a vector to store the common intersections between the 2 streets
        std::vector<IntersectionIdx> common_intersections(intersections_of_first_street.size() + intersections_of_second_street.size());

        //search the two vectors for any common elements and put them into the vector of common intersections
        it = std::set_intersection(intersections_of_first_street.begin(), intersections_of_first_street.end(),
                              intersections_of_second_street.begin(), intersections_of_second_street.end(),
                              common_intersections.begin());
        common_intersections.resize(it-common_intersections.begin());
        return common_intersections;
    }else{
        return intersections_of_a_street[first_street_idx];
    }

}


// Returns the street names at the given intersection (includes duplicate 
// street names in the returned vector)
// Speed Requirement --> high 
std::vector<std::string> findStreetNamesOfIntersection(IntersectionIdx intersection_id) {

    return street_names_of_intersection[intersection_id];
}

// Returns the street segments that connect to the given intersection 
// Speed Requirement --> high
std::vector<StreetSegmentIdx> findStreetSegmentsOfIntersection(IntersectionIdx intersection_id) {
    return segments_of_an_intersection[intersection_id];
}

// Returns all intersections along the a given street.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high
std::vector<IntersectionIdx> findIntersectionsOfStreet(StreetIdx street_id) {


    return intersections_of_a_street[street_id];

}

// Returns all intersections reachable by traveling down one street segment 
// from the given intersection (hint: you can't travel the wrong way on a 
// 1-way street)
// the returned vector should NOT contain duplicate intersections
// Speed Requirement --> high 
std::vector<IntersectionIdx> findAdjacentIntersections(IntersectionIdx intersection_id) {

    return adjacentIntersections[intersection_id];
}

// Returns the nearest intersection to the given position
// Speed Requirement --> none
//helpers: findDistanceBetweenTwoPoints
/*logic:
 Assign an arbitrary intersection to be the closest so far
 find the position of that intersection and the distance between that intersection and my position using findDistanceBetweenTwoPoints
 loop through all the intersections
 while looping, find the current intersection's position and it's distance using findDistanceBetweenTwoPoints
 compare the current intersections distance form my position to that of the closest intersection
 If it is less make the current intersection the closest intersection and make the current distance the closest distance
 once done looping return the closest intersection*/

//this code should be readable with no comments, please let me know if you would like to see it commented
IntersectionIdx findClosestIntersection(LatLon my_position) {

    IntersectionIdx closest_so_far = 0;
    LatLon position_of_closest = getIntersectionPosition(closest_so_far);
    long double distance_from_closest_so_far = findDistanceBetweenTwoPoints(std::make_pair(my_position, position_of_closest));

    for (int intersection = 0; intersection < getNumIntersections(); intersection++){
        LatLon current_intersection_position = getIntersectionPosition(intersection);
        double distance_from_current_intersection = findDistanceBetweenTwoPoints(std::make_pair(my_position, current_intersection_position));
        if (distance_from_current_intersection < distance_from_closest_so_far){
            closest_so_far = intersection;
            distance_from_closest_so_far = distance_from_current_intersection;
        }

    }

    return closest_so_far;
}