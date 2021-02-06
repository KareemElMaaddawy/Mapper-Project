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
#define rOfEarth 6371000
#include "algorithm"

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



std::vector<std::vector<StreetSegmentIdx>> segments_of_an_intersection;
std::vector<std::vector<std::string>> street_names_of_intersection; //stores the street names for each intersection
                                                                    //Includes repetition!!


std::vector<std::vector<IntersectionIdx>> intersections_of_a_street;     

bool loadMap(std::string map_streets_database_filename) {
    bool load_successful = false; //Indicates whether the map has loaded 
                                  //successfully
    segments_of_an_intersection.resize(getNumIntersections());
    
    street_names_of_intersection.resize(getNumIntersections());
    intersections_of_a_street.resize(getNumStreets());
    
    
    for (int intersection = 0; intersection < getNumIntersections(); intersection++){
        for (int i = 0; i < getNumIntersectionStreetSegment(intersection); i++){
            int streetSeg_id = getIntersectionStreetSegment(intersection, i);
            segments_of_an_intersection[intersection].push_back(streetSeg_id);
            
            
            StreetIdx street_ID_of_segment = getStreetSegmentInfo(streetSeg_id).streetID;//gets the street id of a specific segment
            street_names_of_intersection[intersection].push_back(getStreetName(street_ID_of_segment));//stores the name at specified street id in th street names function
            
            
            ///if the intersection does not exists on the list of intersections of a street, then add it
            if (!(std::find(intersections_of_a_street[street_ID_of_segment].begin(), intersections_of_a_street[street_ID_of_segment].end(), intersection) != intersections_of_a_street[street_ID_of_segment].end())){  
                intersections_of_a_street[street_ID_of_segment].push_back(intersection);
            }
        }
    }

    std::cout << "loadMap: " << map_streets_database_filename << std::endl;

    //
    // Load your map related data structures here.
    //
    
    
    
    
    
    

    load_successful = true; //Make sure this is updated to reflect whether
                            //loading the map succeeded or failed
    load_successful = loadStreetsDatabaseBIN(map_streets_database_filename);

    return load_successful;
}

void closeMap() {
    //Clean-up your map related data structures here
    
}

// Returns the nearest point of interest of the given name to the given position
// Speed Requirement --> none 
POIIdx findClosestPOI(LatLon my_position, std::string POIname){
    double smallestDistance = 99999999999;
    double tempDistance = 0;
    int totalNumOfPOI = getNumPointsOfInterest();
    POIIdx closestPOI = -1;
    std::pair <LatLon,LatLon> positions; //pair to hold values of present location, and the poi's location
    for(int i = 0; i < totalNumOfPOI; i++){
        if(getPOIName(i) == POIname){
            positions = std::make_pair(my_position,getPOIPosition(i));
            tempDistance = findDistanceBetweenTwoPoints(positions);
            if(tempDistance < smallestDistance){ //stores value of smallest distance and the POI index for it
                smallestDistance = tempDistance;
                closestPOI = i;
            }
        }
    }
    return closestPOI;
}

// Returns the area of the given closed feature in square meters
// Assume a non self-intersecting polygon (i.e. no holes)
// Return 0 if this feature is not a closed polygon.
// Speed Requirement --> moderate
double findFeatureArea(FeatureIdx feature_id){
    int numOfPoints = getNumFeaturePoints(feature_id);//finds number of points on the feature
    LatLon *featurePoints = new LatLon[numOfPoints];
    double area = 0;
    for(int i = 0; i < numOfPoints; i++){
        featurePoints[i] = getFeaturePoint(feature_id, i);//creates array of featurePoints
    }
    if(featurePoints[0] == featurePoints[numOfPoints-1]){ //returns area of polygon if closed polygon
        double sum = 0;
        for(int i = 0; i < numOfPoints; i++){
            if(i == numOfPoints - 1){
                sum += featurePoints[i].latitude()*featurePoints[0].longitude() - featurePoints[i].longitude()*featurePoints[0].latitude();
                delete[] featurePoints;
            }else{
                sum += featurePoints[i].latitude()*featurePoints[i+1].longitude() - featurePoints[i].longitude()*featurePoints[i+1].latitude();
            }
        }
        return abs(sum / 2);
    }else{
        return area; //returns 0 if not a closed area
    }
}

// Helper function to convert degrees to radians
double degToRad(double degree);
double degToRad(double degree){
    double deg = (M_PI)/180;
    return (deg * degree);
}

double findDistanceBetweenTwoPoints(std::pair<LatLon, LatLon> points){
    // Converting latitude and longitudes from degrees to radians
   double lat1 = degToRad(points.first.latitude());
   double long1 = degToRad(points.first.longitude());
   double lat2 = degToRad(points.second.latitude());
   double long2 = degToRad(points.second.longitude());
   
   double latDistance = lat2 - lat1;
   double longDistance = long2 - long1;
   
   double ans = pow(sin(latDistance/2),2) + cos(lat1)*cos(lat2)*pow(sin(longDistance/2),2);
   ans = 2 * asin(sqrt(ans));
   
   ans = ans * rOfEarth;
   
   return ans;
   
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
std::vector<StreetIdx> findStreetIdsFromPartialStreetName(std::string street_prefix){
    std::vector<StreetIdx> stub;
    return stub;
}

LatLonBounds findStreetBoundingBox(StreetIdx street_id){
    LatLonBounds stub;
    return stub;
}

double findStreetLength(StreetIdx street_id){
    double stub;
    return stub;
}

double findStreetSegmentLength(StreetSegmentIdx street_segment_id){
    double stub;
    return stub;
}

double findStreetSegmentTravelTime(StreetSegmentIdx street_segment_id){
    double stub;
    return stub;
}




//GHAMR'S FUNCTIONS////////////////////////////////////////////////////////////////////



// Returns the nearest intersection to the given position
// Speed Requirement --> none
IntersectionIdx findClosestIntersection(LatLon my_position){
    IntersectionIdx stub;
    return stub;
}



// Returns all intersections reachable by traveling down one street segment 
// from the given intersection (hint: you can't travel the wrong way on a 
// 1-way street)
// the returned vector should NOT contain duplicate intersections
// Speed Requirement --> high 
std::vector<IntersectionIdx> findAdjacentIntersections(IntersectionIdx intersection_id){
    std::vector<IntersectionIdx> stub;
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
    std::vector<IntersectionIdx> stub;
    return stub;
}



    




//GHAMR: DONE////////////////////////////////////////////////////////////////////////

// Returns the street names at the given intersection (includes duplicate 
// street names in the returned vector)
// Speed Requirement --> high 
std::vector<std::string> findStreetNamesOfIntersection(IntersectionIdx intersection_id){
    
    return street_names_of_intersection[intersection_id];
}

// Returns the street segments that connect to the given intersection 
// Speed Requirement --> high
std::vector<StreetSegmentIdx> findStreetSegmentsOfIntersection(IntersectionIdx intersection_id){
   
    return segments_of_an_intersection[intersection_id];
}

// Returns all intersections along the a given street.
// There should be no duplicate intersections in the returned vector.
// Speed Requirement --> high
std::vector<IntersectionIdx> findIntersectionsOfStreet(StreetIdx street_id){
    
   
    return intersections_of_a_street[street_id];
    
}