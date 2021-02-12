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
#include "TrieNode.h"
#include <vector>
#include <map>

#define RADIUS_OF_EARTH 6371000

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
struct TrieNode *root; //root for streetnames trie

double degToRad(double degree);//helper to convert degrees to radians

std::vector<std::vector<std::string>> street_names_of_intersection; //stores the street names for each intersection
//Includes repetition!!
std::vector<std::vector<IntersectionIdx>> intersections_of_a_street;

std::vector<std::vector<StreetSegmentIdx>> segments_of_an_intersection;

std::vector<std::vector<IntersectionIdx>> adjacent_intersections;


std::vector<StreetSegmentInfo> street_segment_info; //vector that holds info struct of each street segment

std::vector<std::vector<std::pair<StreetIdx, StreetIdx>>> intersections_of_two_streets;


bool loadMap(std::string map_streets_database_filename) {
    bool load_successful = loadStreetsDatabaseBIN(
            map_streets_database_filename); //Indicates whether the map has loaded successfully

    std::cout << "loadMap: " << map_streets_database_filename << std::endl;

    if (load_successful) {
        numOfStreets = getNumStreets();
        streetNames = new std::string[numOfStreets]; //container to store streetnames

        for (int i = 0; i < numOfStreets; i++) { //formatting and storing street names w/o spaces and all lowercase
            streetNames[i] = getStreetName(i);
            streetNames[i].erase(std::remove(streetNames[i].begin(), streetNames[i].end(), ' '), streetNames[i].end());
            std::transform(streetNames[i].begin(), streetNames[i].end(), streetNames[i].begin(),
                           [](unsigned char c) { return std::tolower(c); });
        }

        root = makeNode(); //creating root for trie

        for (int i = 0; i < numOfStreets; i++) { //inputs all streetnames and indices into the trie
            insertToTrie(root, streetNames[i], i);
        }

        segments_of_an_intersection.resize(getNumIntersections());

        street_names_of_intersection.resize(getNumIntersections());

        intersections_of_a_street.resize(getNumStreets());

        intersections_of_two_streets.resize(getNumStreets());

        adjacent_intersections.resize(getNumIntersections());
        
        for(int i = 0; i < getNumStreetSegments(); i++){
            street_segment_info.push_back(getStreetSegmentInfo(i)); // vector is filled with struct of info
        }
        
//        for (int intersection = 0; intersection < getNumIntersections(); intersection++){
//            for (int segment = 0; segment < getNumIntersectionStreetSegment(intersection); segment++){
//                int streetSeg_id = getIntersectionStreetSegment(intersection, segment);
//                segments_of_an_intersection[intersection].push_back(streetSeg_id);
//                  
//                /*FINDING THE STREET NAMES THAT PASS THROUGH AN INTERSECTION*/
//                //gets the street id of a specific segment
//                StreetIdx street_ID_of_segment = getStreetSegmentInfo(streetSeg_id).streetID;
//                //stores the name at specified street id in th street names function
//                street_names_of_intersection[intersection].push_back(getStreetName(street_ID_of_segment));
//
//                 /*FINDING INTERSECTIONS OF A STREET*/
//                ///if the intersection does not exists on the list of intersections of a street, then add it
//                if (!(std::find(intersections_of_a_street[street_ID_of_segment].begin(), intersections_of_a_street[street_ID_of_segment].end(), intersection) != intersections_of_a_street[street_ID_of_segment].end())){  
//                    intersections_of_a_street[street_ID_of_segment].push_back(intersection);
//                }
//                
//                 /*FINDING ADJACENT INTERSECTIONS*/
//                 /*logic: since each segment has one "from" and one "to" intersection ids,
//                  * then as we loop through the segments we can check if the current intersection 
//                  * is the "from" or the "to" of the segment
//                  * if it is the "from", then we add the "to" of the segment to it's adjacent intersections
//                  * if it's the to, we move on to the next segment
//                  * This will allow each intersection to be added only once to another intersection's adjacent list
//                  * while only going in a direction that fits one way streets */
//                
//                 //check if the current intersection  is the "from" intersection
//                if(intersection == getStreetSegmentInfo(segment).from){
//                    //add the "to" intersection to the adjacent intersections of the current intersection
//                    adjacent_intersections[intersection].push_back(getStreetSegmentInfo(segment).to);
//                }  
//            }
//        }

        for (int intersection = 0; intersection < getNumIntersections(); intersection++) {
            for (int segment = 0; segment < getNumIntersectionStreetSegment(intersection); segment++) {
                int streetSeg_id = getIntersectionStreetSegment(intersection, segment);
                segments_of_an_intersection[intersection].push_back(streetSeg_id);

                /*FINDING THE STREET NAMES THAT PASS THROUGH AN INTERSECTION*/
                //gets the street id of a specific segment
                StreetIdx street_ID_of_segment = getStreetSegmentInfo(streetSeg_id).streetID;
                //stores the name at specified street id in th street names function
                street_names_of_intersection[intersection].push_back(getStreetName(street_ID_of_segment));

                /*FINDING INTERSECTIONS OF A STREET*/
                ///if the intersection does not exists on the list of intersections of a street, then add it
                if (!(std::find(intersections_of_a_street[street_ID_of_segment].begin(),
                                intersections_of_a_street[street_ID_of_segment].end(), intersection) !=
                      intersections_of_a_street[street_ID_of_segment].end())) {
                    intersections_of_a_street[street_ID_of_segment].push_back(intersection);
                }



                /*FINDING ADJACENT INTERSECTIONS*/
                /*logic: since each segment has one "from" and one "to" intersection ids,
                 * then as we loop through the segments we can check if the current intersection
                 * is the "from" or the "to" of the segment
                 * if it is the "from", then we add the "to" of the segment to it's adjacent intersections
                 * if it's the to, we move on to the next segment
                 * This will allow each intersection to be added only once to another intersection's adjacent list
                 * while only going in a direction that fits one way streets */

                //check if the current intersection  is the "from" intersection
                if (intersection == getStreetSegmentInfo(segment).from) {
                    //add the "to" intersection to the adjacent intersections of the current intersection
                    adjacent_intersections[intersection].push_back(getStreetSegmentInfo(segment).to);
                }


            }
        }
        for (StreetIdx first_street_idx = 0; first_street_idx < getNumStreets(); first_street_idx++) {
            for (int intersection = 0;
                 intersection < intersections_of_a_street[first_street_idx].size(); intersection++) {
                for (int segment = 0; segment < segments_of_an_intersection[intersection].size(); segment++) {
                    StreetIdx second_street_idx = getStreetSegmentInfo(
                            segments_of_an_intersection[intersection][segment]).streetID;
                    if (first_street_idx != second_street_idx) {
                        std::pair<StreetIdx, StreetIdx> original_id_pair(first_street_idx, second_street_idx);
                        std::pair<StreetIdx, StreetIdx> reverse_id_pair(second_street_idx, first_street_idx);
                        if (std::find(intersections_of_two_streets[first_street_idx].begin(),
                                      intersections_of_two_streets[first_street_idx].end(), original_id_pair) !=
                            intersections_of_two_streets[first_street_idx].end()) {
                            if (std::find(intersections_of_two_streets[first_street_idx].begin(),
                                          intersections_of_two_streets[first_street_idx].end(), reverse_id_pair) !=
                                intersections_of_two_streets[first_street_idx].end()) {
                                intersections_of_two_streets[first_street_idx].push_back(original_id_pair);
                            }
                        }
                    }
                }
            }
        }

        return load_successful;
    }
}

void closeMap() {
    //Clean-up your map related data structures here
    closeStreetDatabase();

    std::vector<std::vector<std::string>> GARBAGE_street_names_of_intersection;
    street_names_of_intersection.swap(GARBAGE_street_names_of_intersection);

    std::vector<std::vector<IntersectionIdx>> GARBAGE_intersections_of_a_street;
    intersections_of_a_street.swap(GARBAGE_intersections_of_a_street);

    std::vector<std::vector<StreetSegmentIdx>> GARBAGE_segments_of_an_intersection;
    segments_of_an_intersection.swap(GARBAGE_segments_of_an_intersection);

    std::vector<std::vector<IntersectionIdx>> GARBAGE_adjacent_intersections;
    adjacent_intersections.swap(GARBAGE_adjacent_intersections);

    std::vector<std::vector<std::pair<StreetIdx, StreetIdx>>> GARBAGE_intersections_of_two_streets;
    intersections_of_two_streets.swap(GARBAGE_intersections_of_two_streets);


    delete[] streetNames;


    destroyTrie(root);//dealloc trie

}

double degToRad(double degree) {//convert degrees to radians
    return (((M_PI) / 180) * degree);
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
        avgLat += degToRad(featurePoints[i].latitude());
    }

    avgLat = avgLat / numOfPoints;

    double *x = new double[numOfPoints];
    double *y = new double[numOfPoints];
    double radLat, radLong;

    for (int i = 0; i < numOfPoints; i++) {
        radLat = degToRad(featurePoints[i].latitude());
        radLong = degToRad(featurePoints[i].longitude());

        x[i] = RADIUS_OF_EARTH * radLong * cos(avgLat);
        y[i] = RADIUS_OF_EARTH * radLat;

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

std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix) {
    street_prefix.erase(std::remove(street_prefix.begin(), street_prefix.end(), ' '), street_prefix.end());
    std::transform(street_prefix.begin(), street_prefix.end(), street_prefix.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    return findStreetName(root, street_prefix);
}

double findDistanceBetweenTwoPoints(std::pair<LatLon, LatLon> points) {
    // Converting latitude and longitudes from degrees to radians
    double lat1 = kDegreeToRadian * (points.first.latitude());
    double long1 = kDegreeToRadian * (points.first.longitude());
    double lat2 = kDegreeToRadian * (points.second.latitude());
    double long2 = kDegreeToRadian * (points.second.longitude());

    double latAvg = (lat2 + lat1) / 2;
    double x1 = RADIUS_OF_EARTH* long1 * (cos(latAvg));
    double x2 = RADIUS_OF_EARTH* long2 * (cos(latAvg));
    double y1 = RADIUS_OF_EARTH * lat1;
    double y2 = RADIUS_OF_EARTH* lat2;
    std::cout.precision(30);
//    std::cout << "x: " << x1 << " y: " << y1 << std::endl;
//    std::cout << "x: " << x2 << " y: " << y2 << std::endl;
    double diffinY = y2 - y1;
    double diffinX = x2 - x1;

    double power1 = pow(diffinY, 2);
    double power2 = pow(diffinX, 2);
//    std::cout << (sqrt(power1 + power2)) << std::endl;
    return (sqrt(power1 + power2));
}

LatLonBounds findStreetBoundingBox(StreetIdx street_id) {
    LatLonBounds stub;
    return stub;
}


double findStreetLength(StreetIdx street_id){
    double length = 0;
    for(int i = 0; i < street_segment_info.size() ; i++){
        if(street_segment_info[i].streetID == street_id){
            length += findStreetSegmentLength(i);
        }
    }
    return length;
}

double findStreetSegmentLength(StreetSegmentIdx street_segment_id) {
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
            if(i == 0){
//                std::cout << i << std::endl;
                std::pair<LatLon, LatLon> fromToFirstCurve = std::make_pair(getIntersectionPosition(from),
                                                                            getStreetSegmentCurvePoint(street_segment_id, 0));
                length += findDistanceBetweenTwoPoints(fromToFirstCurve);
                previousPoint = getStreetSegmentCurvePoint(street_segment_id, 0);

            }else if(i == numOfCurvePoints){
//                std::cout << i << std::endl;
                std::pair<LatLon, LatLon> lastCurvePointToTo = std::make_pair(getIntersectionPosition(to), previousPoint);
                length += findDistanceBetweenTwoPoints(lastCurvePointToTo);
            }else{
//                std::cout << i << std::endl;
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
    // fetch the speed limit of the segment in question
    float speedLimit = getStreetSegmentInfo(street_segment_id).speedLimit;
    // return the speed which is length/time
    return findStreetSegmentLength(street_segment_id) / speedLimit;

}




//GHAMR'S FUNCTIONS////////////////////////////////////////////////////////////////////



// Returns the nearest intersection to the given position
// Speed Requirement --> none
IntersectionIdx findClosestIntersection(LatLon my_position) {
    IntersectionIdx stub;
    return stub;
}






// Return all intersection ids at which the two given streets intersect
// This function will typically return one intersection id for streets
// that intersect and a length 0 vector for streets that do not. For unusual 
// curved streets it is possible to have more than one intersection at which 
// two streets cross.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high
std::vector<IntersectionIdx> findIntersectionsOfTwoStreets(std::pair<StreetIdx, StreetIdx> street_ids){

    return {};
}







//GHAMR: DONE////////////////////////////////////////////////////////////////////////

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

    return adjacent_intersections[intersection_id];
}